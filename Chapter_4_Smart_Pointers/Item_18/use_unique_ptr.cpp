#include <iostream>
#include <memory>

// unique_ptr 장점
// - shared_ptr의 경우, raw_pointer로 복귀가 불가능하지만, unique_ptr의 경우 가능하다.
// - 그래서 특별한 목적이 있는 경우가 아니면, 팩토리 함수 등에서 shared_ptr보다는 unique_ptr을 사용하는 것이 좋다.
// - std::unique_ptr를 std::shared_ptr로 손쉽게 변환할 수 있다.

class Example 
{
public:
    void ExamplePrint()
    {
        std::cout << "Example Class" << std::endl;
    }
};

struct DeletorClass {
    void operator() (Example *ptr) const { delete ptr; }
};

struct DeletorClass2 {
    DeletorClass2(std::int32_t num)
    {
        m_arr[0] = m_arr[1] = num;
    }
    void operator() (Example *ptr) const { delete ptr; }
    std::int32_t m_arr[2];
};

int main()
{
    // new보단 make_shared를 사용해야 하는 이유 (Item21에서 예제 다시 보자)
    // -> control block과 actual obeject이 함께 생성되기 때문.
    // 즉, locality (지역성) 와 memory fragmentation이 향상 됨.
    // - Temporal Loaclity : 최근에 접근했던 주소값을 다시 접근하는 경향
    // - Spartial Locality : 최근 접근했던 주소값 근처의 주소들을 접근하는 경향.

    auto usingNew = std::unique_ptr<Example>(new Example);
    usingNew->ExamplePrint();

    auto usingMake = std::make_unique<Example>();
    usingMake->ExamplePrint();

    // unique_ptr 사용 방법에 따라
    // 메모리 사이즈 어떻게 달라지는지 비교 (EBO 적용되면 deleter 메모리 사이즈 측정 X)
    // 바이트 패딩에 의해 memory size가 +8이 됩니다.

    auto deletor = [](Example *ptr) { delete ptr; };
    std::cout << "lambda function size : " << sizeof(deletor) << std::endl;

    auto defaultUniquePtr = std::make_unique<Example>();
    std::cout << "unique_ptr(default) size : " << sizeof(defaultUniquePtr) << std::endl;

    auto lambdaUniquePtr1
        = std::unique_ptr<Example, void(*)(Example*)>(new Example, [](Example *ptr) { delete ptr; });
    std::cout << "unique_ptr(using function) size : " << sizeof(lambdaUniquePtr1) << std::endl;

    auto lambdaUniquePtr2 = std::unique_ptr<Example, decltype(deletor)>(new Example, deletor);
    std::cout << "unique_ptr(using lambda deleter) size : " << sizeof(lambdaUniquePtr2) << std::endl;

    auto deleterClass1 = std::unique_ptr<Example, DeletorClass>(new Example, DeletorClass());
    auto deleterClass2 = std::unique_ptr<Example, DeletorClass2>(new Example, DeletorClass2(1));

    std::cout << "unique_ptr(using deleter class which has no data) size : " << sizeof(deleterClass1) << std::endl;
    std::cout << "unique_ptr(using deleter class which has data) size : " << sizeof(deleterClass2) << std::endl;

    // pointer와 같은 배열 모호성이 없습니다.
    std::unique_ptr<int> singleUniquePtr;
    std::unique_ptr<int[]> arrayUniquePtr;

    return 0;
}