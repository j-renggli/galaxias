#include "command_buffer.h"

#include "device.h"
#include "utils.h"

namespace galaxias
{

CommandPool::CommandPool(const Device& device)
    : device_{device.device()}
{
    //    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = device.graphicsFamilyQueue();

    ensure(vkCreateCommandPool(device_, &poolInfo, nullptr, &pool_), "Create graphics command pool");
}

CommandPool::~CommandPool() { vkDestroyCommandPool(device_, pool_, nullptr); }

CommandBuffer::CommandBuffer(const Device& device)
    : pool_{device}
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = pool_.pool();
    allocInfo.commandBufferCount = 1;

    ensure(vkAllocateCommandBuffers(device.device(), &allocInfo, &buffer_), "Allocate command buffers");

    vkGetDeviceQueue(device.device(), device.graphicsFamilyQueue(), 0, &graphicsQueue_);
}

void CommandBuffer::submitOnce(const std::function<void(VkCommandBuffer)>& fn, bool synchronise)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    ensure(vkBeginCommandBuffer(buffer_, &beginInfo), "Begin command buffer");

    fn(buffer_);

    ensure(vkEndCommandBuffer(buffer_), "End command buffer");

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &buffer_;

    ensure(vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE), "Submit queue");

    if (synchronise)
    {
        ensure(vkQueueWaitIdle(graphicsQueue_), "Wait for queue");
    }
}

} // namespace galaxias
