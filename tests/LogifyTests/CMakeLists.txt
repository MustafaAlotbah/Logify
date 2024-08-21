
add_executable(LogifyTests EXCLUDE_FROM_ALL "main.cpp" "versionTests.cpp")
target_link_libraries(LogifyTests PRIVATE Logify Catch2::Catch2)
