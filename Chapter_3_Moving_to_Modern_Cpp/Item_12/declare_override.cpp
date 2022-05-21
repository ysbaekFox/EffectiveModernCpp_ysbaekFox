#include <iostream>
#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

class Widget
{
public:

	// doWork의 이 버전은 *this가 왼값일때에만 적용된다.
	void doWork() &
	{
		std::cout << "doWork() &" << std::endl;
	}

	// dowork의 이 버전은 **this가 오른값일 때에만 적용된다.
	void doWork() &&
	{
		std::cout << "doWork() &&" << std::endl;
	}
};

class Base
{
public:
	virtual void mf1() const
	{
		std::cout << "Base::mf1" << std::endl;
	}

	virtual void mf2(int x)
	{
		std::cout << "Base::mf2" << std::endl;
	}

	virtual void mf3()&
	{
		std::cout << "Base::mf3" << std::endl;
	}

	void mf4() const
	{
		std::cout << "Base::mf4" << std::endl;
	}

	virtual void mf5() const
	{
		std::cout << "Base::mf5" << std::endl;
	}

	virtual void mf6()
	{
		std::cout << "Base::mf6" << std::endl;
	}
};

class Derived : public Base
{
public:
	virtual void mf1()
	{
		std::cout << "Derived::mf1" << std::endl;
	}

	virtual void mf2(unsigned int x)
	{
		std::cout << "Derived::mf2 " << std::endl;
	}

	virtual void mf3() &&
	{
		std::cout << "Derived::mf3" << std::endl;
	}

	void mf4() const
	{
		std::cout << "Derived::mf4" << std::endl;
	}

	void mf5() const
	{
		std::cout << "Derived::mf5" << std::endl;
	}

	void mf6() override
	{
		std::cout << "Derived::mf6" << std::endl;
	}
};

int main()
{
	/*
	* lvalue, rvalue에 따라
	* 동작하는 함수 오버로딩 예제
	*/

	// lvalue doWork;
	Widget widget;
	widget.doWork();

	// rvalue doWork;
	Widget().doWork();

	std::cout << std::endl;

	/*
	* 파생 클래스 함수가 같은 이름의 기반 클래스 함수를 재정의하지 Case들 예제
	*/

	// Base에서는 mf1이 const로 선언되었지만, Derived에서는 그렇지 않습니다.
	Base base;
	Derived derived;
	base.mf1();
	derived.mf1();

	std::cout << std::endl;

	// Base에서는 mf2가 int를 받지만, Derived에서는 unsigned int를 받습니다.
	int b_var = 0;
	unsigned int d_var = 0;
	base.mf2(b_var);
	derived.mf2(d_var);

	std::cout << std::endl;

	// Base에서는 mf3이 lvalue로 한정되지만, Derived에서는 rvalue 한정됩니다.
	// derived.mf3(); // Error!! (rvalue만 사용 가능)
	Derived().mf3();

	std::cout << std::endl;

	// Base에서 mf4는 virtual로 선언되지 않은 경우. (mf5와 비교)
	Base* pBase1 = new Derived();
	pBase1->mf4();

	Base* pBase2 = new Derived();
	pBase2->mf5();

	std::cout << std::endl;

	// override 키워드 사용
	// Base 클래스의 멤버 함수에 virtual 키워드가 없을 경우 컴파일 에러 발생

	base.mf6();
	derived.mf6();

	return 0;
}