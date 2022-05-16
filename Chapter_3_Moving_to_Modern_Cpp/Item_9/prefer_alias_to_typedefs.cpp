#include <iostream>
#include <unordered_map>
#include <string>

typedef std::unique_ptr<std::unordered_map<std::string, std::string>> TypedefUPtrMapSS;
using AliasUPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;

class Widget
{
	int num = 0;
};

template <class T>
class MyAlloc {};

template<typename T>                       // using (alias declaration)
using UsingMyList = std::list<T, MyAlloc<T>>;

template<typename T>                       // typedef
struct TypeDefMyList
{
	typedef std::list<T, MyAlloc<T>> type;
};


/*
* 기존에 typedef를 using을 사용하여 새로 정의하는 법.
*/

template<class T>
using remove_const_t = typename std::remove_const<T>::type;

template<class T>
using remove_refernece_t = typename std::remove_reference_t<T>::type;

template<class T>
using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;

int main()
{
	/*
	* 2개중 alias declaration이 더 직관적이다.
	* typedef std::unique_ptr<std::unordered_map<std::string, std::string>> TypedefUPtrMapSS;
	* using AliasUPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
	*/

	TypedefUPtrMapSS v1;
	AliasUPtrMapSS v2;


	/*
	* alias declaration은 직관적이다라는 장점 외에 typedef와 달리 템플릿화를 지원합니다.
	* typedef도 지원을 하지 않는 것은 아니지만, 아래와 같이 매우 비직관적이면서 불편합니다.
	* (using을 사용하면 의존 타입 이름으로 인한 typedef 명시하지 않아도 됩니다)
	* 
	* template <class T>
	* class MyAlloc {};
	* template<typename T>                       // using (alias declaration)
	* using UsingMyList = std::list<T, MyAlloc<T>>;
	*
	* template<typename T>                       // typedef
	* struct TypeDefMyList
	* {
	*	typedef std::list<T, MyAlloc<T>> type;
	* };
	*/

	UsingMyList<Widget> usingMyList;
	TypeDefMyList<Widget>::type typeDefMyList;

	return 0;
}