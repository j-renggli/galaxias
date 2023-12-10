#include <ui/ui.h>

#include "renderer.h"
#include "ui.h"

#include <QGuiApplication>
#include <QLoggingCategory>
#include <QtWidgets/QWidget>

namespace galaxias
{

std::unique_ptr<IUI> IUI::create(int argc, char* argv[]) { return std::make_unique<UI>(argc, argv); }

UI::UI(int argc, char* argv[])
    : argc_{argc}
    , argv_{argv}
{
}

int UI::run()
{
    QGuiApplication app(argc_, argv_);

    QVulkanInstance inst;
    // Add validation layer to get debug information
    inst.setLayers({"VK_LAYER_KHRONOS_validation"});
    if (!inst.create())
    {
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());
    }

    VulkanWindow w;
    w.setVulkanInstance(&inst);
    //    auto devices = w.availablePhysicalDevices();
    //    for (const auto& dev : devices)
    //    {
    //        std::cout << dev.deviceType << std::endl;
    //    }

    w.resize(1024, 768);
    try
    {
        w.show();
    }
    catch (const std::exception& exc)
    {
        const std::string asdf = exc.what();
        //        int a = 0;
    }
    catch (...)
    {
        //        int b = 0;
    }
    //    VkResult vkCreateInstance(
    //        const VkInstanceCreateInfo* pCreateInfo,
    //        const VkAllocationCallbacks* pAllocator,
    //        VkInstance* instance) {

    //        if (pCreateInfo == nullptr || instance == nullptr) {
    //            log("Null pointer passed to required parameter!");
    //            return VK_ERROR_INITIALIZATION_FAILED;
    //        }

    //        return real_vkCreateInstance(pCreateInfo, pAllocator, instance);
    //    }
    return app.exec();
}

} // namespace galaxias
