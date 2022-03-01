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
	// arraySize 함수는 noexcept로 선언하는 것이 compiler가
	// code를 더 좋게 generate하는데 도움을 준다. (Item 14)
	return N;
}

int main()
{
	// const char*와 const char[13]은 다르다.
	// 왜냐하면 array-to-pointer에서 발생하는 decay rule 때문.
	// decay : 어떤 타입이 변할 때, 본래 타입의 정보를 잃어 버리는 것.

	const char name[] = "ysbaek";
	const char* ptrToName = name;

	val_f(name); // name is array, but T is deduced to be const char *

	// 만약, template parpameter를 reference로 변경할 경우
	// 진짜 array type으로 paramter를 선언할 수 있다.
	ref_f(name);

	// array-to-reference 시에는 array의 크기를 포함한다.
	// 이 기술을 이용하면 array에 포함된 element의 개수를 추론하는 템플릿을 만들 수 있다.
	std::cout << "name array's size is " << arraySize(name) << std::endl;
	std::cout << std::endl;

	int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };
	int mappedVals[arraySize(keyVals)];

	ref_f(keyVals);
	std::cout << "keyVals array's size is " << arraySize(keyVals) << std::endl;
	std::cout << "mappedVals array's size is " << arraySize(mappedVals) << std::endl;

	return 0;
}