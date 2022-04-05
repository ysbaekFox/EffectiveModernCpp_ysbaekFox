#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cpp_types.h>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;

/*
* closure��?
* Lambda ǥ���Ŀ� ���� ��Ÿ�� ����� closure�̴�.
* (��ġ Ŭ������ Ŭ������ ������Ʈ(�ν��Ͻ�) ����� ���ٰ� �� �� �ִ�.)
* 
* �Ʒ��� ���� ǥ������ �ִٰ� �غ���
* auto f = [&](int x, int y) { return fudgeFactor * (x + y); }
* ���� f�� Ŭ������� ������ �� ���� �������� ���� �ƴϴ�.
* f�� Ŭ������ ���纻�� ���̴�. Ŭ������ �����ϴ� ������ move�� ����ȭ�� �� ������
* f�� Ŭ������ �ƴ϶�� ����� ������ �ʴ´�.
* 
* ���� Ŭ���� ������Ʈ�� �ӽ� ��ü�� �� ���� ������ �ı��ȴ�.
* �׷��ٸ� ������ �ӽð�ü�� ���� Ŭ������ ���� �츮�� �˾ƾ��� �ʿ䰡 ������?��� �ǹ��� ���� �� �ִ�.
* �� �κ��� é��6���� ã�� �� �ִ�. (by Scott Meyers)
*/


/*
* closure �� ���� ������ �����Ϸ��� �� �� �ֱ� ������ ����� �� �����ϴ�.
* �׷�����, auto�� ����ϸ� closure�� hold�� �� �ֽ��ϴ�.
*/

/*
* auto ������ Ŭ������ ũ�� ��ŭ�� �޸𸮸� �����.
* ������ ��Ȳ�԰� ������ �ݺ� �ٿ���
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
	* Lmaba�� auto Parameter ��� ����
	*/
	auto drefLess =
		[]( const auto& p1,
			const auto& p2 )
	{ return *p1 < *p2;  };

	/*
	* std::function�� ���� �ñ״�ó�� ���� ��� callable ��ü�� refer�� �� �ִ�.
	* std::function�� ��� �ñ״�ó�� ���� ������ size�� �Ҵ��Ѵ�.
	* ����, �ش� Ŭ������ std::function�� �Ҵ��� �޸��� ũ�⸦ �ʰ��� ��� heap memory�� �����Ѵ�.
	* auto�� Ŭ������ ũ�⿡ �˸��� ��ŭ �޸𸮸� �Ҵ� �Ѵ�.
	*/

	/*
	* auto�� type shortcuts ������ ���ϴ� ����̴�.
	*/
	
	std::vector<int> list;
	list.push_back(0);
	list.push_back(1);
	list.push_back(2);

	const auto listSize = list.size();
	noRef_f(listSize);
	// size ��ȯ ���� ������ int64 ������
	// ��ȯ ������ int�� ����� ��� type shortcuts �߻��ϰ� ��.
	// auto ����� ��� type shortcuts ���� �߻� ���� ����.


	/*
	* ����, �Ʒ��� ���� ��Ȳ���� type shortcuts �߻��� �� �ֽ��ϴ�.
	* auto ������� ���� ��� �ӽð�ü�� ���� �˴ϴ�.
	* �Ʒ� ��°���� ���� �� �� �ֵ��� �ּ� ���� �ٸ��� �� ���ֽ��ϴ�.
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