#include <iostream>
#include <cpp_types.h>
#include <initializer_list>

/*
* Item 2: Understand auto type deduction.
*/

template<typename T>
void val_f(T param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

template<typename T>
void init_f(std::initializer_list<T> initList)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept
{
	// arraySize �Լ��� noexcept�� �����ϴ� ���� compiler��
	// code�� �� ���� generate�ϴµ� ������ �ش�. (Item 14)
	return N;
}

void someFunc(int, double)
{
	// Dummy
}

int main()
{
	/*
	* Item1���� template type deduction�� ���� �о��ٸ�, 
	* auto���� �߻��ϴ� type deduction�� ��κ��� �̹� �˰� �ִ� �� �Դϴ�.
	* auto�� template���� T ������ �ϰ� �ǰ�, ������ specifier���� ParamType���μ� ������ �ϰ� �ȴ�.
	*/

	auto x = 10;
	const auto cx = x;
	const auto& rx = x;

	std::cout << "auto(x)  type is " << Cpg_GetTypeCategory(x) << std::endl;
	std::cout << "auto(cx) type is " << Cpg_GetTypeCategory(cx) << std::endl;
	std::cout << "auto(rx) type is " << Cpg_GetTypeCategory(rx) << std::endl;
	std::cout << std::endl;

	/*
	* auto ���� ���ϴ� specifier�� ���� �Ʒ� 3���� case�� ���� �� �ֽ��ϴ�.
	* Case1: type specifier�� * �Ǵ� &�� ��
	* Case2: type specifier�� && �� ��
	* Case3: type specifier�� &, *�� �Ѵ� �ƴ� ��
	*/

	auto y = 10;         // Case 3
	const auto cy = y;   // Case 3
	const auto& ry = y;  // Case 1

	/*
	* Case1, Case3�� ���ؼ��� �̹� ��� �����ϴ��� �˰� 
	*/
	std::cout << "auto(y)  type is " << Cpg_GetTypeCategory(y) << std::endl;
	std::cout << "auto(cy) type is " << Cpg_GetTypeCategory(cy) << std::endl;
	std::cout << "auto(ry) type is " << Cpg_GetTypeCategory(ry) << std::endl;
	std::cout << std::endl;

	/*
	* Case2 : lvalue�� ��� lvalue�� �߷��Ѵ�. (�� �̷� ��Ȳ������ reference type���� �߷��Ѵ�.)
	*         rvalue�� ��� Case1�� ��Ģ�� ������.
	*/

	auto&& uref_y = y;    // -> uref_y  is lvalue        -> auto is int&        -> ParamType is int&&&          -> so int&
	auto&& uref_cy = cy;  // -> uref_cy is lvalue, const -> auto is const int&  -> ParamType is const int&&&    -> so const int&
	auto&& uref_ry = ry;  // -> uref_ry is lvalue, const -> auto is const int&  -> ParamType is const int&&&    -> so const int&
	auto&& uref_cn = 27;  // -> uref_cn is rvalue        -> auto is int         -> ParamType is int&&           -> so int&&

	std::cout << "auto(y)  type is " << Cpg_GetTypeCategory(uref_y)  << std::endl;
	std::cout << "auto(cy) type is " << Cpg_GetTypeCategory(uref_cy) << std::endl;
	std::cout << "auto(ry) type is " << Cpg_GetTypeCategory(uref_ry) << std::endl;
	std::cout << "auto(ry) type is " << Cpg_GetTypeCategory(uref_cn) << std::endl;
	std::cout << std::endl;

	/*
	* auto���� Array�� Function�� pointer�� decay �� �� type deduction�� �����ϴ�. 
	*/

	const char name[] = "ysbaek";
	auto arr1 = name;  // const char*
	auto& arr2 = name; // const char(&)[7]

	std::cout << "auto(arr1) type is " << Cpg_GetTypeCategory(arr1) << std::endl;
	std::cout << "auto(arr2) type is " << Cpg_GetTypeCategory(arr2) << std::endl;
	std::cout << "auto(arr2) size is " << arraySize(arr2) << std::endl << std::endl;

	// someFunc is void(int, double)
	auto func1 = someFunc;  // void(*)(int, double)
	auto& func2 = someFunc; // void(&)(int, double)

	std::cout << "auto(func1) type is " << Cpg_GetTypeCategory(func1) << std::endl;
	std::cout << "auto(func2) type is " << Cpg_GetTypeCategory(func2) << std::endl;
	std::cout << std::endl;

	/*
	* �켱 �Ʒ� 4���� �ڵ�� int type�� 10���� �ʱ�ȭ�ϴ� ������ ��� ����.
	*/

	int x1 = 10;
	int x2(10);
	int x3 = { 10 };
	int x4{ 10 };

	/*
	* ������ �˾ƺ� template type deduction�� ������ ��Ģ�� �ܿ� auto���� ���ܰ� �ִ�.
	* Item5������ fixed type�� ���� ��� auto�� ����� ���� ���� ���ؼ� �����Ѵ�.
	* �ϴ��� ���� ������� ��� auto�� �ٲ㺸��.
	*/

	auto auto_x1 = 10;
	auto auto_x2(10);
	auto auto_x3 = { 10 };
	auto auto_x4{ 10 };

	std::cout << "auto(auto_x1) type is " << Cpg_GetTypeCategory(auto_x1) << std::endl;
	std::cout << "auto(auto_x2) type is " << Cpg_GetTypeCategory(auto_x2) << std::endl;

	/*
	* Uniform Initialization�� ���� auto�� type deduction ������
	* std::initializaer_list<int>���� int�� ���� �Ǿ����ϴ�. (å�� ����� �ٸ��ϴ�.)
 	* Remark : http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3922.html 
	*/
	std::cout << "auto(auto_x3) type is " << Cpg_GetTypeCategory(auto_x3) << std::endl;
	std::cout << "auto(auto_x4) type is " << Cpg_GetTypeCategory(auto_x4) << std::endl;
	std::cout << std::endl;

	/*
	 * Error Occurred.
	 * x5�� std::initializaer_list<int>�� �߷еǱ� ������ int type�� 
	 * �ƴ� 3.0�� �������� �ϸ� Compile Error�� �߻��մϴ�.	
	 */

	// auto x5 = { 1, 2, 3.0 };

	auto list_x = { 11, 23, 9 };
	val_f(list_x);          // parameter is std::initializer_list<T> -> ParamType is std::initializer_list<int> -> T is std::initializer_list<int>
	init_f(list_x);         // parameter is std::initializer_list<T> -> ParamType is std::initializer_list<int> -> T is int
	//val_f({ 11, 23, 9 }); // Compile Error, can't deduce type for T
	init_f({ 11, 23, 9 });  // parameter is std::initializer_list<T> -> ParamType is std::initializer_list<int> -> T is int

	/*
	* �׷��ٸ�, auto type deduction �ÿ�, �� braced initializer�鿡 ���ؼ��� Ư���� ��Ģ�� �������?
	* �װ��� �� ���� ������, ��Ģ�� ��Ģ�̹Ƿ� ����� uniform-initialization�� ����Ѵٸ� �� ��Ģ�� ����ؾ߸� �մϴ�.
	*/


	/*
	* C+11�� �������� �Ǽ��� std::initializer_list�� �ǵ�ġ �ʰ� �����ϴ� �� �Դϴ�.
	* ��� �����ڵ��� �ݵ�� �־�߸� �ϴ� ��Ȳ������ Initializers �ֺ��� ��ȣ�� �ִ� ���� �Դϴ�.
	* �ڼ��� ���� Item7���� �ٷ� �� �Դϴ�.
	*/

	/*
	* C++14������ auto�� �߷� �Ǿ�߸� �ϴ� �Լ��� return type�� ����Ű�� ���� ����մϴ�.
	* ���� C++14�� lambda������ auto�� parameter �������� ����մϴ�.
	* �׷����� �̷��� auto�� ����� template type deduction�� ����ŵ�ϴ�. (auto type deduction�� �ƴ϶��.)
	* ��, �Ʒ��� ���� ��Ȳ���� std::initializer_list type������ �߷��� �Ұ����ϴٴ� �� �Դϴ�.
	* 
	* std::initializer_list<int> createInitList()
	* {
	*	return { 1, 2, 3 };
	* } // OK
	* 
	* auto createInitList()
	* {
	*	return { 1, 2, 3 };
	* } // Not OK
	* 
	* ���������� �̰��� auto�� c++14 �̻��� lambda���� function parameter�� ��� �� ���� ���� �մϴ�.
	* 
	* std::vector<int> v;
	* auto resetV = [&v](const auto& newValue) { v = newValue };
	* resetV({1, 2, 3});
	*/

	return 0;
}