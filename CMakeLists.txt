project(amerge)

set(Boost_Libs boost_filesystem boost_program_options)
set(Boost_Test_Libs boost_filesystem boost_unit_test_framework)

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB SRC_FILES src/*.cpp)
add_executable(amerge ${SRC_FILES})
target_link_libraries(amerge ${Boost_Libs})

file(GLOB SRC_FILES test/*.cpp)
add_executable(common_test ${SRC_FILES})
target_link_libraries(common_test ${Boost_Test_Libs})

enable_testing()

add_test(common_test ${EXECUTABLE_OUTPUT_PATH}/common_test)
