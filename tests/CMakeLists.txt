add_executable(cryptkx_core_tests
    test_password_generator.cpp
)

target_link_libraries(cryptkx_core_tests
    PRIVATE
        cryptkx_core
)

add_test(NAME cryptkx_core_tests COMMAND cryptkx_core_tests)
