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
        struct header_view
        {
            std::string_view name;
            std::string_view value;
        };

        struct response_info
        {
            int status_code;
            std::size_t body_size;
        };

        class RestInterface
        {
            public:
                virtual ~RestInterface() = default;

                // Core REST verbs (request writes response into caller-provided buffer)
                virtual response_info get(
                    const std::string_view& path,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;

                virtual response_info head(
                    const std::string_view& path,
                    const header_view* headers,
                    std::size_t header_count) = 0;

                virtual response_info delete_(
                    const std::string_view& path,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;

                virtual response_info post(
                    const std::string_view& path,
                    const std::string_view& body,
                    const std::string_view& content_type,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;

                virtual response_info put(
                    const std::string_view& path,
                    const std::string_view& body,
                    const std::string_view& content_type,
                    const header_view* headers,
                    std::size_t header_count,
                    std::span<char> out_body) = 0;

                virtual response_info patch(
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
