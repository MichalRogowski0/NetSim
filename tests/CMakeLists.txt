set(TESTS
        factory_tests.cpp
        nodes_tests.cpp
        package_tests.cpp
)

add_executable(${PROJECT_NAME}_tests ${TESTS})

target_include_directories(${PROJECT_NAME}_tests PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE
        ${PROJECT_NAME}_lib
        gtest
        gtest_main
)