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
	// 1. ����ó�� argument�� reference�� ���, reference part�� �����Ѵ�.
	// 2. �׷��� ���� arguement�� reference�� �����ߴµ�, �װ��� const�̸� const�� �����Ѵ�. 

	// volatile�� �Ϲ������� �ʽ��ϴ�.
	// volatile�̸� volatile�� �����մϴ�. 
	// (volatile�� device driver���� �Ϲ������� ���Ǹ� Item 40���� �ڼ��ϰ� �ٷ� �� �Դϴ�.)

	int x = 10;
	const int cx = x;
	const int& rx = x;

	value_f(x);
	value_f(cx);
	value_f(rx);

	// cosnt�� volatile�� value paramter�� ���� ���õȴٴ� ���� �߿��ϴ�.
	// reference�� point�� paramter�� �θ� �� type dedution ���� const�� ����ȴ�.
	// ������ value parameter���� const point to const object�� value paramter�� �ѱ� ���� ����ؾ� �Ѵ�.
	// (const point to const object�� ����� ����Ű�� ��� �����͸� �ǹ���)

	// int value = 5;
	// ����� ����Ű�� ������ (Pointer to const value) ex) const int* ptr = &value;
	// ��� ������ (Const pointer) ex) int* const ptr = int* const ptr = &value1;
	// ����� ����Ű�� ��� ������ ex) const int* const ptr = &value;

	const char* const ptr = "Fun with pointers";
	value_f(ptr);

	return 0;
}