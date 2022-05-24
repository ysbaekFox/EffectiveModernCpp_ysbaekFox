#include <iostream>
#include <string>

// std::move는 주어진 인수를 무조건 오른값으로 캐스팅하고, std::forward는 특정 조건이 만족될 때에만 그런 캐스팅을 수행한다.

// std::forward를 언제 사용해야 하는가?
// 1. 함수 템플릿을 정의하려고 한다.
// 2. 이 함수 템플릿 내부에 어떤 함수 X를 호출하는데 X의 인자 타입이 rvalue 참조인 것과 그렇지 않은 여러버전의 오버로딩이 존재하고 
//    함수의 인자에 따라 X의 오버로딩중 적절한 것을 선택해 호출할 필요가 있을 때.

// std::move를 사용하는 이유?
// 둘의 유일한 차이는 std::move는 항상 캐스팅하지만 std::forward는 조건에 따라서만 한다는 점을 생각하면, 
// 항상 std::forward만 사용하면 되지 않느냐는 의문이 들 수도 있겠다.
// - std::move의 매력은 사용하기 편하고, 오류의 여지가 줄어들고, 코드의 명확성이 높아진다.

#define USE_STD_FORWARD 1

// std::forward 사용시 주의 할 점
// - std::move 쪽이 std::forward보다 타자량이 적고, 
//   전달하는 것이 오른값이라는 정보를 부호화하는 형식 인수를 지정하는 번거로움도 없다.
// - 또한, 잘못된 형식을 지정하는 실수를 저지를 여지도 없다.
//  (예를 들어 실수로 레퍼런스를 넣을 경우 &를 지정하면 자료 멤버 s가 이동 생성이 아니라 복사 생성된다.)

class Widget
{
public:
    Widget() = default;

#if USE_STD_FORWARD
      Widget(Widget&& rhs)
     : s(std::forward<std::string>(rhs.s))	// 관레에서 벗어난, 바람직하지 않은 구현
   { ++moveCtorCalls; } 
#else
   Widget(Widget&& rhs)
     : s(std::move(rhs.s))
   { ++moveCtorCalls; }
#endif

    int getMoveCnt() { return moveCtorCalls; }

private:
   static int moveCtorCalls;
   std::string s;
};

int Widget::moveCtorCalls = 0;


void overloaded( int const &arg ) { std::cout << "by lvalue\n"; }
void overloaded( int && arg ) { std::cout << "by rvalue\n"; }
 
template< typename T >
    /* 
    "t &&" with "t" being template param is special, and  adjusts "t" to be
       (for example) "int &" or non-ref "int" so std::forward knows what to do. 
    */
void forwarding( T && arg ) 
{
    std::cout << "via std::forward: ";
    overloaded( std::forward< T >( arg ) );
    std::cout << "via std::move: ";
    overloaded( std::move( arg ) ); // conceptually this would invalidate arg
    std::cout << "by simple passing: ";
    overloaded( arg );
}

int main()
{
    Widget lhs;
    Widget rhs(std::move(lhs));

    std::cout << "rhs move cnt : " << rhs.getMoveCnt() << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "initial caller passes rvalue:" << std::endl;
    forwarding( 5 );

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "initial caller passes lvalue:"  << std::endl;
    int x = 5;
    forwarding( x );

    return 0;
}