#include <iostream>
#include <type_traits>
#include <vector>
#include <time.h>
#include <utility>

// inf f(int x) throw(); // c++98 방식(deprecate)
int f(int x) noexcept;	 // c++11 방식

/*
* 
* 실행시점에서 어떤 예외가 f 바깥으로 튀어나오면 f의 예외 지정이 위반된다. 
* C++98에서는 예외 지정이 위반되면 호출 스택이 f를 호출한 지점에 도달할 때까지 풀리며(unwind), 
* 그 지점에서 몇 가지 동작이 취해진 후 프로그램 실행이 종료된다(terminate).
* 
* C++11의 예외 지정에서는 실행시점 행동이 약간 다르다. 
* C++11에서는 프로그램 실행이 종료되기 전에 호출 스택이 풀릴 수도 있고 풀리지 않을 수도 있다.
* 
* 호출 스택이 풀리는 것과 풀릴 수도 있는 것의 차이는 컴파일러의 코드 작성에 놀랄 만큼 큰 영향을 미친다.
* noexcept 함수에서 컴파일러의 최적화기(optimizer)는 예외가 함수 바깥으로 전파될 수 있다고 해도 실행시점 스택을 풀기 가능 상태로 유지할 필요가 없다. 
* 또한, 예외가 noexcept 함수를 벗어난다고 해도 noexcept 함수 안의 객체들을 반드시 생성의 반대 순서로 파괴해야 하는 것도 아니다.
* (컴파일러가 더 효율적인 object code를 생성할 수 있게 한다)
* 
* 
* 하지만 C++11의 이동 연산을 통해 최적화를 하면 push_back은 강력한 예외 안전성을 보장할 수 없게 된다.
* 
* std::vector<Widget>을 사용하는 어떤 코드에서, 종종 Widget들을 push_back을 이용해서 벡터에 추가한다고 하자.
* push_back을 하다 보면 메모리가 부족하여 재할당하는 경우가 생긴다. 
* 재할당이 일어날 때 기존의 메모리에서 새로 할당받은 메모리로 각 요소(예에서는 Widget 객체)가 복사된다.
* 
* 복사되는 경우 push_back은 강력한 예외 안전성을 보장할 수 있다. 
* 복사가 모두 끝난 후 기존의 메모리에 있던 객체들과 기존의 메모리가 해제되므로, 복사 도중에 예외가 발생해도 std::vector의 상태는 변하지 않는다.
* 
* 하지만 C++11의 이동 연산을 통해 최적화를 하면 push_back은 강력한 예외 안전성을 보장할 수 없게 된다.
* 기존 메모리에서 n개의 요소를 이동한 후 (n+1)번째 요소를 이동하는 도중에 예외가 발생하면 push_back 연산이 완료되지 못하고 실패한다. 
* 그런데 원래의 std::vector는 이미 수정된 상태이다.
* 요소 중 n개가 다른 곳으로 이동했는데, 그것들을 원래대로 복원하는 것은 불가능할 수 있다. 
* 객체를 원래의 메모리로 다시 이동하는 연산 자체에서도 예외가 발생할 수 있기 때문이다.
* 그래서, 이동 연산들이 예외를 방출하지 않음이 확실하지 않은 한 C++11 컴파일러는 push_back 안의 복사 연산들을 소리없이 이동 연산들로 대체하지 않는다. 
* 이동 연산이 예외를 방출하지 않음이 확실한 경우에는 복사를 이동으로 대체해도 안전하다.
* 
* std::vector::push_back 외에도, 표준 라이브러리의 여러 함수는 이러한 "가능하면 이동하되 필요하면 복사한다" 전략을 활용한다. 
* 특히 C++98에서 강한 예외 안전성을 보장하는 다른 함수들이 그런 식으로 작동한다.
* 그런데 이동 연산이 예외를 방출하지 않음을 함수가 어떻게 알아낼 수 있을까? 답은 명백하다. 주어진 연산이 noexcept로 선언되어 있는지를 점검하면 된다.
* 
* std::vector::push_back 같은 함수는 std::move_if_noexcept를 호출하는데, 
* 이것은 타입의 이동 생성자의 noexcept 여부에 따라 우측값으로의 조건부 캐스팅을 수행하는, std::move의 한 변형이다
* 
*/

/*
* 흥미롭게도, 표준 라이브러리에 있는 swap들의 noexcept 여부는 사용자 정의 swap들의 noexcept 여부에 어느 정도 의존한다.
* 예를 들어 다음은 표준 라이브러리에 있는 배열에 대한 swap과 std::pair에 대한 swap의 선언들이다.
* 
* template <class T, size_t N>
* void swap(T(&a)[N], T(&b)[N]) noexcept(noexcept(swap(*a, *b)));
*
* template <class T1, class T2>
* struct pair {
* 	void swap(pair& p) noexcept(noexcept(swap(first, p.first)) && noexcept(swap(second, p.second)));
* };
* 
* 이 함수들은 조건부 noexcept이다. 즉, 이들이 noexcept인지의 여부는 noexcept절 안의 표현식들이 noexcept인지에 의존한다.
* 
* 예를 들어 Widget 배열들에 대한 swap은 Widget들에 대한 swap이 noexcept일 때에만 noexcept인 것이다.
* 따라서, Widget 배열들에 대한 swap이 noexcept인지는 Widget을 위한 swap을 작성한 프로그래머가 결정한다.
* 
* noexcept로 선언하는 것이 아주 중요한 일부 함수들은 기본적으로 noexcept로 선언된다. 
* 기본적으로 모든 메모리 해제 함수(operator delete와 operator delete[] 등)와 모든 소멸자는 암묵적으로 noexcept이다.
* 따라서 그런 함수들은 직접 noexcept로 선언할 필요가 없다(직접 선언해도 해가 되지는 않지만, 관례에서 벗어나는 일이다).
* 소멸자가 암묵적으로 noexcept로 선언되지 않는 유일한 경우는, 예외 방출 가능성을 명시적으로 밝힌(즉, noexcept(false)로 선언된) 
* 소멸자를 가진 타입의 데이터 멤버가 클래스에 있을 때 뿐이다.
* 그런 소멸자들은 흔치 않으며, 라이브러리가 사용하는 어떤 객체의 소멸자가 예외를 방출하면, 프로그램의 행동은 정의되지 않는다.
* 
*/

/*
* 대부분의 함수는 noexcept가 아니라 예외에 중립적이다.
* noexcept는 함수의 인터페이스의 일부이다. 따라서 함수의 구현이 예외를 방출하지 않는다는 성질을 오랫동안 유지할 결심이 선 경우에만 함수를 noexcept로 선언해야 한다.
* 만일 함수를 noexcept로 선언하고는 나중에 마음을 바꾼다면, 딱히 흡족한 수습 방안이 없다. 함수의 인터페이스가 변경(noexcept를 제거)되면 클라이언트 코드가 깨질 위험이 생긴다.
* 중요한 것은, 대부분의 함수가 예외에 중립적(exception-neutral)이라는 점이다. 예외 중립적 함수는 스스로 예외를 던지지는 않지만, 예외를 던지는 다른 함수들을 호출할 수는 있다.
* 다른 함수가 예외를 던지면 예외 중립적 함수는 그 예외를 그대로 통과시킨다. 이처럼 통과하는 예외가 있을 수 있으므로, 예외 중립적 함수는 결코 noexcept가 될 수 없다.
* 그러나, 예외를 전혀 방출하지 않는 것이 자연스러운 구현인 함수들도 있으며, noexcept로 선언하면 최적화에 큰 도움이 되는 함수들도 많다(특히 이동 연산들과 swap). 
* 어떤 함수가 예외를 방출하지 않는다는 점을 확신할 수 있다면, 당연히 noexcept로 선언해야 한다.
* 앞에서 어떤 함수들은 noexcept가 자연스러운 구현이라고 말했음을 주목하기 바란다. 함수를 noexcept로 선언하기 위해 함수의 구현을 작위적으로 비트는 것은 마치 꼬리가 개를 흔드는 격이자, 마차를 말 앞에 두는 격이다.
* 예외를 방출하는 함수를, noexcept로 선언하려고 억지로 고치면 함수의 구현이 복잡해질 뿐만 아니라 호출 지점의 코드도 복잡해질 가능성이 크다.
* 이런 코드가 유발하는 실행비용이, noexcept를 통해서 가능한 최적화가 주는 성능 향상을 능가할 수 있고, 복잡한 코드는 유지보수하기도 어려워진다.
* 넓은 계약(wide contract)들을 가진 함수와 좁은 계약(narrow contract)들을 가진 함수를 구분하는 사람들은 넓은 계약을 가진 함수들에 대해서만 noexcept를 사용하는 경향이 있다.
* 넓은 계약을 가진 함수는 전제조건이 없는 함수를 말한다. 그런 함수는 프로그램의 상태와 무관하게 호출할 수 있으며, 
* 호출자가 전달하는 인수들에 그 어떤 제약도 가하지 않는다. 넓은 계약 함수는 켤코 미정의 행동을 보이지 않는다.
* 
* wide contract의 예) .size() (조건 없이 항상 호출 할 수 있음)
* narrow contract의 예) .at(0) (인덱스 초과하는 것 접근하면 Error 발생함)
* 
* 넓은 계약을 가진 함수가 아닌 함수들은 모두 좁은 계약을 가진 함수이다. 그런 함수의 경우 함수의 전제조건이 위반되면 그 결과는 미정의 행동이다.
* 넓은 계약을 가진 함수를 작성하는 경우, 만일 그 함수가 예외를 던지지 않음을 알고 있다면 이 항목의 조언을 따라 함수를 noexcept로 선언하는 것은 쉬운 일이다.
* 좁은 계약을 가진 함수는 함수 내에서 전제조건을 위반하였음을 알리는 예외를 던질 가능성이 있다. 던져진 예외를 디버깅하는 것이 미정의 행동의 원인을 추적하는 것보다 더 쉽기 때문이다.
* 이런 이유로, 넓은 계약과 좁은 계약을 구분하는 라이브러리 설계자들은 넓은 계약을 가진 함수들에 대해서만 noexcept를 사용하는 경향이 있다.
* 마지막 요점으로, C++11에서도 함수 구현과 예외 지정 사이의 비일관성을 파악하는 데 컴파일러가 별 도움을 주지 않는다.
*/

struct foo
{
	int value;

	explicit foo(int value) : value(value)
	{
		//std::cout << "foo(" << value << ")\n";
	}

	foo(foo const& other) : value(other.value)
	{
		//std::cout << "foo(foo(" << value << "))\n";
	}

	// noexcept 여부에 따라 속도가 달라집니다.
	// MSVC에서는 암시적으로 생성하는 것으로 보임.
	foo(foo&& other) // noexcept 
		: value(std::move(other.value))
	{
		//other.value = -1;
		//std::cout << "foo(move(foo(" << value << "))\n";
	}

	~foo()
	{
		if (value != -1)
		{
			//std::cout << "~foo(" << value << ")\n";
		}
	}
};

struct Good
{
	Good() {}
	Good(Good&&) noexcept       // 예외를 던지지 않는다
	{
		std::cout << "Non-throwing move constructor called\n";
	}
	Good(const Good&) noexcept  // 예외를 던지지 않는다
	{
		std::cout << "Non-throwing copy constructor called\n";
	}
};

struct Bad
{
	Bad() {}
	Bad(Bad&&)          // 예외를 던질 수 있다
	{
		std::cout << "Throwing move constructor called\n";
	}
	Bad(const Bad&)     // 예외를 던질 수 있다
	{
		std::cout << "Throwing copy constructor called\n";
	}
};

/*
* std::conditional은 컴파일 타임에 조건 BoolType에 의거하여, TrueType, FalseType으로 분기하려 할 때 필요한 traits 템플릿
* template의 첫번째 인자가 ture면 두번째 인자로 치환되고 false일 경우 3번째 인자로 치환됩니다.
*/

/*
* 이동(move) 또는 복사(copy)될 수 있는 "X" 를 인자로 받아, 이동 생성자가 noexcept이면 std::move(X)를 반환, 그렇지 않으면 X를 반환한다.
* STL container들이 move semantics에 대한 strong exception guarantee를 요구할 때 사용되고 있다.
* 
* move_if_noexcept 구현
	template <typename T>
	typename std::conditional<
		!std::is_nothrow_move_constructible<T>::value&& std::is_copy_constructible<T>::value,
		const T&,
		T&&
	>::type move_if_noexcept(T& x);
*/

int main()
{
	{
		// test case 1
		// 만약 foo의 이동 생성자가 noexcept를 보장하지 않으면, copy construction이 발생합니다.
		// 즉, reallocating 과정에서 이동이 아닌 복사가 발생하여 성능 이득을 얻지 못하게 되는 것입니다.

		std::vector<foo> foos;

		clock_t start = clock();

		for (int i = 0; i < 10000000; i++)
		{
			foos.emplace_back(0);
		}

		clock_t end = clock();

		std::cout << static_cast<double>(end) - start << std::endl;
	}

	{
		/*
		* 이동(move) 또는 복사(copy)될 수 있는 "X" 를 인자로 받아,
		* 이동 생성자가 noexcept이면 std::move(X)를 반환, 그렇지 않으면 X를 반환한다.
		*/

		{
			Good g;
			Good r_g = std::move_if_noexcept(g);

			Bad b;
			Bad r_b = std::move_if_noexcept(b);
		}
	}

	{

	}

	return 0;
}