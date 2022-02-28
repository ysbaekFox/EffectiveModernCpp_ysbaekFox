#include <iostream>
#include <cpp_types.h>

/*
* Item 1: Understand template type dedution.
* Case 3: ParamType is Neither a Pointer nor a Reference.
*/

template<typename T>
void value_f(T param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

int main()
{
	// 1. 이전처럼 argument가 reference일 경우, reference part를 무시한다.
	// 2. 그런데 만약 arguement의 reference를 무시했는데, 그것이 const이면 const도 무시한다. 

	// volatile은 일반적이지 않습니다.
	// volatile이면 volatile을 무시합니다. 
	// (volatile은 device driver에서 일반적으로 사용되며 Item 40에서 자세하게 다룰 것 입니다.)

	int x = 10;
	const int cx = x;
	const int& rx = x;

	value_f(x);
	value_f(cx);
	value_f(rx);

	// cosnt와 volatile은 value paramter일 때만 무시된다는 점이 중요하다.
	// reference나 point로 paramter를 부를 때 type dedution 동안 const가 보장된다.
	// 하지만 value parameter에서 const point to const object를 value paramter로 넘길 때를 고려해야 한다.
	// (const point to const object는 상수를 가리키는 상수 포인터를 의미함)

	// int value = 5;
	// 상수를 가리키는 포인터 (Pointer to const value) ex) const int* ptr = &value;
	// 상수 포인터 (Const pointer) ex) int* const ptr = int* const ptr = &value1;
	// 상수를 가리키는 상수 포인터 ex) const int* const ptr = &value;

	const char* const ptr = "Fun with pointers";
	value_f(ptr);

	return 0;
}