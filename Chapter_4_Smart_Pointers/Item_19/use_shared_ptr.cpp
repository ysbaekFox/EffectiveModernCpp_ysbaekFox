#include <iostream>
#include <vector>
#include <memory>

class Example
{
public:
	void ExamplePrint()
	{
		std::cout << "Example Class" << std::endl;
	}
};

int main()
{
	/*
	 * shared_ptr은 참조 횟수를 관리하므로, 성능에 영향이 있다.
	 
	 1) std::shared_ptr의 크기가 생 포인터의 두 배이다. 
	 - raw pointer 그 자체와 raw pointer가 가지는 reference count를 저장해야 하므로 2배이다.
	 
	 2) 참조 횟수를 담을 메모리를 반드시 동적으로 할당해야 한다.
	 - 개념적으로, reference count는 point되는 object와 연관이 있다.
	 하지만, pointed된 object는 reference count에 대해 아는 것이 없다.
	 그래서 object들은 reference count에 대해 저장할 공간도 없다. 
	 (Item 21에서는 make_shared를 사용했을 때 회피되는 동적 할당 비용에 대해 설명 한다.)
	 어찌 됐든, reference count는 동적으로 저장되어 진다.

	 3) 참조 횟수의 증가와 감소가 반드시 원자적 연산이어야 한다.
	 - reference count의 증감은 multi-thread에서 수행되더라도 thread-safety해야하므로
	 원자적 연산이어야 한다. 원자적 연산(atomic operation)은 전통적으로 비원자적 연산보다 느리다.
	*/

	// move construction
	auto object = std::make_shared<Example>();
	auto movedObject = std::move(object);
	std::cout << "shared_ptr use_count : " << movedObject.use_count() << std::endl;

	auto usingMake = std::make_shared<Example>();
	std::cout << "shared_ptr(default) size " << sizeof(usingMake) << std::endl;

	auto lambdaSharedPtr1 = std::shared_ptr<Example>(new Example, [](Example* ptr) { delete ptr; });
	std::cout << "shared_ptr(using lambda deleter) size " << sizeof(lambdaSharedPtr1) << std::endl;

	/*
	* dereference(역참조) 호출 시에는 overhead가 없을까?
	* 단순 포인터 참조라서 overhead가 없을 것 같지만,
	* std::shared_ptr은 raw pointer에 비해 2배의 메모리를 사용하므로, cache 측면에서 불리함.
	*/

	/*
	* shared_ptr의 경우, unique_ptr과는 다르게 custom deleter가 type의 일부가 아니다.
	*/

	auto customDeleter1 = [](Example* pw) { delete pw; };
	auto customDeleter2 = [](Example* pw) { delete pw; };

	auto u1 = std::unique_ptr<Example, decltype(customDeleter1)>(new Example, customDeleter1);
	auto u2 = std::unique_ptr<Example, decltype(customDeleter2)>(new Example, customDeleter2);

	// unique_ptr, Type의 일부이므로 Error 발생
	//std::vector<std::unique_ptr<Example>> uniqueVector{ u1 , u2 };

	auto s1 = std::shared_ptr<Example>(new Example, customDeleter1);
	auto s2 = std::shared_ptr<Example>(new Example, customDeleter2);

	// shared_ptr, Type의 일부가 아니므로 Error 발생하지 않음.
	std::vector<std::shared_ptr<Example>> sharedVector{ s1 , s2 };

	// unique_ptr과 달리 deleter에 상관 없이 shared_ptr의 size는 2배이다.
	auto pw = std::shared_ptr<Example>(new Example);
	std::cout << "sizeof(pw) : " << sizeof(pw) << std::endl;

	auto lpw = std::shared_ptr<Example>(new Example, [](Example* pw) { delete pw; });
	std::cout << "sizeof(lpw) : " << sizeof(lpw) << std::endl;

    return 0;
}