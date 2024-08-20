
add_executable(LogifyTests "main.cpp" "versionTests.cpp")
target_link_libraries(LogifyTests PRIVATE Logify Catch2::Catch2)
