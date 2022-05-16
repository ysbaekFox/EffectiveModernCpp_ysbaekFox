#include <iostream>
#include <vector>
#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

void Test1()
{
	enum Color { block, white, red };
   /*
	* 오류! 이 범위에 이미 white가 선언되어 있음
	* errorC2365: 'white': 재정의: 이전 정의는 '열거자'입니다.
	*/

	// auto white = false;
}

void Test2()
{
	enum class Color { block, white, red };

	// ok: 이 범위에 다른 "white"는 없음
	{
		auto white = false;
	}

	{
		// 오류! 이 범위에 "white"라는 이름의 열거자가 없지만, 형식이 맞지 않음.
		// Color c = white;
	}

	{
		// ok
		Color c = Color::white;
	}

	{
		// ok
		auto c = Color::white;
	}
}

double calc(double x)
{
	return x * 1.0;
}

void Test3()
{
	// unscoped enum
	// Color의 범위가 지정되지 않음
	enum Color { block, white, red };
	Color c = red;

	// Color를 double과 비교(!)
	// enum을 정수형식과 비교하는 상황이 발생한다.
	if (c < 14.5)
	{
		auto factors = calc(c);
		std::cout << "type : " << type_id_with_cvr<decltype(factors)>().pretty_name() 
			<< ", " << factors << std::endl;
	}
}

void Test4()
{
	enum class Color { block, white, red };
	
	// 오류, 정의되지 않은 red
	// Color c = red; 

	Color c = Color::red;

	// 오류, 암시적 형변환 불가능
	// int colorNum = Color::red;

	int colorNum = static_cast<int>(Color::red);
	std::cout << "red : " << colorNum << std::endl;
#if 0
	/*
	* 오류!!
	* Color와 double를 비교할 수 없음
	* error C2678: 이항 '<': 왼쪽 피연산자로 'Color' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	*/
	if (c < 14.5)
	{
		/*
		 * 오류
		 * 정수형을 기대하는 함수에 Color를 전달할 수 없음
		 */
		auto factors = calc(c);
	}
#endif
}

/*
* 전방 선언이 안되는 경우라면 해당 enum을 사용하는 모든 Code를 새로 빌드해야하지만,
* 전방 선언을 하고 내부 요소는 각 Code별로 따로 정의가 가능해지므로,
* 사용하는 곳 거기만 빌드해서 해결이 가능합니다.
*/

enum class Color; // 전방 선언(forward-declaration)이 가능
enum Status : std::uint32_t; // C++11 이상에서는 unscoped enum도 가능하지만, type을 지정해줘야합니다.

int main()
{
	Test1();
	Test2();
	Test3();
	Test4();

	std::cout << std::endl;

	/*
	* 그렇다고 모든 경우에 대해서 다 scoped enum이 편할까요 ?
	* enum 값을 배열이나 std::tuple 의 인덱스로 사용하는 경우는 unscoped enum이 더 편리합니다.
	*/

	using UserInfo = std::tuple<std::string, std::string, std::size_t>;
	enum UserInfoFields { EM_NAME, EM_ADDR, EM_SCORE };
	enum class UserInfoFields_ { EM_NAME, EM_ADDR, EM_SCORE };

	UserInfo UI = {"ysbaek", "Busan", 100};

	auto sName = std::get<0>(UI);
	std::cout << sName << std::endl;

	auto sAddr = std::get<EM_ADDR>(UI);
	std::cout << sAddr << std::endl;

	// 매우 불편합니다.
	auto nScore = std::get<static_cast<int>(UserInfoFields_::EM_SCORE)>(UI);
	std::cout << nScore << std::endl;

	return 0;
}