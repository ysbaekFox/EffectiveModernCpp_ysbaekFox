#include <iostream>
#include <vector>
#include <cpp_types.h>
#include <deque>

struct Widget
{ };

bool f(const Widget& w)
{
	Widget b;
	decltype(w) test = b;
	std::cout << "j type is " << Cpg_GetTypeCategory(test) << std::endl;

	return true;
}

//template<typename Container, typename Index>
//auto authAndAccess(Container& c, Index i)
//{
//	// Do Something
//	return c[i];
//}

//template<typename Container, typename Index>
//auto authAndAccess(Container& c, Index i)
// -> decltype(c[i])
//{
//	// Do Something
//	return c[i];
//}

//template<typename Container, typename Index>
//decltype(auto) authAndAccess(Container& c, Index i) 
//{
//	// Do Something
//	return c[i];
//}

template<typename Container, typename Index>
decltype(auto) authAndAccess(Container&& c, Index i)
{
	// Do Something
	
	// return c[i]; 
	return std::forward<Container>(c)[i];
}

std::deque<int> makeStringDeque()
{
	std::deque<int> dummy;
	dummy.push_back(10);
	return dummy;
}

int main()
{
	/*
	* decltype은 거의 대부분의 상황에서 
	* 항상 뒤에 오는 name 또는 expression과 정확하게 동일한 것으로 추론된다.
	*/

	std::cout << "---------------------------" << std::endl;
	std::cout << "* decltype test" << std::endl;
	std::cout << "---------------------------" << std::endl;
	
	const int i = 0;
	decltype(i) j = i;

	std::cout << "j type is " << Cpg_GetTypeCategory(i) << std::endl;

	Widget widget;
	f(widget);
	std::cout << "f type is " << Cpg_GetTypeCategory(f) << std::endl;

	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "---------------------------" << std::endl;
	std::cout << "* operator[] Test" << std::endl;
	std::cout << "---------------------------" << std::endl;

	std::deque<int> deque;
	deque.push_back(0);

	int a = 0;
	decltype(deque[0]) b = a;
	std::cout << "deque<int> operater[] return type is " << Cpg_GetTypeCategory(b) << std::endl;

	std::vector<int> list;
	list.push_back(0);

	std::cout << "vector<int> operater[] return type is " << Cpg_GetTypeCategory(list[0]) << std::endl;

	/*
	* vector<int>의 경우에는 operator[]의 return type이 int&지만 (왠만한 경우 다 그러함)
	* vector<bool>의 경우에는 operator[]의 return type이 new object 이다.
	* 이것에 대한 자세한 것은 Item6에서 다룬다.
	* 그렇지만, 여기서 다룰 중요하게 다룰 내용은 operator[]의 return type이 operator[]의 container에 종속성을 가진다는 것 입니다.
	* 예를 들자면, 27 line의 template 함수에서 반환형은 c[i]의 type에 의해 결정 된다는 것이지요.
	* 이러한 경우에 trailing return type과 함께 decltype을 사용할 수 있습니다
	* 또한 C++14 부터는 trailing return type을 빼고 auto만 남겨서 사용할 수 있습니다.
	*/

	std::vector<bool> c;
	c.push_back(false);
	c.push_back(true);
	std::cout << "vector<bool> operater[] return type is " << Cpg_GetTypeCategory(c[0]) << std::endl;
	std::cout << "vector<bool> operater[] return type is " << Cpg_GetTypeCategory(c[1]) << std::endl;

	std::cout << authAndAccess(list, 0) << std::endl;
	std::cout << authAndAccess(c, 0) << std::endl;

	/*
	* Item2에서는 auto return type을 쓰는 함수들과
	* 컴파일러들이 사용하는 template type deduction에 대해 설명했다.
	* Item1에서 설명했듯이, 초기화 식에서 type deduction은 &로 동작하지 않습니다. (&를 직접 붙여주지 않는 이상)
	*/

	/*
	* Container의 operator[]의 반환 타입은 T&지만
	* auto는 타입 추론 과정에서 &를 제거해버립니다.
	* 하지만, decltype(auto)를 사용하게 되면 기존의 T&를 있는 그대로 추론하게 됩니다. (35 line)
	* (But, 아직 개선이 필요합니다. 아래에서 다뤄 보겠습니다.)
	*/

	std::deque<int> d;
	d.push_back(0);
	std::cout << "authAndAccess(deque<int>, 5) return type is " << Cpg_GetTypeCategory(authAndAccess(d, 0)) << std::endl;
	std::cout << "Do assign d[0] = 1" << std::endl;
	authAndAccess(d, 0) = 1;
	std::cout << "d[0] : " << d[0] << std::endl;
	std::cout << std::endl;

	/*
	* decltype(auto)는 function return type에 국한되지 않고 아래와 같이 사용할 수도 있습니다.
	*/

	Widget w;
	const Widget& cw = w;
	auto myWidget = cw;
	decltype(auto) myWidget2 = cw;

	std::cout << "Widget type w is " << Cpg_GetTypeCategory(w) << std::endl;
	std::cout << "const Widget& cw type is " << Cpg_GetTypeCategory(cw) << std::endl;
	std::cout << "auto(const Widget&) myWidget is " << Cpg_GetTypeCategory(myWidget) << std::endl;
	std::cout << "decltype(auto(const Widget&)) type is " << Cpg_GetTypeCategory(myWidget2) << std::endl;

	/*
	* container는 operator[]를 &로 반환함으로서 client에게 container를 수정할 수 있게 해줍니다.
	* 그러나, 반환 값이 &라는 것은 rvalue function에 이 값을 전달할 수는 없다는 것을 의미합니다.
	*/

	auto s = authAndAccess(d, 0);
	std::cout << "\"auto s = authAndAccess(d, 0)\" type is " << Cpg_GetTypeCategory(s) << std::endl;
	std::cout << std::endl;

	/*
	* 물론 아래와 같이 const reference에 대한 것은 이 case에서 다루지 않습니다 ^_^.
	*/

	int testNum = 10;
	int&& rvalueRef = 10;
	const int& refTestNum = rvalueRef;

	/*
	* 아래와 같이 사용하는 것의 의미는(authAndAccess(makeStringDeque(), 0))
	* rvalue와 lvalue에 대해서 function 지원을 모두 하겠다는 것 입니다.
	* 하지만, 이러한 경우 우리는 lvalue와 rvalue 파라미터 2개에 대한 오버로딩 된 함수를 유지보수해야만 합니다.
	* 그것을 피하기 위해서 우리는 이와 같이 구현할 수 있습니다. (line 43)
	* Item 24에서 universal reference (&&)가 하는 것에 대해 정확하게 설명할 것 입니다.
	*/

	auto auto_test = authAndAccess(makeStringDeque(), 0);
	decltype(auto) decltype_auto_test = authAndAccess(makeStringDeque(), 0);
	std::cout << "---------------------------" << std::endl;
	std::cout << "* authAndAccess(makeStringDeque(), 0)" << std::endl;
	std::cout << "---------------------------" << std::endl;
	std::cout << "auto type is " << Cpg_GetTypeCategory(auto_test) << std::endl;
	std::cout << "decltype(auto) type is " << Cpg_GetTypeCategory(decltype_auto_test) << std::endl;
	std::cout << std::endl;

	/*
	* Item 25의 경고에 따라 std::forward를 universal reference에 적용시킬 필요가 있습니다. (line 48)
	*/

	/*
	* decltype은 앞서 말했듯이 거의 대부분의 상황에서 항상 사용자가 기대한 타입을 만들어낸다고 했습니다.
	* 대부분의 상황에서 항상 이라는 것은 예외 상황이 있다는 의미입니다.
	* decltype의 special case에 대해 친숙해질 필요가 있을 것 입니다.
	* 
	* decltype은 name이 아닌 lvalue expression에 대해서 T& type을 추론합니다.
	* 이러한 case에 대해 우리는 알고 있어야 합니다. 왜냐하면 함수의 반환형에서도 마찬가지로 동작하기 때문입니다.
	* 
	* decltype(auto) add(int a, int b)
	* {
	*	int result = a + b;
	*	// Do something
	*	return (result); // 이러한 상황에서 함수 내부의 lvalue에 대한 참조를 반환하기 때문에
	*					 // undefined behavior를 유발할 수 있습니다.
	* }
	* 
	* 대부분의 경우에서 decltype은 우리가 expect하는 값으로 동작하겠지만
	* 아닌 case에 대해 인지하고 있어야만 할 것 입니다.
	*/

	int num = 10;
	decltype(auto) noBrace = 10;
	decltype(auto) brace = (num);

	std::cout << "decltype(auto) noBrace is " << Cpg_GetTypeCategory(noBrace) << std::endl;
	std::cout << "decltype(auto) brace is " << Cpg_GetTypeCategory(brace) << std::endl;

	return 0;
}