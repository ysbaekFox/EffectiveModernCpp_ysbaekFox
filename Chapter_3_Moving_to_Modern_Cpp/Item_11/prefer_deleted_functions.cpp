#include <iostream>
#include <type_traits>

#include <boost/type_index.hpp>
using boost::typeindex::type_id_with_cvr;

/*
* 이전 까지는 선언만하고 정의하지 않는 방법으로
* 기본으로 생성되는 function을 삭제한 것처럼 동작하게 만들었습니다.
*/

template<class charT, class traits = std::char_traits<charT>>
class basic_ios : public ios_base
{
private:

	// private 섹션에 선언되어 있으므로 클라이언트가 호출할 수 없다.
	basic_ios(const basic_ios&);			 // 선언만 하고 정의 X
	basic_ios& operator=(const basic_ios&);	 // 선언만 하고 정의 X
};

/*
* C++11 이상부터, delete 키워드 사용해서 함수를 삭제할 수 있습니다.
* 참고로, 삭제된 함수는 private이 아니라 public으로 선언하는 것이 관례입니다.
*/

template<class charT, class traits = std::char_traits<charT>>
class new_basic_ios : public ios_base
{
public:
	new_basic_ios(const new_basic_ios&) = delete;
	new_basic_ios& operator=(const new_basic_ios&) = delete;
};

/*
* 또 다른 trick으로 template instantiation에 delete가 사용될 수 있습니다.
* 아래와 같이 사용하면, 
*/

template<typename T>
void processPointer(T* ptr)
{
	std::cout << type_id_with_cvr<T>().pretty_name() << '\n';
}

template<>
void processPointer<void>(void*) = delete;

template<>
void processPointer<char>(char*) = delete;


/*
* delete 키워드를 사용하여 class 내부에 선언된 template instantiation을 금지할 때
* 아래와 같이 private 멤버로 처리하여 사용을 금지할 수 없습니다. (빌드 에러 발생)
* 
	class Widget
	{
	public:
		template<typename T>
		void processPointer(T* ptr)
		{ }

	private:
		template<>
		void processPointer<void>(void*); // Error !!!
	};

	template<>
	void Widget::processPointer<void>(void*) = delete; //문제없이 봉인
* 
* 아래와 같이 외부에서 delete를 사용하여 삭제할 수 있습니다.
* 참고로, 아래 코드는 C++17에서는 동작하지 않습니다. (C++11 권장)
* 
	class Widget
	{
	public:
		template<typename T>
		void processPointer(T* ptr)
		{

		}
	};

	template<>
	void Widget::processPointer<void>(void*) = delete; //문제없이 봉인
*/

int main()
{
	double* d = nullptr;
	processPointer(d);

	// delete 되어 사용 불가.
	//void* v = nullptr;
	//processPointer(v);

	// delete 되어 사용 불가.
	//char* c = nullptr;
	//processPointer(c);

	return 0;
}