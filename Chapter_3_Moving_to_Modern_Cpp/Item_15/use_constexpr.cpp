#include <iostream>
#include <array>
#include <cmath>

/*
* 모든 constexpr 객체는 const이지만 모든 const 객체가 constexpr인 것은 아니다.
* 어떤 변수의 값을 반드시 컴파일 시점 상수를 요구하는 문맥에서 사용할 수 있어야 한다면, constexpr로 정의한다.
*/

void Test()
{
	// 비 constexpr 변수
	int sz;

	// 오류
	// sz의 값이 컴파일 도중에 알려지지 않음
	// constexpr auto arraySize1 = sz;

	// 오류
	// 같은 문제
	// std::array<int, sz> data;

	// ok
	// 10은 확실히 컴파일 시점 상수
	constexpr auto arraySize2 = 10;

	// ok
	// arraySize2는 constexpr 객체
	std::array<int, arraySize2> data;
}

void Test2()
{
	int sz;
	const auto arraySize = sz;
	// Error!!, arraySize의 값은 컴파일 시점에서 알려지지 않음
	//std::array<int, arraySize> data;
}

constexpr int pow(int base, int exp) noexcept
{
	return 0;
}

int main()
{
	{
		// pow 앞에 constexpr이 있다고 해서 pow가 반드시 const 값을 돌려주는 것은 아닙니다.
		// 오직 base와 exp가 컴파일 시점 상수일 때에만 pow의 결과를 컴파일 시점 상수로 사용할 수 있습니다.
		// 만일 base나 exp 중 하나라도 컴파일 시점 상수가 아니면 pow의 결과는 컴파일 시점 상수를 보장하지 않습니다.

		constexpr auto numConds = 5;
		std::array<int, pow(3, numConds)> results;  // No Error

		// int base = 1;
		// int exp = 1;
		// std::array<int, pow(base, exp)> results; // Error!!
	}

	return 0;
}