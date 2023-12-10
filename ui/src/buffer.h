#pragma once

#include <span>
#include <vulkan/vulkan_core.h>

namespace galaxias
{

class Device;

class Buffer
{
public:
    Buffer() = delete;
    Buffer(const Device& device, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags props);
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&&) noexcept = default;
    virtual ~Buffer();

    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) noexcept = default;

    VkBuffer buffer() const { return buffer_; }

public:
    static inline VkDeviceSize bytesAlign(VkDeviceSize rawSize, VkDeviceSize bytes)
    {
        return (rawSize + bytes - 1) & ~(bytes - 1);
    }

protected:
    VkDevice device_;
    VkDeviceSize size_;
    VkDeviceSize allocatedSize_;
    VkBuffer buffer_;
    VkDeviceMemory bufferMemory_;
};

class StagingBuffer : public Buffer
{
public:
    StagingBuffer(const Device& device, VkDeviceSize bufferSize);

    void copyFromHost(const std::span<const int8_t>& buffer);
    void copyTo(Buffer& dest);
};

} // namespace galaxias
