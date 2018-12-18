#pragma once
#include<iostream>

template<bool threads,int inst>
class _default_alloc_template {
private:
	enum { _ALIGN = 8 };//С�������ϵ��߽�	
	enum { _MAX_BYTES = 128 };//����
	enum { _NFREELISTS = _MAX_BYTES / _ALIGN };//FREE_LIST ����

private:
	static size_t ROUND_UP(size_t bytes)//��bytes�ϵ���8�ı���
	{
		return ((bytes)+_ALIGN - 1)&~(_ALIGN - 1);
	}

private:
	union obj {//�ڵ�����
		union obj* free_list_link;
		char client_data[1];
	};

private:
	static obj* volatile free_list[_NFREELISTS];//16��free_list
	static size_t FREELIST_INDEX(size_t bytes)//���������λ��
	{
		return (((bytes)+_ALIGN - 1) / _ALIGN - 1);
	}
	static void* refill(size_t n);
	static char* chunk_alloc(size_t size, int &nobjs);
	static char* start_free;
	static char* end_free;
	static size_t heap_size;

public:
	static void* allocate(size_t n);
	static void deallocate(void* p,size_t n);
	static void* reallocate(void* p, size_t old_sz, size_t new_sz);

};

template<bool threads, int inst>
void* _default_alloc_template<threads, inst>::reallocate(void* p, size_t old_sz, size_t new_sz)
{
	void* result = allocate(new_sz);
	deallocate(p, old_sz);
	return result;
}

//size�ѵ���Ϊ8�ı���
template<bool threads, int inst>
char* _default_alloc_template<threads, inst>::chunk_alloc(size_t size, int &nobjs)
{
	char* result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free;//�ڴ��ʣ��ռ�

	if (bytes_left >= total_bytes)//ʣ��ռ��������ռ�
	{
		result = start_free;
		start_free += total_bytes;
		return(result);
	}
	else if (bytes_left >= size)//ʣ��ռ�ֻ��һ���ռ�
	{
		nobjs = bytes_left / size;//ȷ���㹻������
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return(result);
	}
	else//���϶��޷�����
	{
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);

		if (bytes_left > 0)
		{
			obj* volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
			//������ռ���뵽free_list�ĺ��ʿռ�
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj*)start_free;
		}
		start_free = (char*)malloc(bytes_to_get);//�����ڴ��
		if (0 == start_free)//����ʧ��
		{
			int i;
			obj* volatile *my_free_list, *p;
			for (i = size; i < _MAX_BYTES; i += _ALIGN)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p)
				{
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					return (chunk_alloc(size, nobjs));
				}
			}
			end_free = 0;
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return(chunk_alloc(size, nobjs));
	}

}

template<bool threads, int inst>
void* _default_alloc_template<threads, inst>::refill(size_t n)
{
	int nobjs = 20;
	char* chunk = chunk_alloc(n, nobjs);//ȡ��nobjs���ڵ���Ϊ�½ڵ�-���²���һ�������Ĵ���ڴ�
	obj* volatile *my_free_list;
	obj* result;
	obj* current_obj, *next_obj;
	int i;
	if (1 == nobjs)return(chunk);//���ֻ���һ���ڵ� ֱ�ӷ��ؽڵ�
	my_free_list = free_list + FREELIST_INDEX(n);//�������free_list�����½ڵ�
	result = (obj*)chunk;//���ظ��ͻ���ʹ��
	*my_free_list = next_obj = (obj*)(chunk + n);//����ڵ���¿�ͷ
	for (i = 1;; ++i)
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);
		if (nobjs - 1 == i)
		{
			current_obj->free_list_link = 0;
			break;
		}
		else
			current_obj->free_list_link = next_obj;
	}
	return(result);

}

template<bool threads,int inst>
void* _default_alloc_template<threads, inst>::allocate(size_t n)
{
	std::cout << "using _default_alloc_template" << std::endl;
	obj* volatile *my_free_list;
	obj* result;
	if (n > (size_t)_MAX_BYTES)
	{
		return (malloc_alloc::allocate(n));
	}
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0)
	{
		void* r = refill(ROUND_UP(n));//�������
		return r;
	}
	*my_free_list = result->free_list_link;//��ָ��ָ����һ����ַ
	return(result);
}

template<bool threads, int inst>
void _default_alloc_template<threads, inst>::deallocate(void* p,size_t n)
{
	std::cout << "using _default_alloc_template deallocate" << std::endl;
	obj* q = (obj*)p;
	obj* volatile *my_free_list;
	if (n > (size_t)_MAX_BYTES)
	{
		malloc_alloc::deallocate(p, n);
		return;
	}
	my_free_list = free_list + FREELIST_INDEX(n);
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

template<bool threads,int inst>
char* _default_alloc_template<threads, inst>::start_free = 0;

template<bool threads, int inst>
char* _default_alloc_template<threads, inst>::end_free = 0;

template<bool threads,int inst>
size_t _default_alloc_template<threads, inst>::heap_size = 0;

//��Ҫʹ��typename�����߱����� ���Ǹ�����
template<bool threads,int inst>
typename _default_alloc_template<threads, inst>::obj* volatile
_default_alloc_template<threads, inst>::free_list[_NFREELISTS] =
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };

