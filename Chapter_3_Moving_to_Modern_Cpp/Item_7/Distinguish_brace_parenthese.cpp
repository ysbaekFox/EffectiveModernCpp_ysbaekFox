#include <iostream>
#include <vector>

struct Widget1
{
	Widget1(int _count)
		: count(0)
	{}

	int count;
};

struct Widget2
{

};

class Widget
{
public:
	Widget(int i, bool b);
	Widget(int i, double d);
};

Widget::Widget(int i, bool b)
{
	std::cout << "Widget(int i, bool b)" << std::endl;
}

Widget::Widget(int i, double b)
{
	std::cout << "Widget(int i, double b)" << std::endl;
}

class StdInitWidget
{
public:
	StdInitWidget(int i, bool b);
	StdInitWidget(int i, double d);
	StdInitWidget(std::initializer_list<long double> il);
};

StdInitWidget::StdInitWidget(int i, bool b)
{
	std::cout << "Widget(int i, bool b)" << std::endl;
}

StdInitWidget::StdInitWidget(int i, double b)
{
	std::cout << "Widget(int i, double b)" << std::endl;
}

StdInitWidget::StdInitWidget(std::initializer_list<long double> il)
{
	std::cout << "Widget(std::initializer_list<long double> il)" << std::endl;
}

class FloatWidget
{
public:
	FloatWidget(int i, bool b);
	FloatWidget(int i, double b);
	FloatWidget(std::initializer_list<long double> il);

	operator float() const;
};

FloatWidget::FloatWidget(int i, bool b)
{
	std::cout << "Widget(int i, bool b)" << std::endl;
}

FloatWidget::FloatWidget(int i, double b)
{
	std::cout << "Widget(int i, double b)" << std::endl;
}

FloatWidget::FloatWidget(std::initializer_list<long double> il)
{
	std::cout << "Widget(std::initializer_list<long double> il)" << std::endl;
}

FloatWidget::operator float() const
{
	std::cout << "float operator" << std::endl;
	return 0.0;
}

class BestMatchWidget
{
public:
	BestMatchWidget(int i, bool b);
	BestMatchWidget(int i, double b);
	BestMatchWidget(std::initializer_list<bool> il);
};

BestMatchWidget::BestMatchWidget(int i, bool b)
{
	std::cout << "Widget(int i, bool b)" << std::endl;
}

BestMatchWidget::BestMatchWidget(int i, double b)
{
	std::cout << "Widget(int i, double b)" << std::endl;
}

BestMatchWidget::BestMatchWidget(std::initializer_list<bool> il)
{
	std::cout << "Widget(std::initializer_list<bool> il)" << std::endl;
}

class NoWayWidget
{
public:
	NoWayWidget(int i, bool b);
	NoWayWidget(int i, double b);
	NoWayWidget(std::initializer_list<std::string> il);
};

NoWayWidget::NoWayWidget(int i, bool b)
{
	std::cout << "Widget(int i, bool b)" << std::endl;
}

NoWayWidget::NoWayWidget(int i, double b)
{
	std::cout << "Widget(int i, double b)" << std::endl;
}

NoWayWidget::NoWayWidget(std::initializer_list<std::string> il)
{
	std::cout << "Widget(std::initializer_list<std::string> il)" << std::endl;
}


class EdgeCaseWidget
{
public:
	EdgeCaseWidget();
	EdgeCaseWidget(std::initializer_list<int> il);
};

EdgeCaseWidget::EdgeCaseWidget()
{
	std::cout << "Widget()" << std::endl;
}

EdgeCaseWidget::EdgeCaseWidget(std::initializer_list<int> il)
{
	std::cout << "Widget(std::initializer_list<int> il)" << std::endl;
}

int main()
{
	{
		Widget1 w1(0);
		Widget2 w2_1();  // 인스턴스 생성을 의도하지만 컴파일러는 그것을 함수 선언으로 처리하는
						 // most vexing parse 문제가 발생할 수 있음
						 // Visual Studio
						 // C4930 : 프로토타입 함수가 호출되지 않았습니다. 변수 정의로 사용하려고 한 것은 아닌지 확인하십시오.
		Widget2 w2_2{};
	}

	// Name Lookup: https://en.cppreference.com/w/cpp/language/lookup
	// Overload Resolution: https://en.cppreference.com/w/cpp/language/overload_resolution

	{
		Widget w1( 10, true );  // Widget(int i, bool b)
		Widget w2{ 10, true };  // Widget(int i, bool b)
		Widget w3( 10, 5.0 );	// Widget(int i, double b)
		Widget w4{ 10, 5.0 };	// Widget(int i, double b)
		std::cout << std::endl;
		std::cout << std::endl;
	}

	{
		StdInitWidget w1( 10, true);	// Widget(int i, bool b)
		StdInitWidget w2{ 10, true };	// Widget(std::initializer_list<long double> il)
										// (10, true는 long double로 convert 됨)

		StdInitWidget w3( 10, 5.0);		// Widget(int i, bool b)
		StdInitWidget w4{ 10, 5.0 };	// Widget(std::initializer_list<long double> il)
										// (10, 5.0은 long double로 convert 됨)
		std::cout << std::endl;
		std::cout << std::endl;
	}

	{
		/*
		* MSCV 컴파일러와 GNU 9.3.0 컴파일러 결과가 다릅니다.
		*/

		FloatWidget w4{ 10, 5.0 };			// Widget(std::initializer_list<long double> il)
		FloatWidget w5( w4 );				// call move ctor
		FloatWidget w6{ w4 };				// Widget(std::initializer_list<long double> il)
		FloatWidget w7( std::move(w4) );	// call move ctor
		FloatWidget w8{ std::move(w4) };	// Widget(std::initializer_list<long double> il)

		std::cout << std::endl;
		std::cout << std::endl;
	}

	{
		/*
		* braces { } 는 std::initializer_list<bool>과 매우 강력하게 연결 되어있습니다.
		* 심지어 알맞은 matching이 아니더라도 연결되어 컴파일 에러가 발생합니다.
		* 왜냐하면 std::initializer_list<bool>로 인해 narrowing conversion이 발생하게 될텐데,
		* C++ 규칙상, braces { } 안에서는 narrowing conversion이 금지되어 있기 때문입니다.
		*/

		// BestMatchWidget w{ 10, 5.0 };

		NoWayWidget w1( 10, true ); // Widget(int i, bool b)
		NoWayWidget w2{ 10, true }; // Widget(int i, bool b)
		NoWayWidget w3( 10, 5.0 );  // Widget(int i, double b)
		NoWayWidget w4{ 10, 5.0 };  // Widget(int i, double b)
		
		std::cout << std::endl;
		std::cout << std::endl;
	}

	{
		/*
		* 대부분의 case를 다뤘지만, Edge Case가 있습니다.
		* default 생성자와 std::initializer_list를 함께 쓰는 경우 입니다.
		* 이런 경우 most vexing parse가 발생할 수 있습니다.
		*/

		EdgeCaseWidget w1;		// Widget()
		EdgeCaseWidget w2{};	// Widget()
		EdgeCaseWidget w3();	// most vexing parse!!
		EdgeCaseWidget w4({});	// Widget(std::initializer_list<int> il)
		EdgeCaseWidget w5{{}};	// Widget(std::initializer_list<int> il)
	}

	{
		/*
		* 지금까지 알아본 이상한 규칙들이 얼마나 실제 코딩에 있어서 중요한 정보일까요?
		* 당신이 생각하는 것보다 더 중요합니다. 왜냐하면 영향이 가는 클래스들 중 하나가
		* 바로 std::vector 이기 때문이죠.
		*/
		
		std::vector<int> v1( 10, 20 );	// { 20, 20, ... 20 } 10개
		std::vector<int> v2{ 10, 20 };	// { 10, 20 } 2개

		/*
		* parentheses, braces, constructor overloading resolution 규칙을 다시 봅시다.
		* 이 discussion에서 2가지 중요한 takeaways(중요한 정보, 사실)이 있습니다.
		* 첫번째는, (당신이 클래스 제작자로서), 만약 당신이 하나 이상의 std::initializer_list를 포함하는
		* 생성자를 오버로드해야한다면, braced initializeation {}을 사용하는 client code가 오직 std::initilize_list에만
		* overload 된다는 것을 인지해야한다는 것 입니다.
		* 즉, 당신의 생성자를 디자인하는 가장 좋은 방법은 client가 parenthese () 또는 braces {}를 사용하든지 간에 오버로드에 영향이 없어야 한다는 것 입니다.
		* 
		* 둘째로는 class를 사용하는 client로서 당신이 parenthese ()와 braces {} 를 매우 조심스럽게 선택해야만 한다는 것 입니다.
		* 대부분의 개발자는 반드시 사용해야만 하는 경우일 때 말곤, ()혹은 {}중 하나를 고정해서 사용합니다.
		* 
		* braces를 default로 사용하는 사람들은 
		* 그것의 비교할수 없이 폭넓은 applicability(이용가능성)
		* braces의 narrowing conversion 금지
		* C++ most vexing parse에 대한 면역성
		* 에 매력을 느낍니다.
		* 
		* 그리고 parenthese를 default로 사용하는 사람들은
		* 전통적인 C++98 문법 유지 
		* auto-deduced-a-std::initializer_list 문제 회피
		* 부주의로 인해 std::initializer_list 생성자에 의해, 객체 생성이 가로막히는 것에 대한 회피
		* 에 매력을 느낍니다.
		* 
		* 둘 중 어느것이 다른 한쪽보다 더 낫다는 consensus는 없습니다.
		* 그러니 둘 중 하나를 pick하고 그것을 지속적으로 적용하세요.
		*/
	}

	{

	}

	return 0;
}