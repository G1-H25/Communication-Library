#ifndef REST_INTERFACE_H
#define REST_INTERFACE_H

#include <string_view>
#include <cstddef>
#include <span>



//! @brief CommunicationLibrary::RestInterface namespace
//! @details This namespace defines the contract for the RestInterface classes
//! This defines the interface needed for the concrete REST implementation for the devices
//! This ensures both our devices use the same interface to communicate with the REST API
//! Gateway will have an API and sensor packages will have REST API as well
//! Sensor packages will POST to the Gateway REST API for sending data and GET from the Gateway REST API for receiving configuration
//! Gateway will GET from sensor packages to check health
//! Each device should have its own route table. Route table should be able to be given as a parameter to the constructor (by reference)
//! Since we are targeting embedded platform, we cannot use std::map or std::unordered_map, or std::vector
//! We will need string views into fixed size arrays or memory pools
//! Each device will be responsible for using ArduinoJson or similar to parse and serialize JSON
namespace CommunicationLibrary
{
    namespace RestInterface
    {
        //! @brief Lightweight header view used to pass request headers without ownership.
        //! @details Names and values refer to caller-owned, null-terminated strings or string views.
        //! The interface does not copy; lifetime must outlive the call.
        struct header_view
        {
            std::string_view name;
            std::string_view value;
        };

        //! @brief Minimal HTTP response metadata.
        //! @details Provides status code and body size written. Convertible to bool for success checks.
        //! @pre Returned by interface methods only; users do not construct directly.
        //! @post When used with output buffers, body_size indicates bytes written into the provided span.
        struct http_response
        {
            int status_code;
            std::size_t body_size;
            explicit operator bool() const { return status_code == 200; }
        };

        class RestInterface
        {
            public:
                virtual ~RestInterface() = default;

                //! @brief HTTP GET request.
                //! @details Writes response body into caller-provided buffer.
                //! @param path Relative path from the configured base URL.
                //! @param headers Pointer to an array of header_view entries (may be nullptr when header_count==0).
                //! @param header_count Number of header entries provided.
                //! @param out_body Caller-owned output span to write response bytes into.
                //! @return http_response with status_code and body_size bytes written into out_body.
                //! @pre `out_body.data()` is valid and `out_body.size()` reflects writable capacity.
                //! @pre Each header_view strings must remain valid for the call duration.
                //! @post `body_size <= out_body.size()`.
                //! @post No ownership is taken; all buffers remain caller-owned.
                virtual http_response get(
                    const std::string_view& path,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;

                //! @brief HTTP HEAD request.
                //! @details No response body is written; headers/status only.
                //! @param path Relative path from the configured base URL.
                //! @param headers Pointer to an array of header_view entries (may be nullptr when header_count==0).
                //! @param header_count Number of header entries provided.
                //! @return http_response with status_code; body_size is zero.
                virtual http_response head(
                    const std::string_view& path,
                    const header_view* headers,
                    std::size_t header_count) = 0;

                //! @brief HTTP DELETE request.
                //! @details Writes response body into caller-provided buffer.
                //! @param path Relative path from the configured base URL.
                //! @param headers Pointer to an array of header_view entries (may be nullptr when header_count==0).
                //! @param header_count Number of header entries provided.
                //! @param out_body Caller-owned output span to write response bytes into.
                //! @return http_response with status_code and body_size bytes written into out_body.
                virtual http_response delete_(
                    const std::string_view& path,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;

                //! @brief HTTP POST request.
                //! @details Sends a request body and writes response body into caller-provided buffer.
                //! @param path Relative path from the configured base URL.
                //! @param body Request body view; data must remain valid for the call duration.
                //! @param content_type MIME content type of the request body (e.g., "application/json").
                //! @param headers Pointer to an array of header_view entries (may be nullptr when header_count==0).
                //! @param header_count Number of header entries provided.
                //! @param out_body Caller-owned output span to write response bytes into.
                //! @return http_response with status_code and body_size bytes written into out_body.
                //! @pre `body` and `content_type` lifetimes outlive the call.
                //! @post `body_size <= out_body.size()`.
                virtual http_response post(
                    const std::string_view& path,
                    const std::string_view& body,
                    const std::string_view& content_type,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;

                //! @brief HTTP PUT request.
                //! @details Sends a request body and writes response body into caller-provided buffer.
                //! @copydetails post
                virtual http_response put(
                    const std::string_view& path,
                    const std::string_view& body,
                    const std::string_view& content_type,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;

                //! @brief HTTP PATCH request.
                //! @details Sends a request body and writes response body into caller-provided buffer.
                //! @copydetails post
                virtual http_response patch(
                    const std::string_view& path,
                    const std::string_view& body,
                    const std::string_view& content_type,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;
            protected:
                RestInterface() = default;
                RestInterface(const RestInterface&) = default;
                RestInterface& operator=(const RestInterface&) = default;
        };
    }
}

#endif // REST_INTERFACE_H
