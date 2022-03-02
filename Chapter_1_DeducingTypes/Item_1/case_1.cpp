#include <iostream>
#include <cpp_types.h>

/*
* Item 1: Understand template type deduction.
* Case 1: ParamType is a Reference or Pointer,
*         but not a Universal Reference.
*/

template<typename T>
void noRef_f(T param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

template<typename T>
void ref_f(T& param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

template<typename T>
void ptr_f(T* param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

int main()
{
	int x = 10;
	const int cx = x; // &�� ������ ������ reference�� �������� ����.
	const int& rx = x;

	std::cout << "-----------------------" << std::endl;
	std::cout << "No Reference or Pointer" << std::endl;
	std::cout << "-----------------------" << std::endl;
	noRef_f(x);  // ParamType�� &�� *�� ���� ���
	noRef_f(cx); // const�� &�� T���� ���� ���� ��.
	noRef_f(rx);
	std::cout << std::endl;
	
	std::cout << "-----------------------" << std::endl;
	std::cout << "ParamType is Reference" << std::endl;
	std::cout << "-----------------------" << std::endl;
	ref_f(x);   //  x is int        -> T is int       -> ParamType is int&
	ref_f(cx);  // cx is const int  -> T is const int -> ParamType is const int&
	ref_f(rx);  // rx is const int& -> T is const int -> ParamType is const int&
				// const�� �پ��� ��� const�� ���� �ǰ�, 
				// &�� ParamType�� ���� �ڿ������� �߰��ǰų� ���� ��.
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