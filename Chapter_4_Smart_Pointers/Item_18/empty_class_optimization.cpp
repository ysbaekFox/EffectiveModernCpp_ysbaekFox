#include <iostream>

class EmptyClass
{
public:
    void emptyFunc()
    {
        // 함수는 sizeof에 포함되지 않음.
        std::cout << "empty Func" << std::endl;
    }
};

class IntClass
{
private:
    int x;
};

class IntClassInsertedEBO
{
private:
    EmptyClass empty;
    int x;
};

class IntClassDrivedEBO : public EmptyClass
{
private:
    int x;
};

template<class T1, class T2>
class BaseOptimization : public EmptyClass, IntClass
{
public:
    BaseOptimization() {}
    BaseOptimization(T1 const& _emptyClass, T2 const& _intClass)
        : EmptyClass(_emptyClass), IntClass(_intClass) {}
};

int main()
{
    // Empty Class 조건
    // 1. non-static 멤버 데이터가 없어야 한다.
    // 2. virtual 함수가 없어야 한다 (vptr과 vtable)
    // 3. virtual base 클래스가 없어야 한다. (2번과 마찬가지)

    // "독립 구조 객체는 반드시 크기가 0을 넘어야 한다"는 C++ 규칙이 있음.
    // 그래서 아래 EmptyClass는 size가 0이 아님.

    std::cout << "EmptyClass : " << sizeof(EmptyClass) << std::endl;
    
    // 그런데, C++는 상속받은 빈 클래스에 대해 특별 면제를 제공할 수 있음.
    // 상속을 사용하면, 빈 기본 클래스가 공간을 사용하지 않는 방식으로 상속 계층을 평면화할 수 있음.
    // 이것을 EBO라고 함. (또는 EBCO)
    // EBCO: Empty Base Class Optimization
    // 다만, 이 방법은 그렇게까지 좋은 방법은 아님.
    // 왜냐하면, 상속받은 함수를 재정의해버릴 Risk가 있음.

    std::cout << "IntClass : " << sizeof(IntClass) << std::endl;
    std::cout << "IntClassInsertedEBO : " << sizeof(IntClassInsertedEBO) << std::endl;
    std::cout << "IntClassDrivedEBO : " << sizeof(IntClassDrivedEBO) << std::endl; // Empty class size는 포함 안됨.

    // 그래서 아래와 같이 template을 사용하여, 구현할 수 있습니다.
    // 용도 : boost::compressed_pair, unique_ptr
    BaseOptimization<EmptyClass, IntClass> baseOptimization;
    std::cout << "BaseOptimization : " << sizeof(baseOptimization) << std::endl;
    baseOptimization.emptyFunc();

    return 0;
}