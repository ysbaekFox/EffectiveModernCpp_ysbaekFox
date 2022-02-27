#include <iostream>
#include <typechecker.h>

/*
* Item 1: Understand template type dedution.
* 
*/

template<typename T>
void noRef_f(T param)
{
	std::cout << "T type is " << TypeName<T>::Get() << std::endl;
}

template<typename T>
void ref_f(T& param)
{
	std::cout << "T type is " << TypeName<T>::Get() << std::endl;
}

template<typename T>
void ptr_f(T* param)
{
	std::cout << "T type is " << TypeName<T>::Get() << std::endl;
}

using namespace ysbaek;

int main()
{
	int x = 10;
	const int cx = x;
	const int& rx = x;

	std::cout << "-----------------------" << std::endl;
	std::cout << "No Reference or Pointer" << std::endl;
	std::cout << "-----------------------" << std::endl;
	noRef_f(x);  // ParamType에 &나 *가 없을 경우
	noRef_f(cx); // 전부 const나 &는 T에서 무시 됨.
	noRef_f(rx);
	std::cout << std::endl;
	
	std::cout << "-----------------------" << std::endl;
	std::cout << "ParamType is Reference" << std::endl;
	std::cout << "-----------------------" << std::endl;
	ref_f(x);   //  x is int        -> T is int       -> ParamType is int&
	ref_f(cx);  // cx is const int  -> T is const int -> ParamType is const int&
	ref_f(rx);  // rx is const int& -> T is const int -> ParamType is const int&
				// const가 붙었을 경우 const는 유지 되고, 
				// &는 ParamType에 따라 자연스럽게 추가되거나 제거 됨.
	std::cout << std::endl;

	std::cout << "-----------------------" << std::endl;
	std::cout << "ParamType is Pointer" << std::endl;
	std::cout << "-----------------------" << std::endl;
	const int* px = &x;
	ptr_f(&x); //  x is int        -> T is int        -> ParamType is int*
	ptr_f(px); // px is const int* -> T is const int  -> ParamType is int*
	std::cout << std::endl;

	return 0;
}