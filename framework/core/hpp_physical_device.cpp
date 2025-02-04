#include "hpp_physical_device.h"
#include "spdlog/spdlog.h"

namespace vkb
{
	HPPPhysicalDevice::HPPPhysicalDevice(HPPInstance& instance, vk::PhysicalDevice physical_device) :
		instance(instance),
		handle(physical_device)
	{
		features = physical_device.getFeatures();
		properties = physical_device.getProperties();
		memory_properties = physical_device.getMemoryProperties();

		spdlog::info("Found GPU: {}", properties.deviceName.data());

		queue_family_properties = physical_device.getQueueFamilyProperties();

		device_extensions = physical_device.enumerateDeviceExtensionProperties();

		if (device_extensions.size() > 0)
		{
			spdlog::info("HPPDevice supports the following extensions:");
			for (auto& extension : device_extensions)
			{
				spdlog::info("  \t{}", extension.extensionName.data());
			}
		}
	}

	bool HPPPhysicalDevice::is_extension_supported(const std::string& requested_extension) const
	{
		return std::find_if(device_extensions.begin(), device_extensions.end(),
			[requested_extension](auto& device_extension) {
				return std::strcmp(device_extension.extensionName, requested_extension.c_str()) == 0;
			}) != device_extensions.end();
	}
}