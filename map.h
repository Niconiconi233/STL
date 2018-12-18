#include"rb_tree.h"
#include"alloc.h"

template<typename T1,typename T2>
struct pair {
	T1 first;
	T2 second;
	pair() :first(T1()), second(T2()) {}
	pair(const T1& a, const T2& b) :first(a), second(b) {}
};
/*
	通过改变compare 来改变 排序方法
	Forexample:
	template<typename T = void>
	struct bigger {
	bool operator()(const T& rhs, const T& lhs) {
		return (rhs > lhs);
	}
};
*/


template<typename key,typename T,typename compare=std::less<key>,typename Alloc = alloc>
class map {
public:
	typedef key key_type;
	typedef T	data_type;
	typedef T	mapped_type;
	typedef pair<key_type, data_type> value_type;
	typedef compare key_compare;

	class value_compare :public binary_function<value_type,value_type,bool> {
		friend class map<key, T, compare, Alloc>;
	protected:
		compare comp;
		value_compare(compare c) :comp(c) {}
	public:
		bool operator()(const value_type& x, const value_type& y)const {
			return comp(x.first, y.first);
		}

	};
private:
	typedef rb_tree<key_type, value_type, selectlst<value_type>, key_compare, Alloc> rep_type;
	rep_type t;
public:
	typedef typename rep_type::pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::reverse_iterator reverse_iterator;
	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	map() :t(compare()) {}
	explicit map(const compare& comp) :t(comp) {}
	template<typename InputIterator>
	map(InputIterator first, InputIterator last) : t(compare()) { t.insert_unique(first, last); }
	template<typename InputIterator>
	map(InputIterator first, InputIterator last, const compare& comp) : t(comp) { t.insert_unique(first, last); }
	map(const map<key, T, compare, Alloc>& x) :t(x.t) {}
	map<key, T, compare.Alloc>& operator=(const map<key, T, compare, Alloc>& x) {
		t = x.t;
		return *this;
	}
	/*
	same as set
	
	*/

	T& operator[](const key_type& k) {
		return (*((insert(value_type(k, T()))).first)).second;//insert 放回一个pair<iterator,bool> *iterator is value_type, value_type is pair<key,value>
	}
	/*
		same as set
	*/



};

