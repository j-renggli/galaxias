#pragma once

#include <functional>
#include <vulkan/vulkan_core.h>

namespace galaxias
{

class Device;

class CommandPool
{
public:
    CommandPool() = delete;
    CommandPool(const Device& device);
    CommandPool(const CommandPool&) = delete;
    CommandPool(CommandPool&&) noexcept = default;
    ~CommandPool();

    CommandPool& operator=(const CommandPool&) = delete;
    CommandPool& operator=(CommandPool&&) noexcept = default;

    VkCommandPool pool() const { return pool_; }

private:
    VkDevice device_;
    VkCommandPool pool_;
};

class CommandBuffer
{
public:
    CommandBuffer() = delete;
    CommandBuffer(const Device& device);
    CommandBuffer(const CommandBuffer&) = delete;
    CommandBuffer(CommandBuffer&&) noexcept = default;
    virtual ~CommandBuffer() = default;

    CommandBuffer& operator=(const CommandBuffer&) = delete;
    CommandBuffer& operator=(CommandBuffer&&) noexcept = default;

    void submitOnce(const std::function<void(VkCommandBuffer)>& fn, bool synchronise);

public:
    CommandPool pool_;
    VkQueue graphicsQueue_;
    VkCommandBuffer buffer_;
};

} // namespace galaxias
