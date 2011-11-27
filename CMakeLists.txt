CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
include(ExternalProject)
project(CSE355ExtraCredit)
Find_Package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
find_package(GTest)
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -ggdb -Wall ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG ")
include_directories(.)
ExternalProject_Add(
    RapidXML
    URL http://downloads.sourceforge.net/project/rapidxml/rapidxml/rapidxml%201.13/rapidxml-1.13.zip
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(RapidXML SOURCE_DIR)
include_directories(${SOURCE_DIR})
set(PROJ_SOURCES Automaton.cpp)
add_executable(simulator main.cpp ${PROJ_SOURCES})
target_link_libraries(simulator boost_filesystem boost_system)

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
