#include <iostream>
#include <array>
#include <cmath>

/*
* 모든 constexpr 객체는 const이지만 모든 const 객체가 constexpr인 것은 아닙니다.
* 어떤 변수의 값을 반드시 컴파일 시점 상수를 요구하는 문맥에서 사용할 수 있어야 한다면, constexpr로 정의해야합니다.
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

class Point
{
public:
  constexpr Point(double xVal = 0, double yVal = 0) noexcept
   : x(xVal), y(yVal)
   {}
   
	// c++11에서는 두 가지 제약 때문에 Point의 멤버 함수 setX와 setY를 constexpr로 선언할 수 없습니다.
	// 첫째로, 이 멤버 함수들은 작동 대상 객체를 수정하는데, c++11에서 constexpr 멤버 함수는 암묵적으로 const로 선언됩니다.
	// 둘째로, 이 멤버 함수들은 반환 형식이 void인데, c++11에서 void는 리터럴 형식이 아닙니다.
	// c++14에서는 두 제약 모두 사라졌습니다. 그래서 c++14에서는 Point의 설정용 멤버함수(setter; 줄여서 설정 함수)들도 constexpr이 될 수 있습니다.

  constexpr void setX(double newX) noexcept	// c++14
  { x = newX; }
  
  constexpr void setY(double newY) noexcept	// c++14
  { y = newY; }
  
  constexpr double xValue() const noexcept { return x; }
  constexpr double yValue() const noexcept { return y; }
  
private:
   double x, y;
};

constexpr Point midpoint(const Point& p1, const Point& p2) noexcept
{
	// constexpr 멤버 함수들을 호출
	return { (p1.xValue() + p2.xValue()) /2 ,
			 (p1.yValue() + p2.yValue()) / 2 };
}

constexpr Point reflection(const Point& p) noexcept
{
   // 비const Point를 생성
   Point result;
   
   // 그 Point의 x와 y를 설정
   result.setX(-p.xValue());
   result.setY(-p.yValue());
   
   // 그 복사본을 반환
   return result;
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


	{
		// 컴파일 도중에 알려진 값으로 초기화된 Point 객체(즉, constexpr Point 객체)에 대해 호출된다면 조회용 멤버 함수(getter) 
		// xValue와 yValue 역시 constexpr이 될 수 있습니다.
		// 자료 멤버 x와 y의 값을 컴파일 도중에 알 수 있기 때문입니다.
		// Point의 조회 함수들은 호출한 결과들로 또 다른 constexpr 객체를 초기화하는 constexpr 함수를 작성하는 것이 가능합니다.

		constexpr Point p1(9.4, 27.7);
		constexpr Point p2(28.8, 5.3);
		constexpr auto mid = midpoint(p1, p2);

		// reflectedMid의 값은 (-19.1 -16.5)입니다.
		// 이는 컴파일 도중에 알수 있는 값입니다.
		constexpr auto reflectedMid = reflection(mid);
	}

	return 0;
}