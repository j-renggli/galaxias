#pragma once

#include <stdexcept>
#include <string>
#include <vulkan/vulkan_core.h>

namespace galaxias
{

inline void ensure(const VkResult& result, const std::string& message)
{
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error(message + " failed with result " + std::to_string(result));
    }
}

} // namespace galaxias
