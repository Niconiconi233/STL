#include "pch.h"
#include <iostream>
#include<vector>
#include"slist.h"

using namespace std;

template<typename T>
class t {
	friend bool operator==(const t& x, const t& y) { return x.a == y.a; }
	friend ostream& operator<<(ostream& out, t& x) {
		cout << x.a << endl;
		return out;
	}
	typedef t<T> self;
	typedef T value_type;
	typedef T& reference;

public:
	t() :a(T()) {}
	t(const T& x) :a(x) {}
	t(const t& x) :a(x.a) {}
	void swap(const t& x) {
		t tmp = x;
		x = *this;
		*this = tmp;
	}
	self& operator=(t& x) {
		(*this).a = x.a;
		return *this;

	}
	void set(const reference x) { this->a = x; }
	const T get()const { return a; }
private:
	T a;
};

template<typename compare=std::less<int>>
class com {
	typedef compare compare_value;
	compare com;

public:
	int get(int a, int b) {
		return com(a, b) == true ? 1 : 0;
	}

};

bool bigger(int a, int b) {
	return a > b;
}


int main(){



	system("pause");
}
