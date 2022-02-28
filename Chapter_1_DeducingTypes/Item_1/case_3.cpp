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
	// 1. ���ø��� parameter�� value parameter�̰�
	// 2. argument�� reference�� ���, reference part�� �����Ѵ�.
	// 3. �׷��� ���� arguement�� reference�� �����ߴµ�, �װ��� const�̸� const�� �����Ѵ�. 

	// volatile�� �Ϲ������� �ʽ��ϴ�.
	// value parameter�� ��, volatile�̸� volatile�� �����մϴ�. 
	// (volatile�� device driver���� �Ϲ������� ���Ǹ� Item 40���� �ڼ��ϰ� �ٷ� �� �Դϴ�.)

	int x = 10;
	const int cx = x;
	const int& rx = x;

	value_f(x);  // -> x is int       -> T is int -> ParamType is int
	value_f(cx); // -> x is const int -> T is int -> ParamType is int
	value_f(rx); // -> x is const in& -> T is int -> ParamType is int

	// cosnt�� volatile�� value paramter�� ���� ���õȴٴ� ���� �߿��ϴ�.
	// case1, case2���� reference�� point�� paramter�� �θ� �� type dedution ���� const�� ����ȴ�.
	// ������ value parameter���� const point to const object�� value paramter�� �ѱ� ���� ����ؾ� �Ѵ�.
	// (const point to const object�� ����� ����Ű�� ��� �����͸� �ǹ���)

	// int value = 5;
	// ����� ����Ű�� ������ (Pointer to const value) ex) const int* ptr = &value;
	// ��� ������ (Const pointer) ex) int* const ptr = int* const ptr = &value1;
	// ����� ����Ű�� ��� ������ ex) const int* const ptr = &value;

	const char* const ptr = "Fun with pointers";
	value_f(ptr); // -> x is const char* const -> T is const char* -> ParamType is const char*

	return 0;
}