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
	// stdout�ÿ� ��Ÿ�� ������ �߻� �մϴ�. (Undefined Error�� ���� Crash)

	/*
		�� ��Ÿ�� ������ �߻��ұ��? ���� std:::vector<bool>::reference�� ���ؼ� �����غ��ڽ��ϴ�.

		std::vector<bool>�� bit 1�ڸ��� �ϳ��� bool�� ���·� �Ǿ��ֽ��ϴ�.
		�̰��� std::vector<T>�� operator[]�� T&�� ��ȯ�ϴµ� �־ ������ ����ŵ�ϴ�. (C++�� bit�鿡 ���� &�� �����մϴ�.)
		�׷��� ��� ����(contexts)���� bool&ó�� ����� �� �ִ� std::vector<bool>::reference�� ����մϴ�.
		�׷��� ���⼭ ��ȯ �޴� ������ Ÿ���� bool�� �ƴ� auto�� ����ϰ� �Ǹ� dangling potiner Error�� �߻��� �� �ֽ��ϴ�.

		�ΰ��� ���� ����� �����غ��ڽ��ϴ�.

		1. bool type���� ��ȯ �޴� ���
		highPriority�� features(w)[5]�� ���� �ʱ�ȭ �˴ϴ�.
		���� std::vector<bool>Ÿ���� features(w)��operator[]�� ȣ��ǰ�
		std::vector<bool>::reference�� �����ϰ� �̴� boolŸ�� ������ �ʱ�ȭ �ϱ����Ͽ�
		features(w)�� 5��° ��Ʈ�� �ش�Ǵ� ���� bool�� ����ȯ�Ǿ� �����մϴ�.
		�Ϸ��� ������ � ������ �����ϴ�.

		2. auto type���� ��ȯ �޴� ���
		�ݸ� atuo�� ����ϴ� ��쿣 ��Ⱑ �޶����ϴ�.
		autoHighPriority���� features(w)[5]�� ���� �ʱ�ȭ�ȴ�.
		������ auto�̹Ƿ� autoHighPriority�� std::vector<bool>::reference�� Ÿ�� �߷е˴ϴ�.
		std::vector<bool>::reference�� �����Ǵ� ��Ʈ�� �ּҿ� offset���� ���� �ּҸ� ����ŵ�ϴ�.
		features(w)���� ���ϵǴ� ravlue �ӽ� ���� std::vector<bool>�� temp��� ���� ��
		temp�� ������ �ִ� ���ӵ� ��Ʈ�� ù �ּҸ� 0x0001�̶� �ϸ�,
		temp[5]�� �����ϴ� std::vector<bool>::reference 0x0005�� ����Ű�� �����͸� ������ �˴ϴ�.
		������ features(w)�� ������ temp�� rvalue��� ���Դϴ�. ���� ���� ���Ŀ� temp�� �ı��Ǳ� ������,
		autoHighPriority ���� �����Ͱ� dangling ���°� �ȴ�.
	*/

	/*
	* std::vector<bool>::reference�� proxy pattern�� �� �� �Դϴ�.
	* proxy pattern�� �پ��� �������� ä��Ǿ� ���ϴ�.
	* ���� ��� 
	* 1. std::vector<bool>::reference�� operator[]��  std::vector<bool>�� ��ȯ�ϴ� bit�� ���� reference�� �����ϱ� ���� �����մϴ�.
	* 2. STL�� smart pointer�� raw pointer�� resource management�� ������ proxy class�� �Դϴ�.
	* ���, Proxy Class���� ���뼺�� �̹� �� �˷��� �ֽ��ϴ�.
	* 
	* ��� proxy���� ����ڵ鿡�� �ߺ��̰�(����ϰ�, �������� �� �� �ְ�) ������ �Ǿ����ϴ�. ex) std::shared_ptr, std::unique_ptr
	* �ٸ� proxy class���� �� invisibly�ϰ� design �Ǿ����ϴ�. ( std::vector<bool>::reference�� invisible proxy�� �� �ϳ� �Դϴ�.) 
	* ex) �׿� std::bitset, std::bitset::reference
	* 
	* �׷��� camp(invisible proxy���� ����)�� �밳 "expression temnplates"�̶�� �˷��� ��ũ���� ä���մϴ�.
	* ( Matrix sum = m1 + m2 + m3 + m4;
	*   ���� ���� ���� �Ŀ���, operator+�� �ڱ� �ڽ�(Maxtrix)�� �ƴ� 
	*   Sum<Matrix, Matrix>�� ���� Proxy Class�� ��ȯ�Ѵٸ� �� ȿ�������� ����� �� �ֽ��ϴ�. )
	*/

	/*
	* �츮�� ���� 2���� ������ �غ� �� �ֽ��ϴ�.
	* ù°, auto���� ������ �߻���Ű�� proxy�� �ż������� invisible�ѵ� �츮�� ���� �̰��� ��� ã�ƾ��ұ��?
	* -> library���� document�� �����ϼ���
	* -> 
	* 
	* ��°, ���� auto�� ã�Ҵٸ�, auto�� ���� ������ �����ϰ� auto�� ���־߸� �ұ��?
	* -> ����� Ÿ�� �ʱ�ȭ idom�� ����ϵ��� ������ �� �ֽ��ϴ�. 
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