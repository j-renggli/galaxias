#include "vertex_buffer.h"

#include "command_buffer.h"
#include "device.h"

#include <cstring>
#include <span>
#include <stdexcept>

namespace galaxias
{

VertexBuffer::VertexBuffer(const Device& device, VkDeviceSize bufferSize)
    : Buffer{device,
             bufferSize,
             /*VK_BUFFER_USAGE_TRANSFER_DST_BIT | */ VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT}
{
}

std::unique_ptr<VertexBuffer>
VertexBuffer::createFromSpan(const Device& device, CommandBuffer& command, const std::span<const int8_t>& data)
{
    // Staging part
    StagingBuffer staging{device, data.size()};

    staging.copyFromHost(data);

    // Device buffer
    std::unique_ptr<VertexBuffer> buffer{new VertexBuffer{device, data.size()}};
    command.submitOnce(
        [&staging, &buffer, size = data.size()](VkCommandBuffer commandBuffer)
        {
            VkBufferCopy copyRegion{};
            copyRegion.size = size;
            vkCmdCopyBuffer(commandBuffer, staging.buffer(), buffer->buffer(), 1, &copyRegion);
        },
        true);

    return buffer;
}

} // namespace galaxias
