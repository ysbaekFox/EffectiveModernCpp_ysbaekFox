#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cpp_types.h>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;

/*
* closure란?
* Lambda 표현식에 대한 런타임 결과가 closure이다.
* (마치 클래스와 클래스의 오브젝트(인스턴스) 관계와 같다고 볼 수 있다.)
* 
* 아래와 같은 표현식이 있다고 해보자
* auto f = [&](int x, int y) { return fudgeFactor * (x + y); }
* 에서 f를 클로져라고 생각할 수 있을 것이지만 실은 아니다.
* f는 클로져의 복사본일 뿐이다. 클로져를 복사하는 과정은 move로 최적화될 수 있지만
* f가 클로져가 아니라는 사실은 변하지 않는다.
* 
* 실제 클로져 오브젝트는 임시 객체로 그 줄의 끝에서 파괴된다.
* 그렇다면 람다의 임시객체일 뿐인 클로져를 굳이 우리가 알아야할 필요가 있을까?라는 의문을 가질 수 있다.
* 이 부분은 챕터6에서 찾을 수 있다. (by Scott Meyers)
*/


/*
* closure 형 로컬 변수는 컴파일러만 알 수 있기 때문에 사용할 수 없습니다.
* 그렇지만, auto를 사용하면 closure를 hold할 수 있습니다.
*/

/*
* auto 변수는 클로져의 크기 만큼만 메모리를 사용함.
* 구문의 장황함과 변수의 반복 줄여줌
*/

template<typename It>
void dwim(It b, It e)
{
	std::cout << type_id_with_cvr<It>().pretty_name() << '\n';

	while (b != e)
	{
		/*
		* algorithm to dwim "do what I mean"
		* for all elements in range from b to e
		*/

		//typename std::iterator_traits<It>::value_type currValue = *b;
		auto currValue = *b;
		b++;
	}
}

template<typename T>
void noRef_f(T param)
{
	std::cout << "T type is " << Cpg_GetTypeName(T) << std::endl;
}

int main()
{
	std::vector<int> test;
	test.push_back(0);
	test.push_back(1);
	test.push_back(2);

	dwim(test.begin(), test.end());


	auto derefUPLess =
		[]( const std::unique_ptr<int>& p1,
			const std::unique_ptr<int>& p2 )
	{ return *p1 < *p2; };

	/*
	* Lmaba에 auto Parameter 사용 가능
	*/
	auto drefLess =
		[]( const auto& p1,
			const auto& p2 )
	{ return *p1 < *p2;  };

	/*
	* std::function은 동일 시그니처를 가진 모든 callable 객체를 refer할 수 있다.
	* std::function은 모든 시그니처에 대해 고정된 size를 할당한다.
	* 만약, 해당 클로져가 std::function가 할당한 메모리의 크기를 초과할 경우 heap memory에 저장한다.
	* auto는 클로져의 크기에 알맞은 만큼 메모리를 할당 한다.
	*/

	/*
	* auto는 type shortcuts 문제를 피하는 기술이다.
	*/
	
	std::vector<int> list;
	list.push_back(0);
	list.push_back(1);
	list.push_back(2);

	const auto listSize = list.size();
	noRef_f(listSize);
	// size 반환 값은 실제로 int64 이지만
	// 반환 값으로 int를 사용할 경우 type shortcuts 발생하게 됨.
	// auto 사용할 경우 type shortcuts 문제 발생 방지 가능.


	/*
	* 또한, 아래와 같은 상황에서 type shortcuts 발생할 수 있습니다.
	* auto 사용하지 않을 경우 임시객체가 생성 됩니다.
	* 아래 출력결과를 보면 알 수 있듯이 주소 값이 다른걸 알 수있습니다.
	*/

	std::unordered_map<std::string, int> m;
	m["1"] = 1;
	m["2"] = 2;
	m["3"] = 3;

	for (const std::pair<std::string, int>& p : m)
	{
		std::cout << &p << std::endl;
	}

	std::cout << std::endl;

	for (const auto& p : m)
	{
		std::cout << &p << std::endl;
	}

	return 0;
}