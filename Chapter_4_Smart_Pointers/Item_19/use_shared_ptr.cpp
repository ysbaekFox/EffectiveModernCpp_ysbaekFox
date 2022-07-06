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

class Widget : public std::enable_shared_from_this<Widget>
{
public:
	void process()
	{
		processWidgets.emplace_back(shared_from_this());
	}

	void Print()
	{
		for (auto& widget : processWidgets)
		{
			std::cout << widget.use_count() << std::endl;
		}
	}

private:
	std::vector<std::shared_ptr<Widget>> processWidgets;
};

class Best : public std::enable_shared_from_this<Best>
{
public:
	[[nodiscard]] static std::shared_ptr<Best> create()
	{
		return std::shared_ptr<Best>(new Best);
	}

	std::shared_ptr<Best> getptr()
	{
		return shared_from_this();
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

	/*
	* std::shared_ptr의 custom deleter는 std::shared_ptr object의 size를 변경시키지 않는다.
	* custom deleter는 function object이고, function object는 제멋대로 많은 data를 포함할 수 있음.
	* 그것의 의미는 std::shared_ptr의 크기가 제멋대로 커질 수 있다는 것이다.
	* 어떻게 std::shared_ptr이 많은 메모리를 사용하지 않고 deleter를 참조할 수 있을까?
	* -> 실제로 불가능하다, 많은 메모리를 실제로 사용하고 있다.
	* 그렇지만, 메모리는 std::shared_ptr object의 일부가 아니다. 그것은 별도의 heap 영역에 할당 된다.
	*/

	/*
	* reference count는 control block이라고 알려진 data structure의 일부이다.
	* control block은 reference count, custom deleter의 copy본 등을 포함한다.
	* 만약 custom allocator가 정의되면, 그것도 포함한다.
	* 그리고 weak count라는 두번째 reference count가 있다. 이것은 Item 21에서 추가로 설명할 것 이다.
	* (당장은 128p의 그림만 마음속에 그려둬라)
	*/

	/*
	* 1) make_shared가 호출 될 때, control block이 생성된다.
	* 2) unique onwership으로부터 shared_ptr이 생성 될 때, control block이 생성된다.
	* 3) raw pointer로부터 std::shared_ptr 생성자가 호출되면, control block이 생성된다.
	*/


	auto loggingDel = [](Example* pw) {
		delete pw;
	};

	//auto duplicated = new Example;
	//std::shared_ptr<Example> spw1(duplicated, loggingDel); // control block 생성.
	//std::shared_ptr<Example> spw2(duplicated, loggingDel); // control block 또 생성.
	// 종료 시 Aborted 발생

	std::shared_ptr<Example> recycle(new Example, loggingDel); // control block 생성.
	std::shared_ptr<Example> recycle2(recycle); // control block 재활용

	/*
	* enable_shared_from_this은 shared_from_this라는
	* current object를 위한 std::shared_ptr을 생성하는 member function을 정의합니다.
	* (하지만, 이것은 control block의 중복 없이 동작 합니다.)
	*/
	
	std::shared_ptr<Widget> widget = std::make_shared<Widget>();
	widget->process();
	widget->process();
	widget->Print();

	std::shared_ptr<Best> best0 = Best::create();
	std::shared_ptr<Best> best1 = best0->getptr();
	std::cout << "best1.use_count() = " << best1.use_count() << '\n';
	
	std::shared_ptr<Example[]> test;
	// -> 컴파일은 되지만, 끔찍한 idea이다.
	// 1) operator[] 지원 안됨.
	// 2)  std::shared_ptr가 지원하는 파생 클래스 포인터 -> 기반 클래스 포인터로의 변환은 배열에
	//     적용하면 타입 시스템에 구멍이 생긴다. (? 정확히 잘모르겠다. 아무튼 쓰지 말자)

    return 0;
}