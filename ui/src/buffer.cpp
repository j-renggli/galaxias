#include "buffer.h"

#include "device.h"
#include "utils.h"

#include <cstring>
#include <stdexcept>

namespace galaxias
{

Buffer::Buffer(const Device& device, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags props)
    : device_{device.device()}
    , size_{bufferSize}
    , allocatedSize_{bytesAlign(bufferSize, device.alignmentBytes())}
    , buffer_{nullptr}
{
    VkBufferCreateInfo bufferInfo;
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size_;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    ensure(vkCreateBuffer(device_, &bufferInfo, nullptr, &buffer_), "Create buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device_, buffer_, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device.memoryType(memRequirements.memoryTypeBits, props);

    if (vkAllocateMemory(device_, &allocInfo, nullptr, &bufferMemory_) != VK_SUCCESS)
    {
        vkDestroyBuffer(device_, buffer_, nullptr);
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device_, buffer_, bufferMemory_, 0);
}

Buffer::~Buffer()
{
    vkDestroyBuffer(device_, buffer_, nullptr);
    vkFreeMemory(device_, bufferMemory_, nullptr);
}

StagingBuffer::StagingBuffer(const Device& device, VkDeviceSize bufferSize)
    : Buffer{device,
             bufferSize,
             VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}
{
}

void StagingBuffer::copyFromHost(const std::span<const int8_t>& buffer)
{
    if (buffer.size() > allocatedSize_)
    {
        throw std::runtime_error("Too much data to copy");
    }

    void* data;
    vkMapMemory(device_, bufferMemory_, 0, size_, 0, &data);
    std::memcpy(data, buffer.data(), buffer.size());
    vkUnmapMemory(device_, bufferMemory_);
    size_ = buffer.size();
}

} // namespace galaxias
