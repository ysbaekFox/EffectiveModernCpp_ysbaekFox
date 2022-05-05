#include <iostream>
#include <vector>

struct Widget
{
	bool isHighPrior;
};

std::vector<bool> features(const Widget& w)
{
	std::vector<bool> result;
	result.resize(6);
	result[5] = w.isHighPrior;
	return result;
}

int main()
{
	Widget widget;
	widget.isHighPrior = true;

	bool highPriority = features(widget)[5];
	std::cout << highPriority << std::endl;
	
	auto autoHighPriority = features(widget)[5];
	// std::cout << autoHighPriority << std::endl;
	// stdout시에 런타임 에러가 발생 합니다. (Undefined Error로 인한 Crash)

	/*
		왜 런타임 에러가 발생할까요? 먼저 std:::vector<bool>::reference에 대해서 설명해보겠습니다.

		std::vector<bool>은 bit 1자리가 하나의 bool인 형태로 되어있습니다.
		이것은 std::vector<T>의 operator[]가 T&를 반환하는데 있어서 문제를 일으킵니다. (C++은 bit들에 대한 &를 금지합니다.)
		그래서 모든 문맥(contexts)에서 bool&처럼 사용할 수 있는 std::vector<bool>::reference를 사용합니다.
		그런데 여기서 반환 받는 변수의 타입을 bool이 아닌 auto를 사용하게 되면 dangling potiner Error가 발생할 수 있습니다.

		두가지 경우로 나누어서 설명해보겠습니다.

		1. bool type으로 반환 받는 경우
		highPriority은 features(w)[5]에 의해 초기화 됩니다.
		먼저 std::vector<bool>타입인 features(w)의operator[]가 호출되고
		std::vector<bool>::reference를 리턴하고 이는 bool타입 변수를 초기화 하기위하여
		features(w)의 5번째 비트에 해당되는 값이 bool로 형변환되어 저장합니다.
		일련의 과정엔 어떤 오류도 없습니다.

		2. auto type으로 반환 받는 경우
		반면 atuo를 사용하는 경우엔 얘기가 달라집니다.
		autoHighPriority역시 features(w)[5]에 의해 초기화된다.
		하지만 auto이므로 autoHighPriority는 std::vector<bool>::reference로 타입 추론됩니다.
		std::vector<bool>::reference는 참조되는 비트의 주소에 offset값을 더한 주소를 가르킵니다.
		features(w)에서 리턴되는 ravlue 임시 값을 std::vector<bool>를 temp라고 했을 때
		temp가 가지고 있는 연속된 비트의 첫 주소를 0x0001이라 하면,
		temp[5]가 리턴하는 std::vector<bool>::reference 0x0005를 가르키는 포인터를 가지게 됩니다.
		문제는 features(w)가 리턴한 temp가 rvalue라는 점입니다. 대입 연산 이후에 temp는 파괴되기 때문에,
		autoHighPriority 갖는 포인터가 dangling 상태가 된다.
	*/

	/*
	* std::vector<bool>::reference는 proxy pattern의 한 예 입니다.
	* proxy pattern은 다양한 목적으로 채용되어 집니다.
	* 예를 들면 
	* 1. std::vector<bool>::reference는 operator[]가  std::vector<bool>가 반환하는 bit에 대한 reference를 제공하기 위해 존재합니다.
	* 2. STL의 smart pointer는 raw pointer에 resource management를 접목한 proxy class들 입니다.
	* 사실, Proxy Class들의 유용성은 이미 잘 알려져 있습니다.
	* 
	* 몇몇 proxy들은 사용자들에게 잘보이게(명백하게, 누가봐도 알 수 있게) 디자인 되어집니다. ex) std::shared_ptr, std::unique_ptr
	* 다른 proxy class들은 더 invisibly하게 design 되어집니다. ( std::vector<bool>::reference도 invisible proxy들 중 하나 입니다.) 
	* ex) 그외 std::bitset, std::bitset::reference
	* 
	* 그러한 camp(invisible proxy들의 진영)는 대개 "expression temnplates"이라고 알려진 테크닉을 채용합니다.
	* ( Matrix sum = m1 + m2 + m3 + m4;
	*   만약 위와 같은 식에서, operator+가 자기 자신(Maxtrix)가 아닌 
	*   Sum<Matrix, Matrix>와 같은 Proxy Class를 반환한다면 더 효율적으로 계산할 수 있습니다. )
	*/

	/*
	* 우리는 이제 2가지 질문을 해볼 수 있습니다.
	* 첫째, auto에서 문제를 발생시키는 proxy는 컵센접으로 invisible한데 우리가 과연 이것을 어떻게 찾아야할까요?
	* -> library들의 document를 참고하세요
	* -> 
	* 
	* 둘째, 만약 auto를 찾았다면, auto의 많은 장점을 포기하고 auto를 없애야만 할까요?
	* -> 명시적 타입 초기화 idom을 사용하도록 수정할 수 있습니다. 
	*/

	auto newhighPriority = static_cast<bool>(features(widget)[5]);
	std::cout << newhighPriority << std::endl;

	/*
	* auto sum = static_cast<Matrix>( m1 + m2 + m3 + m4 );
	* auto ep = static_cast<float>( calcEpsilon() );
	* auto index = static_cast<int>( d * c.size() );
	*/

	return 0;
}