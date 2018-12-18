#include"iterator_traits.h"
#include"alloc.h"
#include"construct.h"

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base {//节点类型
	typedef __rb_tree_color_type color_type;
	typedef __rb_tree_node_base* base_ptr;

	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	static base_ptr minimum(base_ptr x) {
		while (x->right) {
			x = x->right;
		}
		return x;
	}

	static base_ptr maximum(base_ptr x) {
		while (x->left) {
			x = x->left;
		}
		return x;
	}
};

template<typename value>
struct __rb_tree_node :public __rb_tree_node_base {
	typedef __rb_tree_node<value>* link_type;
	value value_field;
};

struct __rb_tree_base_iterator {
	typedef __rb_tree_node_base::base_ptr base_ptr;
	typedef bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	base_ptr node;//ptr

	void increment() {
		if (node->right != 0) {
			node = node->right;//向根节点的右边
			while (node->left != 0)
				node = node->left;//一直向左子树走
		}
		else {//没有右节点
			base_ptr y = node->parent;
			while (node == y->right) {//如果当前节点是右节点
				node = y;
				y = y->parent;//上行
			}
			if (node->right != y)//右子节点不等于此时的父节点
				node = y;
		}
	}

	void decrement() {
		if (node->color == __rb_tree_red && node->parent->parent == node) {
			node = node->right;
		}
		else if (node->left != 0) {
			base_ptr y = node->left;
			while (y->right != 0) {
				y = y->right;
			}
			node = y;
		}
		else {
			base_ptr y = node->parent;
			while (node == y->left) {
				node = y;
				y = y->parent;
			}
			node = y;
		}
	}
};

template<typename value,typename Ref,typename Ptr>
struct __rb_tree_iterator :public __rb_tree_base_iterator {
	typedef value value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef __rb_tree_iterator<value_type, value_type&, value_type*> iterator;
	typedef __rb_tree_iterator<value_type, const value_type&, const value_type*> const_iterator;
	typedef __rb_tree_iterator<value, Ref, Ptr> self;
	typedef __rb_tree_node<value>* link_type;

	__rb_tree_iterator() {}
	__rb_tree_iterator(link_type x) { node = x; }
	__rb_tree_iterator(const iterator& x) { node = x.node; }

	reference operator*()const { return link_type(node)->value_field; }
	pointer operator->()const { return &(operator*()); }

	self& operator++() { increment(); return *this; }
	self operator++(int) {
		self tmp = *this;
		increment();
		returm tmp;
	}
	self& operator--() { decrement(); return *this; }
	self operator--(int) {
		self tmp = *this;
		decrement();
		return tmp;
	}
};

inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	__rb_tree_node_base* y = x->right;//旋转点右节点
	x->right = y->left;
	if (y->left != 0)
		y->left->parent = x;
	y->parent = x->parent;
	if (x == root)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
	__rb_tree_node_base* y = x->left;
	x->left = y->right;
	if (y->right != 0)
		y->right->parent = x;
	y->parent = x->parent;
	if (x == root)
		root = y;
	else if (x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	y->right = x;
	x->parent = y;
}

inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {//* 在函数内不可以改变指针的值 *& 可以改变指针的值
	x->color = __rb_tree_red;//新节点比为红
	while (x != root && x->parent->color == __rb_tree_red) {//父节点为红
		if (x->parent == x->parent->parent->left) {//父节点为左节点
			__rb_tree_node_base* y = x->parent->parent->right;//伯父节点
			if (y&&y->color == __rb_tree_red) {//伯父为红色
				x->parent->color = __rb_tree_black;//fu->black
				y->color = __rb_tree_black;//bofu->black
				x->parent->parent->color = __rb_tree_red;//zufu->red
				x = x->parent->parent;//
			}
			else {//无伯父节点
				if (x == x->parent->right) {//新节点在右边
					x = x->parent;
					__rb_tree_rotate_right(x->parent->parent, root);
				}
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_right(x->parent->parent, root);
			}
		}
		else {
			__rb_tree_node_base* y = x->parent->parent->left;
			if (y&&y->color == __rb_tree_red) {
				x->parent->color = __rb_tree_black;
				y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				x = x->parent->parent;
			}
			else {
				if (x == x->parent->left) {
					x = x->parent;
					__rb_tree_rotate_right(x, root);
				}
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_left(x->parent->parent, root);
			}
		}
	}
	root->color = __rb_tree_black;
}

template<typename key,typename value,typename keyofvalue,typename compare,typename Alloc=alloc>
class rb_tree {
protected:
	typedef void* void_pointer;
	typedef __rb_tree_node_base* base_ptr;
	typedef __rb_tree_node<value> rb_tree_node;
	typedef simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	typedef __rb_tree_color_type color_type;

public:
	typedef key key_type;
	typedef value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef rb_tree_node* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected:
	link_type get_node() { return rb_tree_node_allocator::allocate(); }
	void put_node(link_type x) { rb_tree_node_allocator::deallocate(x); }

	link_type create_node(const value_type& x) {
		link_type tmp = get_node();
		construct(&tmp->value_field, x);
		return tmp;
	}

	link_type clone_node(link_type x) {
		link_type tmp = create_node(x->value_field);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}

	void destroy_node(link_type x) {
		destroy(&x->value_field);
		put_node(x);
	}

protected:
	size_type node_count;
	link_type header;
	compare key_compare;

	link_type& root()const { return (link_type)header->parent; }
	link_type& leftmost()const { return (link_type)header->left; }
	link_type& rightmost()const { return (link_type)header->right; }

	static link_type& left(link_type x) { return (link_type&)(x->left); }
	static link_type& right(link_type x) { return (link_type&)(x->right); }
	static link_type& parent(link_type x) { return (link_type&)(x->parent); }
	static reference value(link_type x) { return x->value_field; }
	static const key& key(link_type x) { return keyofvalue()(value(x)); }
	static color_type& color(link_type x) { return (color_type&)(x->color); }

	static link_type& left(base_ptr x) { return (link_type&)(x->left); }
	static link_type& right(base_ptr x) { return (link_type&)(x->right); }
	static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
	static reference value(base_ptr x) { return (link_type(x))->value_field; }
	static const key& key(base_ptr) { return keyofvalue()(value(link_type(x))); }
	static color& color(base_ptr x) { return (color_type&)(link_type(x))->color; }

	static link_type minimun(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
	static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }

public:
	typedef __rb_tree_iterator<value_type, value_type&, value_type*> iterator;
	typedef __rb_tree_iterator<value_type, const value_type&, const value_type*> const_iterator;
private:
	iterator __insert(base_ptr x_, base_ptr y_, const value_type& v);
	link_type __copy(link_type x, link_type p);
	void __erase(link_type x);
	void init() {
		header = get_node();
		color(header) = __rb_tree_red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}

public:
	rb_tree(const compare& com = compare()) :node_count(0), key_compare(com) { init(); }
	~rb_tree() {
		clear();
		put_node(header);
	}
	rb_tree<key, value, keyofvalue, compare, Alloc>&
		operator=(const rb_tree<key, value, keyofvalue, compare, Alloc>& x);

public:
	compare key_comp()const { return key_compare; }
	iterator begin() { return leftmost(); }
	iterator end() { return header; }
	bool empty()const { return node_count == 0; }
	size_type size()const { return node_count; }
	size_type max_size()const { return size_type(-1); }

	std::pair<iterator, bool> insert_unique(const value_type& v);
	iterator insert_equal(const value_type& v);
	iterator find(const value_type& k);
};

template<typename key, typename value, typename keyofvalue, typename compare, typename Alloc/*=alloc*/>
typename rb_tree<key,value,keyofvalue,compare,Alloc>::iterator
rb_tree<key, value, keyofvalue, compare, Alloc>::find(const value_type& k)
{
	link_type y = header;
	link_type x = root();
	while (x != 0) {
		if (!key_compare(key(x), k))
			y = x, x = left(x);
		else
			x = right(x);
	}
	iterator j = iterator(y);
	return (j == end()) || key_compare(k, key(j.node))) ? end() : j;
}

template<typename key, typename value, typename keyofvalue, typename compare, typename Alloc/*=alloc*/>
typename rb_tree<key,value,keyofvalue,compare,Alloc>::iterator 
rb_tree<key, value, keyofvalue, compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v)
{
	link_type x = (link_type)x_;
	link_type y = (link_type)y_;
	link_type z;

	if (y == header || x != 0 || key_compare(keyofvalue()(v), key(y))) {//x 插入节点 y 父节点
		z = create_node(v);
		left(y) = z;
		if (y == header) {//如果没有节点
			root() = z;
			rightmost() = z;
		}
		else if (y == leftmost())//调整leftmost
			leftmost() = z;
	}
	else {
		z = create_node(v);
		right(y) = z;
		if (y == rightmost())
			rightmost() = z;
	}
	parent(z) = y;
	left(z) = 0;
	right(z) = 0;
	__rb_tree_rebalance(z, header->parent);
	++node_count;
	return iterator(z);

}

template<typename key, typename value, typename keyofvalue, typename compare, typename Alloc/*=alloc*/>
typename rb_tree<key,value,keyofvalue,compare,Alloc>::iterator
rb_tree<key, value, keyofvalue, compare, Alloc>::insert_equal(const value_type& v) {
	link_type y = header;
	link_type x = root();
	while (x != ) {
		y = x;
		x = key_compare(keyofvalue()(v), key(x)) ? left(x) : right(x);

	}
	return __insert(x, y, v);
}

template<typename key, typename value, typename keyofvalue, typename compare, typename Alloc/*=alloc*/>
std::pair<typename rb_tree<key,value,keyofvalue,compare,Alloc>::iterator,bool>
rb_tree<key, value, keyofvalue, compare, Alloc>::insert_unique(const value_type& v) {
	link_type y = header;
	link_type x = root();
	bool comp = true;
	while (x！ = 0) {//寻找插入位置
		y = x;
		comp = key_compare(keyofvalue()(v), key(x));
		x = comp ? left(x) : right(x);
	}

	iterator j = iterator(y);//父节点
	if (comp)//如果为真 插入左边
		if (j == begin())//如果插入位置为最左边
			return std::pair<iterator, bool>(__insert(x, y, v), true);
		else
			--j;
	if (key_compare(key(j.node), keyofvalue()(v)))//不重复
		return std::pair<iterator, bool>(__insert(x, y, v), true);
	return std::pair<iterator, bool>(j, false);//重复 不插入
}