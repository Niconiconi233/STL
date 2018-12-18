/*从0开始存储元素
*父节点为(i-1)/2
*左节点为2*i+1;
*右为2*i+2;
*堆的根节点为最大或最小值
*堆内部无序 只能保证父节点大于子节点或小于子节点
*/
#include"iterator_traits.h"
template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__make_heap(first, last, value_type(first), distance_type(first));
}

	template<class RandomAccessIterator, class T, class Distance>
	void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
		if (last - first < 2)//长度太短
			return;
		Distance len = last - first;

		Distance parent = (len - 2) / 2;

		while (true) {
			__adjust_heap(first, parent, len, T(*(first + parent)));
			if (parent == 0)
				return;
			--parent;
		}
	}

	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
		while (last - first > 1) {
			pop_heap(first, last--);
		}
	}

	template<class RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
		__pop_heap_aux(first, last, value_type(first));
	}

	template<class RandomAccessIterator, class T>
	inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
		__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
	}

	template<class RandomAccessIterator, class T, class Distance>
	inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
		*result = *first;
		__adjust_heap(first, Distance(0), Distance(last - first), value);
	}

	template<class RandomAccessIterator, class Distance, class T>
	void __adjust_heap(RandomAccessIterator first, Distance holeindex, Distance len, T value) {
		Distance topIndex = holeindex;
		Distance secondchild = 2 * holeindex + 2;//右节点下标
		while (secondchild < len) {
			if (*(first + secondchild) < *(first + (secondchild - 1)))//找到大节点
				--secondchild;
			*(first + holeindex) = *(first + secondchild);
			holeindex = secondchild;
			secondchild = 2 * (secondchild + 1);//找到右节点
		}
		if (secondchild == len) {
			*(first + holeindex) = *(first + (secondchild - 1));
			holeindex = secondchild - 1;
		}

		__push_heap(first, holeindex, topIndex, value);
	}

	template<typename RandomAccessIterator>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		__push_heap_aux(first, last, distance_type(first), value_type(first));
	}

	template<typename RandomAccessIterator, typename Distance, typename T>
	inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
		__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
	}

	template<typename RandomAccessIterator, typename Distance, typename T>
	void __push_heap(RandomAccessIterator first, Distance holeindex, Distance topindex, T value) {
		Distance parent = (holeindex - 1) / 2;//parent
		while (holeindex > topindex&&*(first + parent) < value) {//小于头节点并且值大于父节点
			*(first + holeindex) = *(first + parent);//旧节点赋值为父节点的值
			holeindex = parent;//修改节点地址
			parent = (holeindex - 1) / 2;//寻找新父节点	
		}
		*(first + holeindex) = value;
	}
