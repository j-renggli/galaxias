#pragma once

#include "buffer.h"

#include <memory>
#include <span>

namespace galaxias
{

class CommandBuffer;

class VertexBuffer : public Buffer
{
public:
    VertexBuffer() = delete;
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) noexcept = default;
    ~VertexBuffer() override {}

    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) noexcept = default;

    template <class T>
    static std::unique_ptr<VertexBuffer> create(const Device& device, CommandBuffer& command, const T& container)
    {
        const VkDeviceSize bufferSize = sizeof(typename T::value_type) * container.size();
        return createFromSpan(
            device, command, std::span<const int8_t>{reinterpret_cast<const int8_t*>(container.data()), bufferSize});
    }

private:
    VertexBuffer(const Device& device, VkDeviceSize bufferSize);

    static std::unique_ptr<VertexBuffer>
    createFromSpan(const Device& device, CommandBuffer& command, const std::span<const int8_t>& data);
};

} // namespace galaxias
