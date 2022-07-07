#include <iostream>
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
	std::shared_ptr<Example> sptr = std::make_shared<Example>();
	std::cout << "sptr use_cout : " << sptr.use_count() << std::endl << std::endl;

	/*
	* weak_ptr 자체로는 원래 객체를 참조할 수 없고, 반드시 shared_ptr 로 변환해서 사용해야 합니다. 
	* 이 때 가리키고 있는 객체가 이미 소멸되었다면 빈 shared_ptr 로 변환되고, 아닐경우 해당 객체를 가리키는 shared_ptr 로 변환됩니다.
	* 1) lock을 사용하여 shared_ptr 생성
	* 2) ctor 사용하여 shared_ptr 생성 (weak_ptr이 expired된 경우 bad_weak_ptr throw)
	*/
	std::weak_ptr<Example> wptr(sptr);
	std::shared_ptr<Example> sptr2 = wptr.lock(); // sptr use_count 증가

	std::cout << "sptr use_cout : " << sptr.use_count() << std::endl;
	std::cout << "sptr2 use_cout : " << sptr2.use_count() << std::endl << std::endl;

	sptr.reset();
	std::cout << "sptr use_cout : " << sptr.use_count() << std::endl;
	std::cout << "sptr2 use_cout : " << sptr2.use_count() << std::endl << std::endl;

	sptr2.reset();
	std::cout << "sptr use_cout : " << sptr.use_count() << std::endl;
	std::cout << "sptr2 use_cout : " << sptr2.use_count() << std::endl << std::endl;

	try
	{
		std::shared_ptr<Example> sptr2(wptr);
	}
	catch (const std::exception& e)
	{
		std::cout << "wptr is nullptr : " << e.what() << std::endl;
	}

	std::shared_ptr<Example> sptr3 = wptr.lock();
	if (nullptr == sptr3.get())
	{
		std::cout << "sptr3 is nullptr" << std::endl;
	}

    return 0;
}