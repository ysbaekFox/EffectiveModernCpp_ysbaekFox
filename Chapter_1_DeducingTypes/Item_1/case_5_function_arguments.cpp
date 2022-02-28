#include <iostream>
#include <cpp_types.h>

/*
* Item 1: Understand template type dedution.
* Function Arguments
*/

template<typename T>
void ref_f(T& param)
{
	std::cout << Cpg_GetTypeName(T) << std::endl;
}

template<typename T>
void ptr_f(T* param)
{
	std::cout << Cpg_GetTypeName(T) << std::endl;
}

void someFunc(int, double)
{ }

int main()
{
	ref_f(someFunc); // x is void(int,double) -> T is void(int,double) -> ParamType is void(&)(int,double)
	ptr_f(someFunc); // x is void(int,double) -> T is void(int,double) -> ParamType is void(*)(int,double)

	return 0;
}