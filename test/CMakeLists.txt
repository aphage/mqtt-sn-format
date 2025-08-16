project(mqtt-sn-format-tests)

add_executable(${PROJECT_NAME} test.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)
target_link_libraries(${PROJECT_NAME} PRIVATE mqtt-sn-format)

catch_discover_tests(${PROJECT_NAME})