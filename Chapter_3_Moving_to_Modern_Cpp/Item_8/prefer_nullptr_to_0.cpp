#include <iostream>
#include <mutex>

void f(int)
{
	std::cout << "void f(int)" << std::endl;
}

void f(bool)
{
	std::cout << "void f(bool)" << std::endl;
}

void f(void*)
{
	std::cout << "void f(void*)" << std::endl;
}

void CallTest()
{
	f(0);		// f(int)를 호출
	f(NULL);	// f(int)를 호출
	f(nullptr);	// f(void*)를 호출
}

struct Widget
{
	int num = 0;
};

int f1(std::shared_ptr<Widget> spw)
{
	std::cout << "shared_ptr<Widget>" << std::endl;

	return 0;
}

double f2(std::unique_ptr<Widget> upw)
{
	std::cout << "unique_ptr<Widget>" << std::endl;

	return 0.0;
}

bool f3(Widget* pw)
{
	std::cout << "Widget* pw" << std::endl;

	return false;
}

template<typename FuncType, typename MuxType, typename PtrType>
decltype(auto) lockAndCall(FuncType func, MuxType& mutex, PtrType ptr)
{
	using MuxGuard = std::lock_guard<std::mutex>;

	MuxGuard g(mutex);
	return func(ptr);
}

int main()
{
	/*
	*
	* #ifndef NULL
	* 	#ifdef __cplusplus
	* 		#define NULL 0
	* 	#else
	* 		#define NULL ((void*)0)
	* 	#endif
	* #endif
	* 리터럴 0은 int이지 포인터가 아니다. 포인터만 사용할 수 있는 위치에 0이 있으면 c++은 마지못해 그것을 널 포인터로 해석한다.
	* 정의를 보면 NULL은 __cplusplus 일 경우 0으로 정의하고 있다.
	*
	*/

	CallTest();
	std::cout << std::endl;
	std::cout << std::endl;

	/*
	* nullptr 장점
	* nullptr는 모든 형식의 포인터이다. ( nullptr의 실제 형식은 std::nullprt_t )
	* std::nullptr_t는 모든 생(raw) 포인터 형식으로 암묵적으로 변환되며, 이 덕분에 nullptr는 마치 모든 형식의 포인터처럼 행동한다.
	* 앞에서 나온 중복잭재 함수 f를 nullptr로 호출하면 void* 중복적재(즉, 포인터를 받는 버전)가 호출된다. (nullptr가 정수 형석으로 해석되지 않음)
	*/

	/*
	* nullptr는 코드의 명확성도 높여준다.
	*
	*	void Test()
	*	{
	*		auto result = findRecord();
	*		if (result == 0)
	*		{
	*			...
	*		}
	*
	*		if (result == nullptr)
	*		{
	*			...
	*		}
	*	}
	*/

	/*
	* 템플릿에서의 nullptr 사용
	*/

	std::mutex f1m, f2m, f3m;

	//auto result1 = lockAndCall(f1, f1m, 0);	// 오류
	//auto result2 = lockAndCall(f2, f2m, NULL);	// 오류
	auto result4 = lockAndCall(f1, f3m, nullptr);	// ok
	auto result5 = lockAndCall(f2, f3m, nullptr);	// ok
	auto result6 = lockAndCall(f3, f3m, nullptr);	// ok

	return 0;
}