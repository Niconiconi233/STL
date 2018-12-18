#include"iterator_traits.h"
#include"type_traits.h"

template<typename InputIterator,typename T>
T accumulate(InputIterator first, InputIterator last, T init) {
	for (; first != last; ++first)
		init += *first;
	return init;
}

template<typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
	for (; first != last; ++first)
		init = binary_op(init, *first);
	return init;
}

template<typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last)
		return result;
	*result = *first;
	return __adjacent_difference(first, last, result, value_type(first));
}

template<typename InputIterator, typename OutputIterator, typename T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*) {
	T value = *first;
	while (*++first != last) {
		T tmp = *first;
		*++result = tmp - value;
		value = tmp;
	}
	return ++result;
}

template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
	if (first == last)
		return result;
	*result = *first;
	return __adjacent_difference(first, last, result, value_type(first), binary_op);
}

template<typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op) {
	T value = *first;
	while (*++first != last) {
		T tmp = *first;
		*++result = binary_op(tmp, value);
		value = tmp;
	}
	return ++result;
}

template<typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
	for (; first1 != last1; ++first1, ++first2) {
		init += (*first * first2);
	}
	return init;
}

template<typename InputIterator1, typename InputIterator2, typename T, typename BinaryOperation1, typename BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
	for (; first1 != last1; ++first1, ++first2) {
		init = binary_op1(init, binary_op2(*first1, *first2));
	}
	return init;
}

template<typename InputIterator, typename OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator out) {
	*out = *first;
	return __partial_sum(first, last, out, value_type(first));
}

template<typename InputIterator, typename OutputIterator, typename T>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator out, T*) {
	T value = *first;
	while (*++first != last) {
		value += *first;
		*++out = value;
	}
	return ++out;
}

template<typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last, OutputIterator out, T*, BinaryOperation binary_op) {
	T value = *first;
	while (*++first != last) {
		value = binary_op(value, *first);
		*++out = value;
	}
	return ++out;
}

template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator out, BinaryOperation binary_op) {
	*out = *first;
	return __partial_sum(first, last, out, value_type(first), binary_op);
}

template<typename T, typename Integer>
inline T power(T x, Integer n) {
	return power(x, n, multiplies<T>());//multiplies return left* right;
}

template<typename T, typename Integer, typename MonoidOperation>
T power(T x, Integer n, MonoidOperation op) {//?
	if (n == 0)
		return identity_element(op);
	else {
		while ((n & 1) == 0) {//1 3 5 7 9 ...
			n >>= 1;
			x = op(x, x);
		}
		T result = x;
		n >>= 1;
		while (n != 0) {
			x = op(x, x);
			if ((n & 1) != 0)
				result = op(result, x);
			n >> = 1;
		}
		return result;
	}
}

template<typename InputIterator, typename T>
void iota(InputIterator first, InputIterator last, T value) {
	while (first != last) {
		*first++ += value++;
	}
}

/*
if(vec1.size() == vec2.size() && equal(vec1.begin(), vec1.end(), vec2.begin()))
*/
template<typename InputIterator1, typename InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2)
		if (*first1 != *first2)
			return false;
	return true;
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++last1)
		if (!binary_pred(*first1, *first2))
			return false;
	return true;
}

template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
	for (; first != last; ++first) {
		*first = value;
	}
}

template<typename OutputIterator, typename T>
OutputIterator fill_n(OutputIterator first, OutputIterator last, size_t n, const T& value) {
	//for(; first != last && n > 0; ++first, --n)
	for (; n > 0; --n, ++first)
		*first = value;
	return first;
}

template<typename ForwardIterator1, typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	__iter_swap(a, b, value_type(a));
}

template<typename ForwardIterator1, typename ForwardIterator2, typename T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

template<typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2)
			return true;
		if (*first2 < *first1)
			return false;
	}
	return first1 == last1 && first2 != last2;
}

template<typename InputIterator1, typename InputIterator2, typename Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare compare) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (compare(*first1, *first2))
			return true;
		if (compare(*first2, *first1))
			return false;
	}
	return first1 == last1 && first2 != last2;
}

inline bool
lexicographical_compare(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2) {
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;

	const int result = memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;
}

template<typename T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;
}

template<typename T, typename Compare>
inline const T& max(const T& a, const T& b, Compare cmp) {
	return cmp(a, b) ? b : a;
}

template<typename T>
inline const T& min(const T& a, const T& b) {
	return a < b ? a : b;
}

template<typename T, typename Compare>
inline const T& min(const T& a, const T& b, Compare cmp) {
	return cmp(a, b) ? a : b;
}

template<typename InputIterator1, typename InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	while (first1 != last1 && *first1 == *first2) {
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
	while (first1 != last1 && binary_pred(*first1, *first2)) {
		++first1;
		++first2;
	}
	return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<typename T>
inline void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

//                                                     copy
//针对原生指针 调用copy函数直接使用memmove来内存拷贝
//针对非原生指针 调用copy_t判断是否是基本类型如果是调用memmove 否则调用copy_d
//针对迭代器 调用__copy()判断迭代器类型 使用不同的拷贝方式
//

template<typename InputIterator, typename OutputIterator>
struct __copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}

};
//偏特化版本 int* obj* 类型的指针 非迭代器
template<typename T>
struct __copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		typedef typename __type_traits <T>::has_trivial_assignment_operator t;//struct false_type{} && true_type{}
		return __copy_t(first, last, result, t());
	}

};

template<typename T>
struct __copy_dispatch<const T*, T*> {
	T* operator()(const T* first, const T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}

};

//copy版本 input_iterator is struct like foward bid random
template<typename InputIterator, typename OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
	for (; first != last; ++first, ++result)//以迭代器是否相同决定是否循环 速度慢
		*result = *first;
	return result;
}

//random_iterator版本
template<typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));
}

template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	for (Distance n = last - first; n > 0; --n, ++result, ++first) {//以n决定是否循环 速度快
		*result = *first;
	}
	return result;
}


template<typename T>//基本类型调用memmove()
inline T* copy_t(const T* first, const T* last, T* result, __true_type) {
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

template<typename T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}


template<typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);//实例化一个对象调用operator()

}
//对原生指针的特化
inline char* copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

//copy_backward

template<typename BidirectionalIterator1, typename BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
	//like copy()
	mommove(result - (last - first), last, -(last - first));//*(res - 1) = *(last - 1)->*(res - 2) = *(res - 2)
}

//set set会自动排序

template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	//如果其中一个到达末尾 则将剩下的拷贝到result 其中一个是空的
	return copy(first2, last2, copy(first1, last1, result));
}

template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_intersetction(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			++first1;
		}
		else if (*first1 > *first2) {
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
	}
	return result;
}
//输出s1里s2没有的
template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first2;
			++result;
		}
		else if (*first2 < *first1) {
			++first2;
		}
		else {
			++first1;
			++first2;
		}
	}
	//到达末尾 后面的元素s2都没有
	return copy(first1, last1, result);
}

template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++first2;
			++result;
		}
		else {
			++first1;
			++first2;
		}
	}
	return copy(first2, last2, copy(first1, last1, result));
}

//查找相邻的重复元素
template<typename ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next)
			return first;
		first = next;
	}
	return last;
}

template<typename ForwardIterator, typename BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	if (first == last)
		return;
	ForwardIterator next = first;
	while (++next != last) {
		if (binary_pred(*first, *next))return first;
		first = next;
	}
	return last;
}

//计数
template<class InputIterator, typename T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
	typename iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first) {
		if (*first == value)
			++n;
	}
	return n;
}
//指定n值
template<typename InputIterator, typename T, typename Size>
void count(InputIterator first, InputIterator last, const T& value, Size& n) {
	for (; first != last; ++first) {
		if (*first == value)
			++n;
	}
}

template<typename InputIterator, typename Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
	typename iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first) {
		if (pred(*first))
			++n;
	}
	return n;
}

template<typename InputIterator, typename Predicate, typename Size>
void count_if(InputIterator first, InputIterator last, Size& n, Predicate pred) {
	for (; first != last; ++first) {
		if (pred(*first))
			++n;
	}
}

//find
template<typename InputIterator, typename T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
	while (first != last && *first != value) ++first;
	return first;
}
//bind2nd 绑定的是第二个元素
template<typename InputIterator, typename Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
	while (first != last && !pred(*first))
		++first;
	return first;
}

template<typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	typedef typename iterator_triats<ForwardIterator1>::iterator_category category1;
	typedef typename iterator_triats<ForwardIterator2>::iterator_category category2;
	return __find_end(first1, last1, first2, last2, category1(), category2());
}

template<typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, forward_iterator_tag, forward_iterator_tag) {
	if (first2 == last2)
		return last1;
	else {
		ForwardIterator1 result = last1;
		while (1) {
			ForwardIterator1 new_result = search(first1, last1, first2, last2);//返回寻找的first
			if (new_result == last1)
				return result;
			else {
				result = new_result;
				first1 = new_result;
				++first1;
			}
		}
	}
}

template<typename BidirectionalIterator1, typename BidirectionalIterator2>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag) {
	typedef reverse_iterator<BidirectionalIterator1> reviter1;//反向迭代器
	typedef reverse_iterator<BidirectionalIterator2> reviter2;

	reviter1 rlast1(first1);
	reviter2 rlast2(first2);

	reciter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);
	if (rresult == rlast1)
		return last1;
	else {
		BidirectionalIterator1 result = rrsult.base();
		advance(result, -distance(first2, last2));
		return result;
	}
}

template<typename InputIterator, typename ForwardIterator>//只判断第一个元素是否匹配
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2) {
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)
			if (*first1 == *iter)
				return first1;
	return last1;
}

template<typename InputIterator, typename ForwardIterator, typename BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2, BinaryPredicate binary_pre) {
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)
			if (binary_pre(*first1, *iter))
				return first1;
	return last1;
}


template<typename InputIterator, typename Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
	for (; first != last; ++first)
		f(*first);
	return f;
}

template<typename ForwardIterator, typename Generator>//将仿函数的值作用在[first,last)
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
	for (; first != last; ++first)
		*first = gen();
}

template<typename OutputIterator, typename Size, typename Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
	for (; n > 0; --n, ++first)
		*first = gen();
	return first;
}

//includes
template<typename InputIterator1, typename InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1)
			return false;//涵盖的条件不成立
		else if (*first1 < *first2)
			++first1;
		else
			++first1, ++first2;
	}
	return first2 == last2;//判断是否到达最后
}

template<typename InputIterator1, typename InputIterator2, typename Compare>
bool includes(InputIterator1 first1, InputIterator2 last1, InputIterator2 first2, InputIterator2 last2, Compare cmp) {
	while (first1 != last1 && first2 != last2) {
		if (cmp(*first1, *first2))
			return false;
		else if (cmp(*first1, *first2))
			++first1;
		else
			++first1, ++first2;
	}
	return first2 == last2;
}

template<typename ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last) {
		if (*result < *first)
			result = first;
	}
	return result;
}

template<typename ForwardIterator, typename Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare cmp) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last) {
		if (cmp(*result, *first))
			result = first;
	}
	return result;
}

template<typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return copy(first2, last2, copy(first1, last1, result));
}

template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare cmp) {
	while (first1 != last1 && first2 != last2) {
		if (cmp(*first2, *first1)) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return copy(first2, last2, copy(first1, last1, result));
}

template<typename ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last) {
		if (*first < *result)
			result = first;
	}
	return result;
}

template<typename ForwardIterator, typename Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare cmp) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last) {
		if (cmp(*first, *result))
			result = first;
	}
	return result;
}

template<typename BidirectionalIterator, typename Predicate>
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
	while (true) {//pred 判断为true 的元素 在前面 false 的在后面
		while (true)
			if (first == last)
				return first;
			else if (pred(*first))//不符合移动条件
				++first;
			else
				break;
		--last;
		while (true)
			if (first == last)
				return first;
			else if (!pred(*last))//*last 不符合移动条件
				--last;
			else
				break;
		iter_swap(first, last);//交换头尾指针的元素
		++first;
	}
}

template<typename ForwardIterator, typename T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
	first = find(first, alst, value);//第一个匹配元素
	ForwardIterator next = first;
	return first == last ? first : remove_copy(++next, last, first, value);
}

template<typename InputIterator, typename OutputIterator, typename T>
OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value) {
	for(; first != last; ++first)
		if (*first != value) {
			*result = *first;
			++result;
		}
	return result;
}

template<typename ForwardIterator, typename Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
	first = find_if(first, last, pred);
	ForwardIterator next = first;
	return next == last ? last : remove_copy_if(++next, last, first, pred);
}

template<typename InputIterator, typename OutputIterator, typename Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred) {
	for( ;first != last; ++first)
		if (pred(*first)) {
			*result = *first;
			++result;
		}
	return result;
}

template<typename ForwardIterator, typename T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
	for (; first != last; ++first)
		if (*first == old_value)
			*first = new_value;
}

template<typename InputIterator, typename OutputIterator, typename T>
OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value) {
	for (; first != last; ++first)
		if (*first != old_value)
			*result++ = *first;
	return result;
}

template<typename ForwardIterator, typename Predicate, typename T>
void replcae_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value) {
	for (; first != last; ++first)
		if (pred(*first))
			*first = new_value;
}

template<typename Iterator, typename OutputIterator, typename Predicate, typename T>
OutputIterator replcae_copy_if(Iterator first, Iterator last, OutputIterator result, Predicate pred, const T& new_value) {
	for (; first != last; ++first, ++result)
		*result = pred(*first) ? new_value : *first;
	return result;
}

template<typename BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
	while(true){
		if(first == last || first == --last)//会执行--last
			return;
		else
			iter_swap(first++, last);
	}
}

template<typename RandAccessIterator>
void __reverse(RandAccessIterator first, RandAccessIterator last, random_access_iterator_tag) {
	while(first < last)//只有randomitertor 才能做<操作
		iter_swap(first, last);
}

template<typename BidirectonalIterator>
inline void reverse(BidirectonalIterator first, BidirectonalIterator last) {
	__reverse(first, last, Iterator_category(first));
}

template<typename BidirectionalIterator, typename OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
	while(first != last) {
		--last;
		*result = *last;//*result++ = *--last;
		++result;
	}
	return result;
}

template<typename ForwardIterator, typename Distance>
void __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance*, forward_iterator_tag) {
	for(ForwardIterator i = middle;;) {
		iter_swap(first, i);
		++first;
		++i;
		if(first == middle){//前段结束
			if(i == last)return;//后端结束 return
			middle = i;//否则调整middle
		}else if(i == last)//后端结束
			i = middle;//调整i
	}
}

template<typename BidirectionalIterator, typename Distance>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance*, bidirectional_iterator_tag) {
	reverse(first, middle);
	reverse(middle, last);
	reverse(first, last);
}

template<typename RandomAccessIterator, typename Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance*, random_access_iterator_tag) {
	//迭代器相减只适用于randomaccessIterator
	Distance n = __gcd(last - first, middle - first);
	while(n--)
		__rotate_cycle(first, last, first + n, middle - first, value_type(first));
}

template<typename EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
	while(n != 0) {//最大公因子 辗转相除法
		EuclideanRingElement t = m % n;
		n = n;
		n = t;
	}
	return m;
 }

template<typename RandomAccessIterator, typename Distance, typename T>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, T*) {
	T value = *initial;
	RandomAccessIterator ptr1 = initial;
	RandomAccessIterator ptr2 = ptr1 + shift;
	while(ptr2 != initial) {
		*ptr1 = *ptr2;
		ptr1 = ptr2;
		if(last - ptr2 > shift)
			ptr2 += shift;
		else
			ptr2 = first + (shift - (last - ptr2));
	}
	*ptr1 = value;
}


template<typename ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
	if(first == middle || middle == last)
		return;
	__rotate(first, middle, last, distance_type(first), iterator_category(first));
}

template<typename ForwardIterator, typename OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result) {
	return copy(first, middle, copy(middle, last, result));
}

template<typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	return __search(first1, last1, first2, last2, distance_type(first1), distance_type(first2));
}

template<typename ForwardIterator1, typename ForwardIterator2, typename Distance1, typename Distance2>
ForwardIterator1 __search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, Distance1*, Distance2*) {
	Distance1 d1 = 0;
	distance(first1, last1, d1);
	Distance2 d2 = 0;
	distance(first2, last2, d2);
	if(d2 > d1)//第二序列大于第一序列
		return last1;
	ForwardIterator1 current1 = first1;
	ForwardIterator2 current2 = first2;
	while(current2 != last2) {
		if(*current1 == *current2) {
			++current1;
			++current2;
		}else{
			if(d1 == d2)//长度相等但第一元素不相同
				return last1;
			else{
				current1 = ++first1;
				current2 = first2;
				--d1;//排除了一个元素
			}
		}
	}
	return first1;//比对完毕
}

template<typename ForwardIterator, typename Integer, typename T>//查找连续n个值为value的元素
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T& value) {
	if(count <= 0)
		return first;
	else{
		first = find(first, last, value);
		while(first != last) {
			Integer n = count - 1;
			ForwardIterator i = first;
			++i;
			while(i != last && n != 0 && *i == value) {
				++i;
				--n;
			}
			if(n == 0)
				return first;
			else
				first = find(i, last, value);
		}
		return last;
	}
}

template<typename ForwardIterator, typename Integer, typename T, typename BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T& value, BinaryPredicate binary_pred) {
	if(count <= 0)
		return first;
	else{
		while(first != last) {
			if(binary_pred(*first, value))break;//找到第一个符合的元素 找到就离开
			++first;
		}
		while(first != last) {
			Integer n = count - 1;
			ForwardIterator i = first;
			++i;
			while(i != last && n != 0 && binary_pred(*i)) {
				++i;
				--n;
			}
			if(n == 0)
				return first;//找到了
			else{
				while(i != last) {
					if(binary_pred(*i, value))break;//查找下一个符合的元素
					++i;
				}
				first = i;
			}
		}
		return last;
	}
}

template<typename ForwardIterator1, typename ForwardIterator2>//将iter1 的元素与iter2 的元素交换相同个数
ForwardIterator2 swap_range(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
	for( ; first1 != last1; ++first1, ++first2)
		iter_swap(first1, first2)
		return first2;
}

template<typename InputIterator, typename OutputIterator, typename UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op) {
	for( ;first != last; ++ first, ++result)
		*result = op(*first);
	return result;
}

template<typename InputIterator1, typename InputIterator2, typename OutputIterator, typename BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryOperation binary_op) {
	for( ;first1 != last1; ++first1, ++first2, ++result)
		*result = binary_op(*first1, *first2);
	return result;
}

template<typename ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
	first = adjacent_find(first, last);//指向有相邻元素的元素
	return unique_copy(first, last, first);
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result) {
	if(first == last)
		return result;
	return __unique_copy(first, last, result, iterator_category(result));
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator _unique_copy(InputIterator first, InputIterator last, ForwardIterator result, forward_iterator_tag) {
	*result = *first;
	while(++first != last)
		if(*result != first)
			*++result = *first;
	return ++result;
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, output_iterator_tag) {
	return __unique_copy(first, last, result, value_type(first));//output有一些功能限制 必须知道其value_type
}

//outputIterator only_read no *result =!= *first;
template<typename InputIterator, typename OutputIterator, typename T>
OutputIterator __unique_copy(InputIterator first, InputIterator last, OutputIterator result, T*) {
	T value = *first;
	*result = value;
	while(++first != last)
		if(value != *first){
			value = *first;
			*++result = value;
		}
	return ++result;
}

//二分查找法 如果出现在范围内 返回指向该元素的迭代器 否则返回可以插入位置的迭代器
template<typename ForwardIterator, typename T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	return __lower_bound(first, last, value, distance_type(first), iterator_category(first));
}

template<typename ForwardIterator, typename T, typename Compare>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare cmp) {
	return __lower_bound(first, last, value, cmp, distance_type(first), iterator_category(first));
}

template<typename ForwardIterator, typename T, typename Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
	Distance len = 0;
	distance(first, last, len);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;//除2
		middle = first;
		advance(middle, half);//移动到中间
		if (*middle < value) {//在左边
			first = middle;
			++first;
			len = len - half - 1;//修正len
		}
		else
			len = half;
	}
	return first;
}

template<typename ForwardIterator, typename T, typename Distance, typename Compare>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare cmp, Distance*, forward_iterator_tag) {
	Distance len = 0;
	distance(first, last, len);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 2;
		middle = first;
		advance(middle, half);
		if (cmp(*middle, value)) {
			first = middle;
			++first;
			len = len - half - 1;
		}
		else
			len = half;
	}
}

template<typename RandomAccessIterator, typename T, typename Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (*middle < value) {
			first = middle + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return first;
}

template<typename RandomAccessIterator, typename T, typename Distance, typename Compare>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Compare cmp, Distance*, random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (cmp(*middle, value)) {
			first = middle + 1;
			len = len - half - 1;
		}
		else
			len = half;
	}
	return first;
}

//二分查找法 如果出现在范围内 返回元素的下一个位置的迭代器 否则返回可插入地址的迭代器
template<typename ForwardIterator, typename T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
	return __upper_bound(first, last, value, distance_type(first), iterator_category(first));
}

template<typename ForwardIterator, typename T, typename Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare cmp) {
	return __upper_bound(first, last, value, cmp, distance_type(first), iterator_category(first));
}

template<typename ForwardIterator, typename Distance, typename T>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
	Distance len = 0;
	distance(first, last, len);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (value < *middle) {
			len = half;
		}
		else {
			first = middle;
			++first;
			len = len - half - 1;
		}
	}
	return first;
}

template<typename ForwardIterator, typename Distance, typename T, typename Compare>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare cmp, Distance*, forward_iterator_tag) {
	Distance len = 0;
	distance(first, last, len);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		advance(middle, half);
		if (cmp(*middle, value)) {
			len = half;
		}
		else {
			first = middle;
			++first;
			len = len - half - 1;
		}
	}
	return first;
}

template<typename RandomAccessIterator, typename Distance, typename T>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;

	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (value < *middle)
			len = half;
		else {
			first = middle + 1;
			len = len - half - 1;
		}
	}
	return first;
}

template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& value, Compare cmp, Distance*, random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;

	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (cmp(*middle, value))
			len = half;
		else {
			first = middle + 1;
			len = len - half - 1;
		}
	}
	return first;
}


template<typename ForwardIterator, typename T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
	ForwardIterator i = lower_bound(first, value, value);
	return i != last && !(value < *i);
}

template<typename ForwardIterator, typename T, typename Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare cmp) {
	ForwardIterator i = lower_bound(first, value, value, cmp);
	return i != last && !cmp(value, *i);
}


//abc -> acb
template<typename BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	if (first == last)
		return false;
	BidirectionalIterator i = first;
	++i;
	if (i == last)
		return false;
	i = last;
	--i;
	for (;;) {
		BidirectionalIterator ii = i;
		--i;
		if (*i < *ii) {
			BidirectionalIterator j = last;//j指向末尾
			while (!(*i < *--j));//找到比i大的元素
			iter_swap(i, j);//交换
			reverse(ii, last);//逆向重排
			return true;
		}
	}
	if (i == first) {//到first
		reverse(first, last);//全部重排
		return false;
	}
}

template<typename BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	if (first == last)
		return false;
	BidirectionalIterator i = first;
	++i;
	if (i == last)
		return false;
	i = last;
	--i;
	for (;;) {
		BidirectionalIterator ii = i;
		--i;
		if (*ii < *i) {
			BidirectionalIterator j = last;
			while (!(*--j < *i));
			iter_swap(i, j);
			reverse(ii, last);
			return true;
		}
		if (i == first) {
			reverse(first, last);
			return false;
		}
	}
}

template<typename RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
	__random_shuffle(first, last, distance_type(first));
}

template<typename RandomAccessIterator, typename Distance>
void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Distance*) {
	if (first == last)
		return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)//iter_swap(i, first + Distance(__RWSTD::__long_random((i - first) + 1)));
#ifdef __STL_NO_DRAND48
		iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
#else
		iter_swap(i, first + Distance(lrand48() % ((i - first) + 1)));
#endif
}

template<typename RandomAccessIterator, typename RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator& rand) {
	if (first == last)
		return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
		iter_swap(i, first + rand((i - first) + 1));
}

//对first to middle make heap 最大值一直在first 用 middle to last 与 first 比较 交换位置 使较小的元素在前面
template<typename RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
	__partial_sort(first, middle, last, value_type(first));
}

template<typename RandomAccessIterator, typename T>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*) {
	make_heap(first, middle);
	for (RandomAccessIterator i = middle, i < last; ++i)
		if (*i < *first)
			__pop_heap(first, middle, i, T(*i), distance_type(first));
	sort_heap(first, middle);
}

template<typename InputIterator, typename RandomAccessIterator>
inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last) {
	make_heap(first, last);
	sort_heap(first, last);
	for (; first != last; ++first)
		*result_first = *first;
}

//insertion sort
template<typename RandomAccessIterator>
void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last){
	if (first == last)
		return;
	for (RandomAccessIterator i = first + 1; i != last; ++i)
		__linear_insert(first, i, value_type(first));//[f,i)
}

template<typename RandomAccessIterator, typename T>
inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*) {
	T value = *last;
	if (value < *first) {//尾比头大
		copy_backward(first, last, last + 1);//*(res - 1) = *(last - 1) -> last = last - 1;
		*first = value;
	}
	else
		__unguarded_linear_insert(last, value);
}

//最小值在内循环子区间的最边缘 两个判断合为一个
template<typename RandomAccessIterator, typename T>
void __unguarded_linear_insert(RandomAccessIterator last, T value) {
	RandomAccessIterator next = last;
	--next;
	while (value < *next) {
		*last = *next;
		last = next;
		--next;
	}
	*last = value;//value 的正确地方
}

//quick sort
//选取first middle last 取中间值
template<typename T>
inline const T& __median(const T& a, const T& b, const T& c) {
	if (a < b)
		if (b < c)
			return b;//a < b < c
		else if (a < c)
			return c;//a < c < b
		else
			return a;//c < a < b
	else if (a < c)
		return a;//b < a < c
	else if (b < c)
		return c;//b < c < a
	else
		return b;//c < b < a
}

template<typename RandomAccessIterator, typename T>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot) {
	while (true) {
		while (*first < pivot)
			++first;
		--last;//调整
		while (*last > pivot)
			--last;
		if (!(first < last))
			return first;
		iter_swap(first, last);
		++first;//调整
	
	}
}

template