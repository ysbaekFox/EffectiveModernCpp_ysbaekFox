#include <iostream>
#include <cpp_types.h>

/*
* Item 1: Understand template type dedution.
* Case 1: ParamType is Universal Reference.
*/

template<typename T>
void uniRef(T&& param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

int main()
{
	/*
	* 자세한 내용은 Item24에서 다룰 것이고, 여기서 중요한 것은
	* Universal Reference의 경우, lvalue argument / rvalue argument냐에 따라 동작이 구분된다.
	* 
	* 1. ParamTyp이 T&&이면서 argument가 lvalue일 경우, T와 ParamType 모두 lvalue로 추론한다.
	* 이런 상황에서 일어나는 타입 추론의 경우 T는 reference가 된다. 그리고 ParamType이 rvalue syntax를 사용하고 있더라도
	* lvalue reference로 타입 추론 된다.
	*  
	* 2.ParamTyp이 T&& argument가 rvalue일 경우, case1의 규칙을 따른다.
	*/

	int x = 10;
	const int cx = x;
	const int& rx = x;

	uniRef(x);  // -> x is lvalue -> T is int&        -> ParamType is T&&&          -> so T&
	uniRef(cx); // -> x is lvalue -> T is const int&  -> ParamType is const int &&& -> so const int&
	uniRef(rx); // -> x is lvalue -> T is const int&  -> ParamType is const int &&& -> so const int&
	uniRef(10); // -> x is rvalue -> T is int         -> ParamType is int&&         -> so int&&
	
	return 0;
}