#include "device.h"

#include <stdexcept>
#include <vector>

namespace galaxias
{

Device::Device(VkPhysicalDevice physical, VkDevice device, VkDeviceSize alignmentBytes)
    : device_{device}
    , alignmentBytes_{alignmentBytes}
    , graphicsFamily_{static_cast<uint32_t>(-1)}
{
    // Memory properties
    vkGetPhysicalDeviceMemoryProperties(physical, &memProperties_);

    // Queue families
    uint32_t queueFamilyCount = 0;
    VkSurfaceKHR surface;
    vkGetPhysicalDeviceQueueFamilyProperties(physical, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physical, &queueFamilyCount, queueFamilies.data());
    for (size_t i = 0; i < queueFamilies.size(); ++i)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphicsFamily_ = i;
        }

        //        VkBool32 presentSupport = false;
        //        ensure(vkGetPhysicalDeviceSurfaceSupportKHR(physical, i, surface, &presentSupport));

        //        if (presentSupport)
        //        {
        //            indices.presentFamily = i;
        //        }

        //        if (indices.isComplete())
        //        {
        //            break;
        //        }
    }
}

// uint32_t Device::graphicsFamilyQueue() const
//{

//    int i = 0;
//    for (const auto& queueFamily : queueFamilies)
//    {
//        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//        {
//            indices.graphicsFamily = i;
//        }

//        VkBool32 presentSupport = false;
//        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

//        if (presentSupport)
//        {
//            indices.presentFamily = i;
//        }

//        if (indices.isComplete())
//        {
//            break;
//        }

//        i++;
//    }
//}

uint32_t Device::memoryType(uint32_t typeFilter, VkMemoryPropertyFlags props) const
{
    for (uint32_t i = 0; i < memProperties_.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties_.memoryTypes[i].propertyFlags & props) == props)
        {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

} // namespace galaxias
