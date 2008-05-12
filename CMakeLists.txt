project(amerge)

set(Boost_Libs boost_filesystem boost_program_options)
set(Boost_Test_Libs boost_filesystem boost_unit_test_framework)

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

file(GLOB SRC_FILES src/*.cpp)
add_executable(amerge ${SRC_FILES})
target_link_libraries(amerge ${Boost_Libs})

add_executable(common_test EXCLUDE_FROM_ALL tests/common_test.cpp src/common.cpp src/defrag.cpp)
target_link_libraries(common_test ${Boost_Test_Libs})

add_executable(defrag_test EXCLUDE_FROM_ALL tests/defrag_test.cpp src/common.cpp src/defrag.cpp)
target_link_libraries(defrag_test ${Boost_Test_Libs})

add_executable(unique_test EXCLUDE_FROM_ALL tests/unique_test.cpp src/common.cpp)
target_link_libraries(unique_test ${Boost_Test_Libs})

add_executable(action_test EXCLUDE_FROM_ALL tests/action_test.cpp src/amerge.cpp src/defrag.cpp
	src/insert.cpp src/unique.cpp src/common.cpp src/create.cpp)
target_link_libraries(action_test ${Boost_Test_Libs})

add_custom_target(tests)
add_dependencies(tests common_test defrag_test unique_test action_test)

add_custom_target(docs doxygen ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxyfile VERBATIM)
add_custom_command(TARGET docs PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
	make_directory ${CMAKE_CURRENT_BINARY_DIR}/doc )

enable_testing()

add_test(common_test ${EXECUTABLE_OUTPUT_PATH}/common_test)
add_test(defrag_test ${EXECUTABLE_OUTPUT_PATH}/defrag_test)
add_test(unique_test ${EXECUTABLE_OUTPUT_PATH}/unique_test)
add_test(action_test ${EXECUTABLE_OUTPUT_PATH}/action_test)
