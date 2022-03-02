#include <iostream>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;

template<typename T>
void f(T a)
{
	std::cout << type_id_with_cvr<T>().pretty_name() << '\n';
}

int main()
{
	/*
	* boost library를 사용해서 추론된 타입을 볼 수 있습니다.
	*/

	const int& a = 4;
	std::cout << type_id_with_cvr<decltype(a)>().pretty_name() << '\n';
	f(4);

	return 0;
}