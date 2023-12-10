#pragma once

#include <QVulkanWindowRenderer>

namespace galaxias
{

class CommandBuffer;
class Device;
class VertexBuffer;

class Renderer : public QVulkanWindowRenderer
{
public:
    Renderer(QVulkanWindow* window);
    ~Renderer();

    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;

    void startNextFrame() override;

private:
    //    bool check(VkResult result, const std::string& msg) const;
    //    void ensure(VkResult result, const std::string& msg) const;

    struct BufferInfo
    {
        BufferInfo(VkDeviceSize vertexSize, VkDeviceSize uniformSize, VkDeviceSize alignmentSize);

        VkMemoryRequirements memReq;
        VkDeviceSize vertexAllocSize;
        VkDeviceSize uniformAllocSize;
    };

    void createBuffer(BufferInfo& info);
    void setupDescriptor();
    void createPipelineCache();
    void createPipelineLayout();
    void createVertexBuffer();
    void createIndexBuffer();

    VkCommandBuffer beginRenderPass();
    void setViewport(const VkCommandBuffer& cmdBuf);
    void endRenderPass(const VkCommandBuffer& cmdBuf);

private:
    QVulkanWindow* window_;
    VkDevice device_;
    QVulkanDeviceFunctions* deviceFn_;

    std::unique_ptr<Device> physical_;
    std::unique_ptr<CommandBuffer> commandBuffer_;
    std::unique_ptr<VertexBuffer> vertexBuffer_;

    VkDeviceMemory deviceMemory_;
    VkBuffer buffer_;

    VkDescriptorBufferInfo uniformBufferInfo_[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];
    VkDescriptorPool descriptorPool_;
    VkDescriptorSetLayout descriptorSetLayout_;
    VkDescriptorSet descriptorSet_[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkPipelineCache pipelineCache_;
    VkPipelineLayout pipelineLayout_;
    VkPipeline pipeline_;

    float m_green = 0.f;
    float m_rotation = 0.f;

    QMatrix4x4 projectionMatrix_;
};

// VkDescriptorBufferInfo m_uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

// VkDescriptorSetLayout m_descSetLayout = VK_NULL_HANDLE;
// VkDescriptorSet m_descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

// VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;
// VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
// VkPipeline m_pipeline = VK_NULL_HANDLE;

// float m_rotation = 0.0f;

class VulkanWindow : public QVulkanWindow
{
public:
    QVulkanWindowRenderer* createRenderer() override;
};

} // namespace galaxias
