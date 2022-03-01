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
	// arraySize 함수는 noexcept로 선언하는 것이 compiler가
	// code를 더 좋게 generate하는데 도움을 준다. (Item 14)
	return N;
}

void someFunc(int, double)
{
	// Dummy
}

int main()
{
	/*
	* Item1에서 template type deduction에 대해 읽었다면, 
	* auto에서 발생하는 type deduction의 대부분을 이미 알고 있는 것 입니다.
	* auto는 template에서 T 역할을 하게 되고, 나머지 specifier들은 ParamType으로서 역할을 하게 된다.
	*/

	auto x = 10;
	const auto cx = x;
	const auto& rx = x;

	std::cout << "auto(x)  type is " << Cpg_GetTypeCategory(x) << std::endl;
	std::cout << "auto(cx) type is " << Cpg_GetTypeCategory(cx) << std::endl;
	std::cout << "auto(rx) type is " << Cpg_GetTypeCategory(rx) << std::endl;
	std::cout << std::endl;

	/*
	* auto 또한 취하는 specifier에 따라 아래 3가지 case로 나눌 수 있습니다.
	* Case1: type specifier가 * 또는 &일 때
	* Case2: type specifier가 && 일 때
	* Case3: type specifier가 &, *가 둘다 아닐 때
	*/

	auto y = 10;         // Case 3
	const auto cy = y;   // Case 3
	const auto& ry = y;  // Case 1

	/*
	* Case1, Case3에 대해서는 이미 어떻게 동작하는지 알고 
	*/
	std::cout << "auto(y)  type is " << Cpg_GetTypeCategory(y) << std::endl;
	std::cout << "auto(cy) type is " << Cpg_GetTypeCategory(cy) << std::endl;
	std::cout << "auto(ry) type is " << Cpg_GetTypeCategory(ry) << std::endl;
	std::cout << std::endl;

	/*
	* Case2 : lvalue일 경우 lvalue로 추론한다. (단 이런 상황에서는 reference type으로 추론한다.)
	*         rvalue일 경우 Case1의 규칙에 따른다.
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
	* auto에서 Array와 Function이 pointer로 decay 될 때 type deduction도 동일하다. 
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
	* 우선 아래 4가지 코드는 int type을 10으로 초기화하는 것으로 모두 같다.
	*/

	int x1 = 10;
	int x2(10);
	int x3 = { 10 };
	int x4{ 10 };

	/*
	* 위에서 알아본 template type deduction과 동일한 규칙들 외에 auto만의 예외가 있다.
	* Item5에서는 fixed type를 쓰는 대신 auto를 사용할 때의 이점 대해서 설명한다.
	* 일단은 변수 선언들을 모두 auto로 바꿔보자.
	*/

	auto auto_x1 = 10;
	auto auto_x2(10);
	auto auto_x3 = { 10 };
	auto auto_x4{ 10 };

	std::cout << "auto(auto_x1) type is " << Cpg_GetTypeCategory(auto_x1) << std::endl;
	std::cout << "auto(auto_x2) type is " << Cpg_GetTypeCategory(auto_x2) << std::endl;

	/*
	* Uniform Initialization에 대한 auto의 type deduction 동작은
	* std::initializaer_list<int>에서 int로 변경 되었습니다. (책의 내용과 다릅니다.)
 	* Remark : http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3922.html 
	*/
	std::cout << "auto(auto_x3) type is " << Cpg_GetTypeCategory(auto_x3) << std::endl;
	std::cout << "auto(auto_x4) type is " << Cpg_GetTypeCategory(auto_x4) << std::endl;
	std::cout << std::endl;

	/*
	 * Error Occurred.
	 * x5는 std::initializaer_list<int>로 추론되기 때문에 int type이 
	 * 아닌 3.0을 넣으려고 하면 Compile Error가 발생합니다.	
	 */

	// auto x5 = { 1, 2, 3.0 };

	auto list_x = { 11, 23, 9 };
	val_f(list_x);          // parameter is std::initializer_list<T> -> ParamType is std::initializer_list<int> -> T is std::initializer_list<int>
	init_f(list_x);         // parameter is std::initializer_list<T> -> ParamType is std::initializer_list<int> -> T is int
	//val_f({ 11, 23, 9 }); // Compile Error, can't deduce type for T
	init_f({ 11, 23, 9 });  // parameter is std::initializer_list<T> -> ParamType is std::initializer_list<int> -> T is int

	/*
	* 그렇다면, auto type deduction 시에, 왜 braced initializer들에 대해서만 특별한 규칙을 가질까요?
	* 그것은 알 수는 없지만, 규칙은 규칙이므로 당신이 uniform-initialization을 사용한다면 그 규칙을 명심해야만 합니다.
	*/


	/*
	* C+11의 전통적인 실수는 std::initializer_list를 의도치 않게 선언하는 것 입니다.
	* 몇몇 개발자들이 반드시 넣어야만 하는 상황에서만 Initializers 주변에 괄호를 넣는 이유 입니다.
	* 자세한 것은 Item7에서 다룰 것 입니다.
	*/

	/*
	* C++14에서는 auto가 추론 되어야만 하는 함수의 return type을 가라키는 것을 허락합니다.
	* 또한 C++14의 lambda에서는 auto를 parameter 선언으로 사용합니다.
	* 그렇지만 이러한 auto의 사용은 template type deduction을 일으킵니다. (auto type deduction이 아니라요.)
	* 즉, 아래와 같은 상황에서 std::initializer_list type으로의 추론이 불가능하다는 것 입니다.
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
	* 마찬가지로 이것은 auto가 c++14 이상의 lambda에서 function parameter로 사용 될 때도 동일 합니다.
	* 
	* std::vector<int> v;
	* auto resetV = [&v](const auto& newValue) { v = newValue };
	* resetV({1, 2, 3});
	*/

	return 0;
}