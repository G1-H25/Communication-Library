#ifndef COMMUNICATIONLIBRARY_COMMBUFFER_H_
#define COMMUNICATIONLIBRARY_COMMBUFFER_H_

#include <array>
#include <cstddef>
#include <span>

namespace CommunicationLibrary
{
    namespace Buffer
    {
        //! @brief Fixed-capacity character buffer for communication payloads.
        //! @details Zero-allocation, caller-owned buffer. Provides a writable span for transports.
        template<std::size_t Capacity>
        class CommBuffer
        {
            public:
                //! @brief Obtain a writable span over the entire buffer capacity.
                std::span<char> span() { return std::span<char>(storage_.data(), storage_.size()); }

                //! @brief Raw pointer to underlying storage.
                char* data() { return storage_.data(); }

                //! @brief Capacity in bytes.
                static constexpr std::size_t capacity() { return Capacity; }

            private:
                std::array<char, Capacity> storage_{};
        };

        //! @brief Simple fixed-size pool of fixed-capacity communication buffers.
        //! @details Lock-free via naive busy flag; intended for single-threaded or externally synchronized use.
        template<std::size_t PoolSize, std::size_t Capacity>
        class CommPool
        {
            public:
                //! @brief Acquire a free buffer index.
                //! @return Index [0, PoolSize) on success, or -1 if none available.
                int acquire()
                {
                    for (std::size_t i = 0; i < PoolSize; ++i)
                    {
                        if (!in_use_[i])
                        {
                            in_use_[i] = true;
                            return static_cast<int>(i);
                        }
                    }
                    return -1;
                }

                //! @brief Release a previously acquired buffer index.
                void release(int index)
                {
                    if (index >= 0 && static_cast<std::size_t>(index) < PoolSize)
                    {
                        in_use_[static_cast<std::size_t>(index)] = false;
                    }
                }

                //! @brief Writable span for a given buffer index.
                std::span<char> span(int index)
                {
                    return buffers_[static_cast<std::size_t>(index)].span();
                }

                //! @brief Convenience to get raw pointer for a buffer index.
                char* data(int index)
                {
                    return buffers_[static_cast<std::size_t>(index)].data();
                }

            private:
                std::array<CommBuffer<Capacity>, PoolSize> buffers_{};
                std::array<bool, PoolSize> in_use_{};
        };
    }
}

#endif  // COMMUNICATIONLIBRARY_COMMBUFFER_H_
