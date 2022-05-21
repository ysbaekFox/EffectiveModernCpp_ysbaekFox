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