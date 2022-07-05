#include <iostream>
#include <memory>

class Example
{
public:
    Example(int, int)
    { std::cout << "Example(int, int) called!" << std::endl;}

    Example(std::initializer_list<int> _initializer_list) // value copy
    { 
        std::cout << "Example(std::initializer_list<int>) called!" << std::endl;
        std::cout << "initializer_list address : " << &_initializer_list << std::endl;
        std::cout << "initializer_list size : " << _initializer_list.size() << std::endl;
    }
};

// shared_ptr 사용 시, make_ 사용이 불가능한 경우
// 1. custom deleter 지정하는 경우
// 2. 중괄호 초기치를 전달해야하는 경우

/*
    // make_shared시에 std::forward 사용 중.
    template<class _Tp, class ..._Args>
    inline _LIBCPP_INLINE_VISIBILITY
    typename enable_if<!is_array<_Tp>::value, shared_ptr<_Tp>>::type
    make_shared(_Args&& ...__args)
    {
        return shared_ptr<_Tp>::make_shared(_VSTD::forward<_Args>(__args)...);
    }
*/

int main()
{
   // 중괄호 초기치를 전달해야하는 경우, error 빌생
   //auto errExample = std::make_shared<Example>({1, 2});

   auto a = std::make_shared<Example>(1, 2); /* Example(int, int) called! */

   auto b = std::make_shared<Example>(std::initializer_list<int>{1, 2});

   auto tmp = { 1, 2 };
   std::cout << "temp address : " << &tmp << std::endl;
   auto c = std::make_shared<Example>(tmp);
   
   int arr[4] = { 1,3,5,7 };
   auto qq = { arr[0],
               2,
               arr[1],
               4,
               arr[2],
               6,
               arr[3],
               8 };
    std::cout << sizeof(qq) << std::endl;

    std::initializer_list<int> testList = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << sizeof(testList) << std::endl;

   return 0;
}