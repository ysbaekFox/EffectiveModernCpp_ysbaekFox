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
	* boost library�� ����ؼ� �߷е� Ÿ���� �� �� �ֽ��ϴ�.
	*/

	const int& a = 4;
	std::cout << type_id_with_cvr<decltype(a)>().pretty_name() << '\n';
	f(4);

	return 0;
}