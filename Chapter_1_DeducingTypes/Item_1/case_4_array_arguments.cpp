#include <iostream>
#include <cpp_types.h>

/*
* Item 1: Understand template type deduction.
* Array Arguments
*/

template<typename T>
void val_f(T param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}


template<typename T>
void ref_f(T& param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
	// arraySize �Լ��� noexcept�� �����ϴ� ���� compiler��
	// code�� �� ���� generate�ϴµ� ������ �ش�. (Item 14)
	return N;
}

int main()
{
	// const char*�� const char[13]�� �ٸ���.
	// �ֳ��ϸ� array-to-pointer���� �߻��ϴ� decay rule ����.
	// decay : � Ÿ���� ���� ��, ���� Ÿ���� ������ �Ҿ� ������ ��.

	const char name[] = "ysbaek";
	const char* ptrToName = name;

	val_f(name); // name is array, but T is deduced to be const char *

	// ����, template parpameter�� reference�� ������ ���
	// ��¥ array type���� paramter�� ������ �� �ִ�.
	ref_f(name);

	// array-to-reference �ÿ��� array�� ũ�⸦ �����Ѵ�.
	// �� ����� �̿��ϸ� array�� ���Ե� element�� ������ �߷��ϴ� ���ø��� ���� �� �ִ�.
	std::cout << "name array's size is " << arraySize(name) << std::endl;
	std::cout << std::endl;

	int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };
	int mappedVals[arraySize(keyVals)];

	ref_f(keyVals);
	std::cout << "keyVals array's size is " << arraySize(keyVals) << std::endl;
	std::cout << "mappedVals array's size is " << arraySize(mappedVals) << std::endl;

	return 0;
}