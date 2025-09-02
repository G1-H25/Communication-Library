#include <unity.h>
#include <iostream>

// Forward declarations for test functions
extern void test_http_response_contract();
extern void test_get_method_contract();
extern void test_head_method_contract();
extern void test_post_method_contract();
extern void test_empty_buffer_contract();
extern void test_header_lifetime_contract();
extern void test_error_handling_contract();

int main() {
    std::cout << "Running Communication Library Contract Tests..." << std::endl;

    // Initialize Unity
    UNITY_BEGIN();

    // Run all test cases
    RUN_TEST(test_http_response_contract);
    RUN_TEST(test_get_method_contract);
    RUN_TEST(test_head_method_contract);
    RUN_TEST(test_post_method_contract);
    RUN_TEST(test_empty_buffer_contract);
    RUN_TEST(test_header_lifetime_contract);
    RUN_TEST(test_error_handling_contract);

    // Finalize Unity and return result
    int result = UNITY_END();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << "Some tests failed!" << std::endl;
    }

    return result;
}
