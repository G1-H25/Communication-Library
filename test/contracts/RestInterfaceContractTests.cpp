#include <unity.h>
#include "../../include/CommunicationLibrary/RestInterface.h"
#include <cstring>
#include <cstddef>

// Mock implementation for contract testing
class RestInterfaceMock : public CommunicationLibrary::RestInterface::RestInterface
{
public:
    // Mock behavior flags
    bool should_fail;
    int mock_status_code;
    std::size_t mock_body_size;

    RestInterfaceMock() : should_fail(false), mock_status_code(200), mock_body_size(0) {}

    CommunicationLibrary::RestInterface::http_response get(
        const std::string_view& path,
        const CommunicationLibrary::RestInterface::header_view* headers,
        std::size_t header_count,
        std::span<char> out_body) override
    {
        if (should_fail) {
            return {500, 0};
        }

        // Mock response
        if (out_body.size() > 0) {
            const char* response_text = "mock response";
            std::size_t text_len = std::strlen(response_text);
            std::size_t copy_size = (text_len < out_body.size()) ? text_len : out_body.size();
            std::memcpy(out_body.data(), response_text, copy_size);
            return {mock_status_code, copy_size};
        }

        return {mock_status_code, 0};
    }

    CommunicationLibrary::RestInterface::http_response head(
        const std::string_view& path,
        const CommunicationLibrary::RestInterface::header_view* headers,
        std::size_t header_count) override
    {
        if (should_fail) {
            return {500, 0};
        }
        return {mock_status_code, 0};
    }

    CommunicationLibrary::RestInterface::http_response delete_(
        const std::string_view& path,
        const CommunicationLibrary::RestInterface::header_view* headers,
        std::size_t header_count,
        std::span<char> out_body) override
    {
        if (should_fail) {
            return {500, 0};
        }

        if (out_body.size() > 0) {
            const char* response_text = "deleted";
            std::size_t text_len = std::strlen(response_text);
            std::size_t copy_size = (text_len < out_body.size()) ? text_len : out_body.size();
            std::memcpy(out_body.data(), response_text, copy_size);
            return {mock_status_code, copy_size};
        }

        return {mock_status_code, 0};
    }

    CommunicationLibrary::RestInterface::http_response post(
        const std::string_view& path,
        const std::string_view& body,
        const std::string_view& content_type,
        const CommunicationLibrary::RestInterface::header_view* headers,
        std::size_t header_count,
        std::span<char> out_body) override
    {
        if (should_fail) {
            return {500, 0};
        }

        if (out_body.size() > 0) {
            const char* response_text = "posted";
            std::size_t text_len = std::strlen(response_text);
            std::size_t copy_size = (text_len < out_body.size()) ? text_len : out_body.size();
            std::memcpy(out_body.data(), response_text, copy_size);
            return {mock_status_code, copy_size};
        }

        return {mock_status_code, 0};
    }

    CommunicationLibrary::RestInterface::http_response put(
        const std::string_view& path,
        const std::string_view& body,
        const std::string_view& content_type,
        const CommunicationLibrary::RestInterface::header_view* headers,
        std::size_t header_count,
        std::span<char> out_body) override
    {
        if (should_fail) {
            return {500, 0};
        }

        if (out_body.size() > 0) {
            const char* response_text = "put";
            std::size_t text_len = std::strlen(response_text);
            std::size_t copy_size = (text_len < out_body.size()) ? text_len : out_body.size();
            std::memcpy(out_body.data(), response_text, copy_size);
            return {mock_status_code, copy_size};
        }

        return {mock_status_code, 0};
    }

    CommunicationLibrary::RestInterface::http_response patch(
        const std::string_view& path,
        const std::string_view& body,
        const std::string_view& content_type,
        const CommunicationLibrary::RestInterface::header_view* headers,
        std::size_t header_count,
        std::span<char> out_body) override
    {
        if (should_fail) {
            return {500, 0};
        }

        if (out_body.size() > 0) {
            const char* response_text = "patched";
            std::size_t text_len = std::strlen(response_text);
            std::size_t copy_size = (text_len < out_body.size()) ? text_len : out_body.size();
            std::memcpy(out_body.data(), response_text, copy_size);
            return {mock_status_code, copy_size};
        }

        return {mock_status_code, 0};
    }
};

// Global mock instance for tests
static RestInterfaceMock* g_mock = nullptr;

// Test setup and teardown
void setUp(void)
{
    if (g_mock == nullptr) {
        g_mock = new RestInterfaceMock();
    }
    g_mock->should_fail = false;
    g_mock->mock_status_code = 200;
    g_mock->mock_body_size = 0;
}

void tearDown(void)
{
    // Clean up if needed
}

// Contract test: http_response boolean conversion
void test_http_response_contract()
{
    CommunicationLibrary::RestInterface::http_response response;

    // Test success case (200)
    response.status_code = 200;
    TEST_ASSERT_TRUE(static_cast<bool>(response));

    // Test failure cases
    response.status_code = 404;
    TEST_ASSERT_FALSE(static_cast<bool>(response));

    response.status_code = 500;
    TEST_ASSERT_FALSE(static_cast<bool>(response));
}

// Contract test: GET method preconditions and postconditions
void test_get_method_contract()
{
    char buffer[100];
    std::span<char> out_body(buffer, 100);

    // Test successful GET
    auto response = g_mock->get("/test", nullptr, 0, out_body);

    // Postcondition: body_size <= out_body.size()
    TEST_ASSERT_TRUE(response.body_size <= out_body.size());

    // Postcondition: status_code is valid
    TEST_ASSERT_TRUE(response.status_code > 0);
}

// Contract test: HEAD method always returns zero body_size
void test_head_method_contract()
{
    auto response = g_mock->head("/test", nullptr, 0);

    // Postcondition: HEAD always has zero body_size
    TEST_ASSERT_EQUAL(0, response.body_size);
}

// Contract test: POST method with body
void test_post_method_contract()
{
    char buffer[100];
    std::span<char> out_body(buffer, 100);
    std::string_view body = "test body";
    std::string_view content_type = "text/plain";

    auto response = g_mock->post("/test", body, content_type, nullptr, 0, out_body);

    // Postcondition: body_size <= out_body.size()
    TEST_ASSERT_TRUE(response.body_size <= out_body.size());
}

// Contract test: Empty buffer handling
void test_empty_buffer_contract()
{
    std::span<char> empty_span; //! default ctor makes an empty span

    auto response = g_mock->get("/test", nullptr, 0, empty_span);

    // Postcondition: body_size must be 0 for empty buffer
    TEST_ASSERT_EQUAL(0, response.body_size);
}

// Contract test: Header lifetime (caller-owned)
void test_header_lifetime_contract()
{
    std::string header_name = "Content-Type";
    std::string header_value = "application/json";
    CommunicationLibrary::RestInterface::header_view header{header_name, header_value};

    char buffer[100];
    std::span<char> out_body(buffer, 100);

    // Store original values
    std::string original_name = header_name;
    std::string original_value = header_value;

    auto response = g_mock->post("/test", "body", "text/plain", &header, 1, out_body);

    // Postcondition: Header strings remain valid and unchanged
    TEST_ASSERT_EQUAL_STRING(original_name.c_str(), header.name.data());
    TEST_ASSERT_EQUAL_STRING(original_value.c_str(), header.value.data());
}

// Contract test: Error handling
void test_error_handling_contract()
{
    g_mock->should_fail = true;
    g_mock->mock_status_code = 500;

    char buffer[100];
    std::span<char> out_body(buffer, 100);

    auto response = g_mock->get("/test", nullptr, 0, out_body);

    // Postcondition: Error status codes are valid
    TEST_ASSERT_TRUE(response.status_code >= 400);
}

// Test functions are called from main.cpp using Unity framework
