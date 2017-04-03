/******************************************************************************

This file is placed in the public domain.

LH_Mouse
2015-10-06

******************************************************************************/

#include "intrusive_ptr.hpp"
#include <cstdio>

struct foo : std::intrusive_base<foo> {
	foo(int, const char *);
	virtual ~foo();
};

struct bar : foo {
	bar()
		: foo(1, "hello")
	{
	}
};

template class std::intrusive_base<foo>;
template class std::intrusive_ptr<foo>;
template class std::intrusive_weak_ptr<foo>;

std::intrusive_ptr<foo> gp;

foo::foo(int i, const char *s){
	std::printf("- foo::foo(%d, %s)\n", i, s);

	gp = shared_from_this();
}
foo::~foo(){
	std::printf("- foo::~foo()\n");
}

int main(){
	std::intrusive_ptr<bar> p;
	std::intrusive_weak_ptr<foo> wp;

	p = std::make_intrusive<bar>();
	wp = p;
	std::printf("p = %p, gp = %p, wp.lock() = %p\n", (void *)p.get(), (void *)gp.get(), (void *)wp.lock().get());

	p.reset();
	std::printf("p = %p, gp = %p, wp.lock() = %p\n", (void *)p.get(), (void *)gp.get(), (void *)wp.lock().get());

	gp.reset();
	std::printf("p = %p, gp = %p, wp.lock() = %p\n", (void *)p.get(), (void *)gp.get(), (void *)wp.lock().get());
}

/*

$ g++ test_intrusive_ptr.cpp -std=c++11
$ ./a.out
- foo::foo(1, hello)
p = 0000000000487980, gp = 0000000000487980, wp.lock() = 0000000000487980
p = 0000000000000000, gp = 0000000000487980, wp.lock() = 0000000000487980
- foo::~foo()
p = 0000000000000000, gp = 0000000000000000, wp.lock() = 0000000000000000
$ 

*/
