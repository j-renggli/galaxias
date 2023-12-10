#pragma once

#include <vulkan/vulkan_core.h>

namespace galaxias
{

class Device
{
public:
    Device(VkPhysicalDevice physical, VkDevice device, VkDeviceSize alignmentBytes);

    VkDevice device() const { return device_; }

    VkDeviceSize alignmentBytes() const { return alignmentBytes_; }

    uint32_t memoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    uint32_t graphicsFamilyQueue() const { return graphicsFamily_; }

private:
    const VkDevice device_;
    const VkDeviceSize alignmentBytes_;

    VkPhysicalDeviceMemoryProperties memProperties_;
    uint32_t graphicsFamily_;
};

} // namespace galaxias
