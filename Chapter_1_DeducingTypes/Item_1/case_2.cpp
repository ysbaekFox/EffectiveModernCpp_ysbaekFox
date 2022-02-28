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
	* �ڼ��� ������ Item24���� �ٷ� ���̰�, ���⼭ �߿��� ����
	* Universal Reference�� ���, lvalue argument / rvalue argument�Ŀ� ���� ������ ���еȴ�.
	* 
	* 1. ParamTyp�� T&&�̸鼭 argument�� lvalue�� ���, T�� ParamType ��� lvalue�� �߷��Ѵ�.
	* �̷� ��Ȳ���� �Ͼ�� Ÿ�� �߷��� ��� T�� reference�� �ȴ�. �׸��� ParamType�� rvalue syntax�� ����ϰ� �ִ���
	* lvalue reference�� Ÿ�� �߷� �ȴ�.
	*  
	* 2.ParamTyp�� T&& argument�� rvalue�� ���, case1�� ��Ģ�� ������.
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