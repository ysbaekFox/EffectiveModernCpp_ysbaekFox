Effective Modern C++ (by Scott Meyers)
- Test Code
- Explaination
- Translate to Korean

이해가 가지 않는 부분
- Item3 28p 첫번째 문단. (전체 이해에는 영향 없음)
- Item7 'An implication~' 문단 (전체 이해에는 영향 없음)
- Item12 비멤버 cbegin 관련 설명 (그냥 넘어가도 될 듯)

Boost Library 사용법
- boost 공식 홈페이지에서 다운로드
- "b2.exe -j8 -a --toolset=msvc-14.2 variant=debug,release link=static,shared threading=multi address-model=64"로 build
- THIRD_PARTY_LIB 환경 변수에 설치한 Boost의 Root 경로 설정
- CMake 설정은 이미 되어있으므로 빌드 및 실행
