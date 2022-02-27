# Deducing Types

#### Item 1 : Understand template type dedution
Case 1. ParamType is Reference or Pointer, but not a Universal Reference

```c++
template<typename T>
void f(T& param);
...
...
f(expr)
```

> 1. 만약 expr의 type이 reference이면, reference를 무시하고
> 2. 그런 다음에, expr의 타입을 T를 결정하기 위해서 ParamType과 대조하여 유형에 맞춰라. 

예를 들어서 ParamType에 &T가 들어간다고 했을 때, T가 어떻게 결정되는 지 알아보겠다.

1. int
2. const int
3. const int &

