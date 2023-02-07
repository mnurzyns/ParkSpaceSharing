add_executable(ProxyTest)

target_sources(
    ProxyTest PRIVATE
    src/test.cc
)

target_link_libraries(
    ProxyTest PRIVATE
    proxy
    GTest::gtest_main
)

gtest_discover_tests(ProxyTest)
