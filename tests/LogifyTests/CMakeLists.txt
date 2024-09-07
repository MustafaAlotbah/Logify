
add_executable(LogifyTests "main.cpp" "versionTests.cpp" "LoggerTests.cpp")
target_link_libraries(LogifyTests PRIVATE Logify Catch2::Catch2)

add_test(NAME LogifyTests COMMAND LogifyTests)