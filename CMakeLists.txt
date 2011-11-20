CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(CSE355ExtraCredit)
Find_Package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
find_package(GTest)
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -ggdb -Wall ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG ")
include_directories(.)

set(PROJ_SOURCES Automaton.cpp)
add_executable(simulator main.cpp ${PROJ_SOURCES})

if(GTEST_FOUND)
    ENABLE_TESTING()
    add_custom_target(gtest 
        COMMAND test_buddy
        DEPENDS test_buddy)
    FILE(GLOB Test_Sources Tests/*.cpp)
    
    #Mutex Version
    add_executable(test_automaton ${Test_Sources} ${PROJ_SOURCES})
    target_link_libraries(test_automaton ${GTEST_BOTH_LIBRARIES} pthread)
    ADD_TEST(GTest ${CMAKE_CURRENT_BINARY_DIR}/test_automaton --gtest_output=xml)

endif()
