#include <iostream>
#include <string>
#include <type_traits>
#include <thread>

// std::move는 주어진 인수를 무조건 오른값으로 캐스팅하고, std::forward는 특정 조건이 만족될 때에만 그런 캐스팅을 수행한다.
// 아래 예제를 보면, 단순 캐스팅을 한다는 것을 알 수 있음.

// std::move의 간단한 구현 (완벽히 동일하진 않다.)
// remove_reference??
// std::remove_reference<T>
// T가 참조자 type이라면 T가 가리키는 type을 제공하고
// 그 외에는 T type 그대로를 제공한다.
// 즉 T& -> T
//    T  -> T 를 제공하는 것

// 즉 아래 예제에서는
// T& -> T -> + && -> T&&
// T  -> T -> + && -> T&&
// 가 되는 것 임.

template<typename T>
typename std::remove_reference<T>::type&& move(T&& param)
{
    using ReturnType = typename remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

// c++14 이상에서 좀 더 간단하게 구현
template<typename T>
decltype(auto) move(T&& param)
{
	using ReturnType = typename remove_reference<T>::type&&;
	return static_cast<ReturnType>(param);
}

struct _string
{
	_string() = default;
	~_string() = default;

    _string(const _string& rhs)
    {
		std::cout << "copy!!" << std::endl;
    }

    _string(_string&& rhs)
    {
		std::cout << "move!!" << std::endl;
    }
};

class Annotation_1
{
public:
    explicit Annotation_1(_string _str)
        : str(std::move(_str)) // 복사?, 이동? 무엇이 동작할까
                               // move 발생!!
    {
    }

private:
    _string str;
};

class Annotation_2
{
public:
	explicit Annotation_2(const _string _str)
		: str(std::move(_str)) // 복사?, 이동? 무엇이 동작할까
                               // copy 발생!!
	{
	}

private:
    _string str;
};

// std::move -> 항상 rvalue로 캐스팅
// std::forward -> 특정 조건이 만족 될때만 rvalue로 캐스팅
// 아래 예제를 통해 알아보자

struct E_Widget
{ };

void process(const E_Widget& lvalArg)
{
    std::cout << "process(const Widget& lvalArg)" << std::endl;
}

void process(E_Widget&& rvalArg)
{
	std::cout << "process(Widget&& rvalArg)" << std::endl;
}

template<typename T>
void logAndProcess(T&& param)
{
	process(std::forward<T>(param));
}

// 우리는 std::move를 사용하지 않고 std::forward만 사용하면 아무 문제도 없지 않느냐?라고 생각할 수 있다.
// 순수하게 기술적인 부분에서는 그래도 되지만...... 각자 차이가 꽤 있다.

// std::forward
// 1. rvalue로 전달된 참조만 rvalue로 캐스팅하겠다는 의미.
// 2. 매개변수 T를 사용하여, rvalue라는 정보를 encoding하는 번거로움을 감수해야함.
// 3. 전달하려는 객체를 기존의 성질(rvalue인지 lvalue인지)를 유지하여 그냥 넘겨 주는 것

// std::move
// 1. 주어진 인수를 무조건 rvalue로 캐스팅하겠다는 의미.
// 2. 매개변수 T를 사용하여, rvalue라는 정보를 encoding하는 번거로움이 없음 (타자량이 적다.)
// 3. 전달하려는 객체를 move하겠다는 것
// 4. 잘못된 형식을 지정하는 실수를 저지를 여지가 없다 
// - 예를 들어 std::forwad의 경우,  실수로 std::string&를 지정하면, 이동 생성이 아니라 복사 생성이 된다,

int main()
{
    _string str1;
	Annotation_1 anno1(std::move(str1));
	std::cout << std::endl;

	_string str2;
	Annotation_2 anno2(std::move(str2));
	std::cout << std::endl;

	// T&& param은 lvalue 이다. (type은 rvalue 참조 타입)
	// lvalue이므로 process 함수 호출 시, 항상 lvalue 버전을 호출하게 된다.
	// 우리는 param이 rvalue로 초기화 된 경우에는 rvalue 버전 process 함수를 호출하게 만들고 싶을 것이다.
	// 이 때 필요한 것이 std::forward<T>이다.
	// 그렇다면 std::forward는 param이 lvalue로 초기화되었는지 rvalue로 초기화 되었는지 어떻게 알 수 있을까?
	// 그것은 템플릿 매개변수 T에 그 정보가 encoding 되어 있다는 것이다. 매개변수 T는 std::forward로 전달 되며
	// 거기서 해당 정보를 복원할 수 있다. (자세한 내용은 Item28 참고)

	E_Widget widget;
	logAndProcess(widget);			  // 내부에서 process 함수 호출 시, lvalue 버전으로 오버로딩
	logAndProcess(std::move(widget)); // 내부에서 process 함수 호출 시, rvalue 버전으로 오버로딩
	std::cout << std::endl;

    return 0;
}