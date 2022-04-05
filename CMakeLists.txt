include_directories ( Common )

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT "$ENV{THIRD_PARTY_LIB}/boost_1_72_0")
set(BOOST_INCLUDE_DIR ${BOOST_ROOT})
set(BOOST_LIBRARY_DIR ${BOOST_ROOT}/stage/lib)

include_directories(${BOOST_INCLUDE_DIR})
link_directories(${BOOST_LIBRARY_DIR})

#add_executable(exec "Chapter_1_DeducingTypes/Item_1/case_1.cpp")
#add_executable(exec "Chapter_1_DeducingTypes/Item_1/case_2.cpp")
#add_executable(exec "Chapter_1_DeducingTypes/Item_1/case_3.cpp")
#add_executable(exec "Chapter_1_DeducingTypes/Item_1/case_4_array_arguments.cpp")
#add_executable(exec "Chapter_1_DeducingTypes/Item_1/case_5_function_arguments.cpp")
#add_executable(exec "Chapter_1_DeducingTypes/Item_2/auto_deduction_rule.cpp")
#add_executable(exec "Chapter_1_DeducingTypes/Item_3/decltype_deduction_rule.cpp")
#add_executable(exec "Chapter_1_DeducingTypes/Item_4/how_to_view_deduced_types.cpp")
add_executable(exec "Chapter_1_DeducingTypes/Item_5/prefer_auto_explicit.cpp")
target_link_libraries(exec ${BOOST_LIBRARIES})