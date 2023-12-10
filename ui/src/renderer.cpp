#include "renderer.h"

#include "command_buffer.h"
#include "device.h"
#include "utils.h"
#include "vertex_buffer.h"
#include <core/files.h>

#include <QVulkanFunctions>

#include <vulkan/vulkan.h>

#include <iostream>
#include <span>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace galaxias
{

namespace
{

// clang-format off
static float vertexData[] = { // Y up, front = CCW
 0.0f,  0.5f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.0f, 0.0f,
 1.0f,  0.0f,  0.5f, -0.5f,
 0.0f, 0.0f,  1.0f
};
// clang-format on

struct Vertex
{
    std::array<float, 3> pos;
    std::array<float, 3> col;
};

// Uniform is a single 4x4 matrix
static const int UNIFORM_DATA_SIZE = 4 * 4 * sizeof(float);

static inline VkDeviceSize realign(VkDeviceSize v, VkDeviceSize byteAlign)
{
    return (v + byteAlign - 1) & ~(byteAlign - 1);
}

void createShaderModule(const View1DArray<uint32_t>& file,
                        QVulkanWindow* window,
                        QVulkanDeviceFunctions* deviceFn,
                        VkShaderModule& module)
{
    VkShaderModuleCreateInfo shaderInfo;
    memset(&shaderInfo, 0, sizeof(shaderInfo));
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = file.bytes();
    shaderInfo.pCode = file.data();
    Q_PROPERTY(type name READ name WRITE setName NOTIFY nameChanged)
    VkResult err = deviceFn->vkCreateShaderModule(window->device(), &shaderInfo, nullptr, &module);
    if (err != VK_SUCCESS)
    {
        throw std::runtime_error("FAILED");
    }
}

} // namespace

QVulkanWindowRenderer* VulkanWindow::createRenderer() { return new Renderer(this); }

////////////////////////////////////////////////////////////////

Renderer::BufferInfo::BufferInfo(VkDeviceSize vertexSize, VkDeviceSize uniformSize, VkDeviceSize alignmentSize)
    : vertexAllocSize{realign(vertexSize, alignmentSize)}
    , uniformAllocSize{realign(uniformSize, alignmentSize)}
{
}

////////////////////////////////////////////////////////////////

Renderer::Renderer(QVulkanWindow* window)
    : window_{window}
    , deviceMemory_{nullptr}
    , buffer_{nullptr}
    , descriptorPool_{nullptr}
    , descriptorSetLayout_{nullptr}
    , pipelineCache_{nullptr}
    , pipelineLayout_{nullptr}
    , pipeline_{nullptr}
{
    const QList<int> counts = window->supportedSampleCounts();
    qDebug() << "Supported sample counts:" << counts;
    for (int s = 16; s >= 4; s /= 2)
    {
        if (counts.contains(s))
        {
            qDebug("Requesting sample count %d", s);
            window->setSampleCount(s);
            break;
        }
    }
}

Renderer::~Renderer() = default;

void Renderer::initResources()
{
    device_ = window_->device();
    deviceFn_ = window_->vulkanInstance()->deviceFunctions(device_);

    physical_ = std::make_unique<Device>(window_->physicalDevice(),
                                         device_,
                                         window_->physicalDeviceProperties()->limits.minUniformBufferOffsetAlignment);
    commandBuffer_ = std::make_unique<CommandBuffer>(*physical_);

    VkResult err;
    const int concurrentFrameCount = window_->concurrentFrameCount();

    createVertexBuffer();
    createIndexBuffer();
    // clang-format off
    const std::vector<uint16_t> indices = {0, 1, 2, 2, 1, 3, 3, 1, 4, 4, 1, 0};
    // clang-format on

    setupDescriptor();
    {
        BufferInfo info{sizeof(vertexData),
                        UNIFORM_DATA_SIZE,
                        window_->physicalDeviceProperties()->limits.minUniformBufferOffsetAlignment};
        createBuffer(info);

        uint8_t* ptrMem;
        constexpr VkDeviceSize offsetVertex{0};
        constexpr VkMemoryMapFlags flags{0};
        err = deviceFn_->vkMapMemory(
            device_, deviceMemory_, offsetVertex, info.memReq.size, flags, reinterpret_cast<void**>(&ptrMem));
        if (err != VK_SUCCESS)
        {
            qFatal("Failed to map memory: %d", err);
        }

        // Copy vertices
        memcpy(ptrMem, vertexData, sizeof(vertexData));

        // Copy uniforms
        QMatrix4x4 ident;
        memset(uniformBufferInfo_, 0, sizeof(uniformBufferInfo_));
        for (int i = 0; i < concurrentFrameCount; ++i)
        {
            const VkDeviceSize offset = info.vertexAllocSize + i * info.uniformAllocSize;
            memcpy(ptrMem + offset, ident.constData(), 16 * sizeof(float));
            uniformBufferInfo_[i].buffer = buffer_;
            uniformBufferInfo_[i].offset = offset;
            uniformBufferInfo_[i].range = info.uniformAllocSize;
        }
        deviceFn_->vkUnmapMemory(device_, deviceMemory_);
    }

    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        VkDescriptorSetAllocateInfo descSetAllocInfo = {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, nullptr, descriptorPool_, 1, &descriptorSetLayout_};
        err = deviceFn_->vkAllocateDescriptorSets(device_, &descSetAllocInfo, &descriptorSet_[i]);
        if (err != VK_SUCCESS)
        {
            qFatal("Failed to allocate descriptor set: %d", err);
        }

        VkWriteDescriptorSet descWrite;
        memset(&descWrite, 0, sizeof(descWrite));
        descWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite.dstSet = descriptorSet_[i];
        descWrite.descriptorCount = 1;
        descWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descWrite.pBufferInfo = &uniformBufferInfo_[i];
        deviceFn_->vkUpdateDescriptorSets(device_, 1, &descWrite, 0, nullptr);
    }

    createPipelineCache();
    createPipelineLayout();

    std::array<VkShaderModule, 2> vertexAndFragmentModules;
    {
        // Graphics pipeline
        VkGraphicsPipelineCreateInfo pipelineInfo;
        memset(&pipelineInfo, 0, sizeof(pipelineInfo));
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        {
            auto vertex = FilesManager::loadShader(FilesManager::ShaderType::DirectVertex);
            auto fragment = FilesManager::loadShader(FilesManager::ShaderType::DirectFragment);
            createShaderModule(vertex, window_, deviceFn_, vertexAndFragmentModules[0]);
            createShaderModule(fragment, window_, deviceFn_, vertexAndFragmentModules[1]);
        }

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {
            {{VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              nullptr,
              0,
              VK_SHADER_STAGE_VERTEX_BIT,
              vertexAndFragmentModules[0],
              "main",
              nullptr},
             {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
              nullptr,
              0,
              VK_SHADER_STAGE_FRAGMENT_BIT,
              vertexAndFragmentModules[1],
              "main",
              nullptr}}};

        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo;
        {
            VkVertexInputBindingDescription vertexBindingDesc = {0, // binding
                                                                 5 * sizeof(float),
                                                                 VK_VERTEX_INPUT_RATE_VERTEX};
            VkVertexInputAttributeDescription vertexAttrDesc[] = {{   // position
                                                                   0, // location
                                                                   0, // binding
                                                                   VK_FORMAT_R32G32_SFLOAT,
                                                                   0},
                                                                  {// color
                                                                   1,
                                                                   0,
                                                                   VK_FORMAT_R32G32B32_SFLOAT,
                                                                   2 * sizeof(float)}};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.pNext = nullptr;
            vertexInputInfo.flags = 0;
            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDesc;
            vertexInputInfo.vertexAttributeDescriptionCount = 2;
            vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;
        }

        pipelineInfo.pVertexInputState = &vertexInputInfo;

        VkPipelineInputAssemblyStateCreateInfo ia;
        memset(&ia, 0, sizeof(ia));
        ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipelineInfo.pInputAssemblyState = &ia;

        // The viewport and scissor will be set dynamically via vkCmdSetViewport/Scissor.
        // This way the pipeline does not need to be touched when resizing the window.
        VkPipelineViewportStateCreateInfo vp;
        memset(&vp, 0, sizeof(vp));
        vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        vp.viewportCount = 1;
        vp.scissorCount = 1;
        pipelineInfo.pViewportState = &vp;

        VkPipelineRasterizationStateCreateInfo rs;
        memset(&rs, 0, sizeof(rs));
        rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rs.polygonMode = VK_POLYGON_MODE_LINE; // VK_POLYGON_MODE_FILL;
        rs.cullMode = VK_CULL_MODE_NONE;       // we want the back face as well
        rs.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rs.lineWidth = 1.0f;
        pipelineInfo.pRasterizationState = &rs;

        VkPipelineMultisampleStateCreateInfo ms;
        memset(&ms, 0, sizeof(ms));
        ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        // Enable multisampling.
        ms.rasterizationSamples = window_->sampleCountFlagBits();
        pipelineInfo.pMultisampleState = &ms;

        VkPipelineDepthStencilStateCreateInfo ds;
        memset(&ds, 0, sizeof(ds));
        ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        ds.depthTestEnable = VK_TRUE;
        ds.depthWriteEnable = VK_TRUE;
        ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        pipelineInfo.pDepthStencilState = &ds;

        VkPipelineColorBlendStateCreateInfo cb;
        memset(&cb, 0, sizeof(cb));
        cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        // no blend, write out all of rgba
        VkPipelineColorBlendAttachmentState att;
        memset(&att, 0, sizeof(att));
        att.colorWriteMask = 0xF;
        cb.attachmentCount = 1;
        cb.pAttachments = &att;
        pipelineInfo.pColorBlendState = &cb;

        VkDynamicState dynEnable[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dyn;
        memset(&dyn, 0, sizeof(dyn));
        dyn.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dyn.dynamicStateCount = sizeof(dynEnable) / sizeof(VkDynamicState);
        dyn.pDynamicStates = dynEnable;
        pipelineInfo.pDynamicState = &dyn;

        pipelineInfo.layout = pipelineLayout_;
        pipelineInfo.renderPass = window_->defaultRenderPass();

        err = deviceFn_->vkCreateGraphicsPipelines(device_, pipelineCache_, 1, &pipelineInfo, nullptr, &pipeline_);
        if (err != VK_SUCCESS)
        {
            qFatal("Failed to create graphics pipeline: %d", err);
        }
    }

    for (auto& module : vertexAndFragmentModules)
    {
        if (module)
        {
            deviceFn_->vkDestroyShaderModule(device_, module, nullptr);
        }
    }
}

void Renderer::createBuffer(BufferInfo& info)
{
    VkBufferCreateInfo bufInfo;
    memset(&bufInfo, 0, sizeof(bufInfo));
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    // Our internal layout is vertex, uniform, uniform, ... with each uniform buffer start offset aligned to
    // uniAlign.
    // We will have size of vertices (mem aligned) + N * uniform size (mem aligned), where N is concurrent
    // frame count
    bufInfo.size = info.vertexAllocSize + window_->concurrentFrameCount() * info.uniformAllocSize;
    bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    ensure(deviceFn_->vkCreateBuffer(device_, &bufInfo, nullptr, &buffer_), "Create buffer");

    deviceFn_->vkGetBufferMemoryRequirements(device_, buffer_, &info.memReq);

    VkMemoryAllocateInfo memAllocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, nullptr, info.memReq.size, window_->hostVisibleMemoryIndex()};

    ensure(deviceFn_->vkAllocateMemory(device_, &memAllocInfo, nullptr, &deviceMemory_), "Allocate buffer memory");

    ensure(deviceFn_->vkBindBufferMemory(device_, buffer_, deviceMemory_, 0), "Bind buffer memory");
}

void Renderer::setupDescriptor()
{
    VkDescriptorPoolCreateInfo descPoolInfo;
    memset(&descPoolInfo, 0, sizeof(descPoolInfo));
    descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descPoolInfo.maxSets = window_->concurrentFrameCount();
    descPoolInfo.poolSizeCount = 1;
    VkDescriptorPoolSize descPoolSizes = {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                          static_cast<uint32_t>(window_->concurrentFrameCount())};
    descPoolInfo.pPoolSizes = &descPoolSizes;
    ensure(deviceFn_->vkCreateDescriptorPool(device_, &descPoolInfo, nullptr, &descriptorPool_),
           "Create descriptor pool");

    VkDescriptorSetLayoutBinding layoutBinding = {0, // binding
                                                  VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                  1,
                                                  VK_SHADER_STAGE_VERTEX_BIT,
                                                  nullptr};
    VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, nullptr, 0, 1, &layoutBinding};
    ensure(deviceFn_->vkCreateDescriptorSetLayout(device_, &descLayoutInfo, nullptr, &descriptorSetLayout_),
           "Create descriptor set layout");
}

void Renderer::createPipelineCache()
{
    VkPipelineCacheCreateInfo pipelineCacheInfo;
    memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    ensure(deviceFn_->vkCreatePipelineCache(device_, &pipelineCacheInfo, nullptr, &pipelineCache_),
           "Create pipeline cache");
}

void Renderer::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_;
    ensure(deviceFn_->vkCreatePipelineLayout(device_, &pipelineLayoutInfo, nullptr, &pipelineLayout_),
           "Create pipeline layout");
}

void Renderer::createVertexBuffer()
{
    const std::vector<Vertex> vertices{
        {{-1.f, 0.f, 0.f}, {0.0f, 0.5f, 0.5f}},
        {{0.f, 0.f, 1.f}, {0.5f, 0.5f, 1.0f}},
        {{0.f, 1.f, 0.f}, {0.5f, 1.0f, 0.5f}},
        {{1.f, 0.f, 0.f}, {1.0f, 0.5f, 0.5f}},
        {{0.f, -1.f, 0.f}, {0.5f, 0.0f, 0.5f}},
    };

    try
    {
        auto buffer = VertexBuffer::create(*physical_, *commandBuffer_, std::span(vertices));
    }
    catch (const std::exception& exc)
    {
        std::cout << exc.what() << std::endl;
        throw;
    }

    //    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    //    VkBuffer stagingBuffer;
    //    VkDeviceMemory stagingBufferMemory;
    //    createBuffer(bufferSize,
    //                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    //                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    //                 stagingBuffer,
    //                 stagingBufferMemory);

    //    void* data;
    //    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    //    memcpy(data, vertices.data(), (size_t)bufferSize);
    //    vkUnmapMemory(device, stagingBufferMemory);

    //    createBuffer(bufferSize,
    //                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    //                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    //                 vertexBuffer,
    //                 vertexBufferMemory);
}

void Renderer::createIndexBuffer() {}

void Renderer::initSwapChainResources()
{
    // Projection matrix
    projectionMatrix_ = window_->clipCorrectionMatrix(); // adjust for Vulkan-OpenGL clip space differences
    const QSize sz = window_->swapChainImageSize();
    constexpr float verticalAngle{45.f};
    constexpr float nearPlane{0.01f};
    constexpr float farPlane{100.f};
    const float aspectRatio = static_cast<float>(sz.width()) / static_cast<float>(sz.height());
    projectionMatrix_.perspective(verticalAngle, aspectRatio, nearPlane, farPlane);
    projectionMatrix_.translate(0.f, 0.f, -5.5f);
}

void Renderer::releaseSwapChainResources() {}

void Renderer::releaseResources()
{
    if (pipeline_)
    {
        deviceFn_->vkDestroyPipeline(device_, pipeline_, nullptr);
        pipeline_ = nullptr;
    }

    if (pipelineLayout_)
    {
        deviceFn_->vkDestroyPipelineLayout(device_, pipelineLayout_, nullptr);
        pipelineLayout_ = nullptr;
    }

    if (pipelineCache_)
    {
        deviceFn_->vkDestroyPipelineCache(device_, pipelineCache_, nullptr);
        pipelineCache_ = nullptr;
    }

    if (descriptorSetLayout_)
    {
        deviceFn_->vkDestroyDescriptorSetLayout(device_, descriptorSetLayout_, nullptr);
        descriptorSetLayout_ = nullptr;
    }

    if (descriptorPool_)
    {
        deviceFn_->vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);
        descriptorPool_ = nullptr;
    }

    if (buffer_)
    {
        deviceFn_->vkDestroyBuffer(device_, buffer_, nullptr);
        buffer_ = nullptr;
    }

    if (deviceMemory_)
    {
        deviceFn_->vkFreeMemory(device_, deviceMemory_, nullptr);
        deviceMemory_ = nullptr;
    }
}

void Renderer::startNextFrame()
{
    try
    {
        auto cmdBuf = beginRenderPass();
        {
            quint8* ptrMem;
            ensure(deviceFn_->vkMapMemory(device_,
                                          deviceMemory_,
                                          uniformBufferInfo_[window_->currentFrame()].offset,
                                          UNIFORM_DATA_SIZE,
                                          0,
                                          reinterpret_cast<void**>(&ptrMem)),
                   "Map memory uniform");

            // Not exactly a real animation system, just advance on every frame for now.
            m_rotation += 1.75f;

            QMatrix4x4 m = projectionMatrix_;
            m.rotate(m_rotation, 0, 0, 1);
            memcpy(ptrMem, m.constData(), 16 * sizeof(float));

            deviceFn_->vkUnmapMemory(device_, deviceMemory_);
        }

        deviceFn_->vkCmdBindPipeline(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);
        deviceFn_->vkCmdBindDescriptorSets(cmdBuf,
                                           VK_PIPELINE_BIND_POINT_GRAPHICS,
                                           pipelineLayout_,
                                           0,
                                           1,
                                           &descriptorSet_[window_->currentFrame()],
                                           0,
                                           nullptr);
        VkDeviceSize vbOffset = 0;
        deviceFn_->vkCmdBindVertexBuffers(cmdBuf, 0, 1, &buffer_, &vbOffset);

        setViewport(cmdBuf);

        deviceFn_->vkCmdDraw(cmdBuf, 3, 1, 0, 0);

        endRenderPass(cmdBuf);

        window_->frameReady();
        window_->requestUpdate();
    }
    catch (const std::runtime_error& exc)
    {
        std::cerr << "Exception caught during frame rendering: " << exc.what() << std::endl;
    }
}

VkCommandBuffer Renderer::beginRenderPass()
{
    m_green += 0.05f;

    VkClearColorValue clearColor = {{0.0f, 0.05f * (static_cast<float>(sin(m_green)) + 1.f), 0.0f, 1.0f}};
    VkClearDepthStencilValue clearDS = {1.0f, 0};
    constexpr size_t numClearColors{3};
    VkClearValue clearValues[numClearColors];
    memset(clearValues, 0, sizeof(clearValues));
    clearValues[0].color = clearColor;
    clearValues[1].depthStencil = clearDS;
    clearValues[2].color = clearColor;

    VkRenderPassBeginInfo rpBeginInfo;
    memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
    rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBeginInfo.renderPass = window_->defaultRenderPass();
    rpBeginInfo.framebuffer = window_->currentFramebuffer();
    const QSize sz = window_->swapChainImageSize();
    rpBeginInfo.renderArea.extent.width = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();
    rpBeginInfo.clearValueCount = numClearColors;
    rpBeginInfo.pClearValues = clearValues;

    VkCommandBuffer cmdBuf = window_->currentCommandBuffer();
    deviceFn_->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    return cmdBuf;
}

void Renderer::setViewport(const VkCommandBuffer& cmdBuf)
{
    const QSize sz = window_->swapChainImageSize();
    VkViewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = sz.width();
    viewport.height = sz.height();
    viewport.minDepth = 0;
    viewport.maxDepth = 1;
    deviceFn_->vkCmdSetViewport(cmdBuf, 0, 1, &viewport);

    VkRect2D scissor;
    scissor.offset.x = 0;
    scissor.offset.y = 0;
    scissor.extent.width = viewport.width;
    scissor.extent.height = viewport.height;
    deviceFn_->vkCmdSetScissor(cmdBuf, 0, 1, &scissor);
}

void Renderer::endRenderPass(const VkCommandBuffer& cmdBuf) { deviceFn_->vkCmdEndRenderPass(cmdBuf); }

} // namespace galaxias
