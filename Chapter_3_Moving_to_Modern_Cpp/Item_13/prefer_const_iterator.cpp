#include <iostream>
#include <vector>
#include <algorithm> 

void Test1()
{
	typedef std::vector<int>::iterator IterT;
	typedef std::vector<int>::const_iterator ConstIterT;

	std::vector<int> values;

	ConstIterT ci =
		std::find( static_cast<ConstIterT>( values.begin() ),
				   static_cast<ConstIterT>( values.end() ), 1983);

	// c++17 컴파일 오류
	// error C2440: 'static_cast': 'ConstIterT'에서 'IterT'(으)로 변환할 수 없습니다.
	// values.insert(static_cast<IterT>(ci), 1998);
}

void Test2()
{
	// It is works on version over to C++17

	std::vector<int> values;
	// cbegin과 cend를 사용
	auto it = std::find(values.cbegin(), values.cend(), 1983);
	values.insert(it, 1998);
}

// container에서 targetVal의 첫 출현을 찾고, 그 위치에 insertVal을 삽입한다.
template<typename C, typename V>
void findAndInsert( C& container, const V& targetVal, const V& insertVal)
{
	using std::cbegin;
	using std::cend;

	auto it = std::find(cbegin(container),	// 비멤버 cbegin
		cend(container), 	// 비멤버 cend
		targetVal);

	container.insert(it, insertVal);
}

int main()
{
	Test1();
	Test2();

	std::vector<int> container = {0, 1, 2, 4, 5, 6, 7};
	findAndInsert(container, 4, 3);
	std::for_each(container.begin(), container.end(), [](int value) { std::cout << value << " "; });

	return 0;
}