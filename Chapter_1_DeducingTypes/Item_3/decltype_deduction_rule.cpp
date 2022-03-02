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
	* decltype�� ���� ��κ��� ��Ȳ���� 
	* �׻� �ڿ� ���� name �Ǵ� expression�� ��Ȯ�ϰ� ������ ������ �߷еȴ�.
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
	* vector<int>�� ��쿡�� operator[]�� return type�� int&���� (�ظ��� ��� �� �׷���)
	* vector<bool>�� ��쿡�� operator[]�� return type�� new object �̴�.
	* �̰Ϳ� ���� �ڼ��� ���� Item6���� �ٷ��.
	* �׷�����, ���⼭ �ٷ� �߿��ϰ� �ٷ� ������ operator[]�� return type�� operator[]�� container�� ���Ӽ��� �����ٴ� �� �Դϴ�.
	* ���� ���ڸ�, 27 line�� template �Լ����� ��ȯ���� c[i]�� type�� ���� ���� �ȴٴ� ��������.
	* �̷��� ��쿡 trailing return type�� �Բ� decltype�� ����� �� �ֽ��ϴ�
	* ���� C++14 ���ʹ� trailing return type�� ���� auto�� ���ܼ� ����� �� �ֽ��ϴ�.
	*/

	std::vector<bool> c;
	c.push_back(false);
	c.push_back(true);
	std::cout << "vector<bool> operater[] return type is " << Cpg_GetTypeCategory(c[0]) << std::endl;
	std::cout << "vector<bool> operater[] return type is " << Cpg_GetTypeCategory(c[1]) << std::endl;

	std::cout << authAndAccess(list, 0) << std::endl;
	std::cout << authAndAccess(c, 0) << std::endl;

	/*
	* Item2������ auto return type�� ���� �Լ����
	* �����Ϸ����� ����ϴ� template type deduction�� ���� �����ߴ�.
	* Item1���� �����ߵ���, �ʱ�ȭ �Ŀ��� type deduction�� &�� �������� �ʽ��ϴ�. (&�� ���� �ٿ����� �ʴ� �̻�)
	*/

	/*
	* Container�� operator[]�� ��ȯ Ÿ���� T&����
	* auto�� Ÿ�� �߷� �������� &�� �����ع����ϴ�.
	* ������, decltype(auto)�� ����ϰ� �Ǹ� ������ T&�� �ִ� �״�� �߷��ϰ� �˴ϴ�. (35 line)
	* (But, ���� ������ �ʿ��մϴ�. �Ʒ����� �ٷ� ���ڽ��ϴ�.)
	*/

	std::deque<int> d;
	d.push_back(0);
	std::cout << "authAndAccess(deque<int>, 5) return type is " << Cpg_GetTypeCategory(authAndAccess(d, 0)) << std::endl;
	std::cout << "Do assign d[0] = 1" << std::endl;
	authAndAccess(d, 0) = 1;
	std::cout << "d[0] : " << d[0] << std::endl;
	std::cout << std::endl;

	/*
	* decltype(auto)�� function return type�� ���ѵ��� �ʰ� �Ʒ��� ���� ����� ���� �ֽ��ϴ�.
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
	* container�� operator[]�� &�� ��ȯ�����μ� client���� container�� ������ �� �ְ� ���ݴϴ�.
	* �׷���, ��ȯ ���� &��� ���� rvalue function�� �� ���� ������ ���� ���ٴ� ���� �ǹ��մϴ�.
	*/

	auto s = authAndAccess(d, 0);
	std::cout << "\"auto s = authAndAccess(d, 0)\" type is " << Cpg_GetTypeCategory(s) << std::endl;
	std::cout << std::endl;

	/*
	* ���� �Ʒ��� ���� const reference�� ���� ���� �� case���� �ٷ��� �ʽ��ϴ� ^_^.
	*/

	int testNum = 10;
	int&& rvalueRef = 10;
	const int& refTestNum = rvalueRef;

	/*
	* �Ʒ��� ���� ����ϴ� ���� �ǹ̴�(authAndAccess(makeStringDeque(), 0))
	* rvalue�� lvalue�� ���ؼ� function ������ ��� �ϰڴٴ� �� �Դϴ�.
	* ������, �̷��� ��� �츮�� lvalue�� rvalue �Ķ���� 2���� ���� �����ε� �� �Լ��� ���������ؾ߸� �մϴ�.
	* �װ��� ���ϱ� ���ؼ� �츮�� �̿� ���� ������ �� �ֽ��ϴ�. (line 43)
	* Item 24���� universal reference (&&)�� �ϴ� �Ϳ� ���� ��Ȯ�ϰ� ������ �� �Դϴ�.
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
	* Item 25�� ��� ���� std::forward�� universal reference�� �����ų �ʿ䰡 �ֽ��ϴ�. (line 48)
	*/

	/*
	* decltype�� �ռ� ���ߵ��� ���� ��κ��� ��Ȳ���� �׻� ����ڰ� ����� Ÿ���� �����ٰ� �߽��ϴ�.
	* ��κ��� ��Ȳ���� �׻� �̶�� ���� ���� ��Ȳ�� �ִٴ� �ǹ��Դϴ�.
	* decltype�� special case�� ���� ģ������ �ʿ䰡 ���� �� �Դϴ�.
	* 
	* decltype�� name�� �ƴ� lvalue expression�� ���ؼ� T& type�� �߷��մϴ�.
	* �̷��� case�� ���� �츮�� �˰� �־�� �մϴ�. �ֳ��ϸ� �Լ��� ��ȯ�������� ���������� �����ϱ� �����Դϴ�.
	* 
	* decltype(auto) add(int a, int b)
	* {
	*	int result = a + b;
	*	// Do something
	*	return (result); // �̷��� ��Ȳ���� �Լ� ������ lvalue�� ���� ������ ��ȯ�ϱ� ������
	*					 // undefined behavior�� ������ �� �ֽ��ϴ�.
	* }
	* 
	* ��κ��� ��쿡�� decltype�� �츮�� expect�ϴ� ������ �����ϰ�����
	* �ƴ� case�� ���� �����ϰ� �־�߸� �� �� �Դϴ�.
	*/

	int num = 10;
	decltype(auto) noBrace = 10;
	decltype(auto) brace = (num);

	std::cout << "decltype(auto) noBrace is " << Cpg_GetTypeCategory(noBrace) << std::endl;
	std::cout << "decltype(auto) brace is " << Cpg_GetTypeCategory(brace) << std::endl;

	return 0;
}