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

template<typename T, typename... Ts>
void doSomeWork(Ts&&... params)
{
	// create local T object from params ...
	T localObject1(std::forward<Ts>(params));
	T localObject2{ std::forward<Ts>(params) };
}

int main()
{
	{
		Widget1 w1(0);
		Widget2 w2_1();  // �ν��Ͻ� ������ �ǵ������� �����Ϸ��� �װ��� �Լ� �������� ó���ϴ�
						 // most vexing parse ������ �߻��� �� ����
						 // Visual Studio
						 // C4930 : ������Ÿ�� �Լ��� ȣ����� �ʾҽ��ϴ�. ���� ���Ƿ� ����Ϸ��� �� ���� �ƴ��� Ȯ���Ͻʽÿ�.
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
										// (10, true�� long double�� convert ��)

		StdInitWidget w3( 10, 5.0);		// Widget(int i, bool b)
		StdInitWidget w4{ 10, 5.0 };	// Widget(std::initializer_list<long double> il)
										// (10, 5.0�� long double�� convert ��)
		std::cout << std::endl;
		std::cout << std::endl;
	}

	{
		/*
		* MSCV �����Ϸ��� GNU 9.3.0 �����Ϸ� ����� �ٸ��ϴ�.
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
		* braces { } �� std::initializer_list<bool>�� �ſ� �����ϰ� ���� �Ǿ��ֽ��ϴ�.
		* ������ �˸��� matching�� �ƴϴ��� ����Ǿ� ������ ������ �߻��մϴ�.
		* �ֳ��ϸ� std::initializer_list<bool>�� ���� narrowing conversion�� �߻��ϰ� ���ٵ�,
		* C++ ��Ģ��, braces { } �ȿ����� narrowing conversion�� �����Ǿ� �ֱ� �����Դϴ�.
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
		* ��κ��� case�� �ٷ�����, Edge Case�� �ֽ��ϴ�.
		* default �����ڿ� std::initializer_list�� �Բ� ���� ��� �Դϴ�.
		* �̷� ��� most vexing parse�� �߻��� �� �ֽ��ϴ�.
		*/

		EdgeCaseWidget w1;		// Widget()
		EdgeCaseWidget w2{};	// Widget()
		EdgeCaseWidget w3();	// most vexing parse!!
		EdgeCaseWidget w4({});	// Widget(std::initializer_list<int> il)
		EdgeCaseWidget w5{{}};	// Widget(std::initializer_list<int> il)
	}

	{
		/*
		* ���ݱ��� �˾ƺ� �̻��� ��Ģ���� �󸶳� ���� �ڵ��� �־ �߿��� �����ϱ��?
		* ����� �����ϴ� �ͺ��� �� �߿��մϴ�. �ֳ��ϸ� ������ ���� Ŭ������ �� �ϳ���
		* �ٷ� std::vector �̱� ��������.
		*/
		
		std::vector<int> v1( 10, 20 );	// { 20, 20, ... 20 } 10��
		std::vector<int> v2{ 10, 20 };	// { 10, 20 } 2��

		/*
		* parentheses, braces, constructor overloading resolution ��Ģ�� �ٽ� ���ô�.
		* �� discussion���� 2���� �߿��� takeaways(�߿��� ����, ���)�� �ֽ��ϴ�.
		* ù��°��, (����� Ŭ���� �����ڷμ�), ���� ����� �ϳ� �̻��� std::initializer_list�� �����ϴ�
		* �����ڸ� �����ε��ؾ��Ѵٸ�, braced initializeation {}�� ����ϴ� client code�� ���� std::initilize_list����
		* overload �ȴٴ� ���� �����ؾ��Ѵٴ� �� �Դϴ�.
		* ��, ����� �����ڸ� �������ϴ� ���� ���� ����� client�� parenthese () �Ǵ� braces {}�� ����ϵ��� ���� �����ε忡 ������ ����� �Ѵٴ� �� �Դϴ�.
		* 
		* ��°�δ� class�� ����ϴ� client�μ� ����� parenthese ()�� braces {} �� �ſ� ���ɽ����� �����ؾ߸� �Ѵٴ� �� �Դϴ�.
		* ��κ��� �����ڴ� �ݵ�� ����ؾ߸� �ϴ� ����� �� ����, ()Ȥ�� {}�� �ϳ��� �����ؼ� ����մϴ�.
		* 
		* braces�� default�� ����ϴ� ������� 
		* �װ��� ���Ҽ� ���� ������ applicability(�̿밡�ɼ�)
		* braces�� narrowing conversion ����
		* C++ most vexing parse�� ���� �鿪��
		* �� �ŷ��� �����ϴ�.
		* 
		* �׸��� parenthese�� default�� ����ϴ� �������
		* �������� C++98 ���� ���� 
		* auto-deduced-a-std::initializer_list ���� ȸ��
		* �����Ƿ� ���� std::initializer_list �����ڿ� ����, ��ü ������ ���θ����� �Ϳ� ���� ȸ��
		* �� �ŷ��� �����ϴ�.
		* 
		* �� �� ������� �ٸ� ���ʺ��� �� ���ٴ� consensus�� �����ϴ�.
		* �׷��� �� �� �ϳ��� pick�ϰ� �װ��� ���������� �����ϼ���.
		*/
	}

	{
		/*
		* ���� ����� tempate �����ڶ��,
		* object ������ ���� braces�� parentheses ������ tension(��������?)�� ������ �Ҹ��������� �� �ֽ��ϴ�.
		* �ֳ��ϸ�, �Ϲ������� ������ ���Ǿ����� �ƴ� ���� �Ұ����ϱ� ���� �Դϴ�.
		*
		* ���� ���, ����� ���� ������ ���ڸ� ����Ͽ� ������ Type�� �����Ѵٰ� �����غ��ô�.
		* �� �Ʒ��� ���� �ڵ尡 �ִٰ� �����غ��ô�.
		*
		*	template<typename T, typename... Ts>
		*	void doSomeWork(Ts&&... params)
		*	{
		*		// create local T object from params ...
		*		T localObject1(std::forward<Ts>(params));
		*		T localObject2{ std::forward<Ts>(params) };
		*	}
		*/

		doSomeWork<std::vector<int>>(10, 20);

		/*
		* �̰��� Standard Library �Լ� std::make_unique�� std::make_shared�� ������ �����Դϴ�. (see Item 21)
		* �� �Լ����� �� ������ ���������� parentheses �� ����ϰ� �׷��� ������ ����ȭ�Ͽ� �ذ��Ͽ����ϴ�.
		*
		*/
	}

	return 0;
}