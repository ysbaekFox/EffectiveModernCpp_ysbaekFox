#include <iostream>
#include <string>

struct MyInt {
	int m_i;
};

class Doubler {
public:
	Doubler(MyInt i) : my_int(i) {}

	int doubleIt() {
		return my_int.m_i * 2;
	}

private:
	MyInt my_int;

};

int main() {
	int i = 5;
	Doubler d(MyInt(i)); // most vexing parse here!!
	std::cout << d.doubleIt() << std::endl;
}