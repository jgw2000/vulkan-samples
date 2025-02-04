#include "hpp_device.h"
#include "spdlog/spdlog.h"

namespace vkb
{
	HPPDevice::HPPDevice(vkb::HPPPhysicalDevice& gpu, vk::SurfaceKHR surface,
						 std::unordered_map<const char*, bool> requested_extensions) :
		VulkanResource(nullptr, this),
		gpu(gpu)
	{
		// Prepare the device queues
		auto queue_family_properties = gpu.get_queue_family_properties();
		std::vector<vk::DeviceQueueCreateInfo> queue_create_infos(queue_family_properties.size());
		std::vector<std::vector<float>> queue_priorities(queue_family_properties.size());

		for (uint32_t queue_family_index = 0U; queue_family_index < queue_family_properties.size(); ++queue_family_index)
		{
			const auto& queue_family_property = queue_family_properties[queue_family_index];

			if (gpu.has_high_priority_graphics_queue())
			{
				uint32_t graphics_queue_family = get_queue_family_index(vk::QueueFlagBits::eGraphics);
				if (graphics_queue_family == queue_family_index)
				{
					queue_priorities[queue_family_index].reserve(queue_family_property.queueCount);
					queue_priorities[queue_family_index].push_back(1.0f);
					for (uint32_t i = 1; i < queue_family_property.queueCount; i++)
					{
						queue_priorities[queue_family_index].push_back(0.5f);
					}
				}
				else
				{
					queue_priorities[queue_family_index].resize(queue_family_property.queueCount, 0.5f);
				}
			}
			else
			{
				queue_priorities[queue_family_index].resize(queue_family_property.queueCount, 0.5f);
			}

			vk::DeviceQueueCreateInfo& queue_create_info = queue_create_infos[queue_family_index];

			queue_create_info.queueFamilyIndex = queue_family_index;
			queue_create_info.queueCount = queue_family_property.queueCount;
			queue_create_info.pQueuePriorities = queue_priorities[queue_family_index].data();
		}

		// Check that extensions are supported before trying to create the device
		std::vector<const char*> unsupported_extensions{};
		for (auto& extension : requested_extensions)
		{
			if (is_extension_supported(extension.first))
			{
				enabled_extensions.emplace_back(extension.first);
			}
			else
			{
				unsupported_extensions.emplace_back(extension.first);
			}
		}

		if (enabled_extensions.size() > 0)
		{
			spdlog::info("HPPDevice supports the following requested extensions:");
			for (auto& extension : enabled_extensions)
			{
				spdlog::info("  \t{}", extension);
			}
		}

		if (unsupported_extensions.size() > 0)
		{
			auto error = false;
			for (auto& extension : unsupported_extensions)
			{
				auto extension_is_optional = requested_extensions[extension];
				if (extension_is_optional)
				{
					spdlog::warn("Optional device extension {} not available, some features may be disabled", extension);
				}
				else
				{
					spdlog::warn("Required device extension {} not available, cannot run", extension);
					error = true;
				}
			}

			if (error)
			{
				throw std::runtime_error("Device extension not present");
			}
		}

		vk::DeviceCreateInfo create_info({}, queue_create_infos, {}, enabled_extensions, &gpu.get_mutable_requested_features());
		create_info.pNext = gpu.get_extension_feature_chain();

		set_handle(gpu.get_handle().createDevice(create_info));
	}

	HPPDevice::~HPPDevice()
	{
		if (get_handle())
		{
			get_handle().destroy();
		}
	}

	uint32_t HPPDevice::get_queue_family_index(vk::QueueFlagBits queue_flag) const
	{
		const auto& queue_family_properties = gpu.get_queue_family_properties();

		// Dedicated queue for compute
		// Try to find a queue family index that supports compute but not graphics
		if (queue_flag & vk::QueueFlagBits::eCompute)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i)
			{
				if ((queue_family_properties[i].queueFlags & queue_flag) && !(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics))
				{
					return i;
				}
			}
		}

		// Dedicated queue for transfer
		// Try to find a queue family index that supports transfer but not graphics and compute
		if (queue_flag & vk::QueueFlagBits::eTransfer)
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i)
			{
				if ((queue_family_properties[i].queueFlags & queue_flag) && !(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
					!(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eCompute))
				{
					return i;
				}
			}
		}

		// For other queue types or if no separate compute queue is present, return the first one to support the requested
		// flags
		for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i)
		{
			if (queue_family_properties[i].queueFlags & queue_flag)
			{
				return i;
			}
		}

		throw std::runtime_error("Could not find a matching queue family index");
	}

	bool HPPDevice::is_enabled(const std::string& extension) const
	{
		return std::find_if(enabled_extensions.begin(), enabled_extensions.end(),
			[extension](const char* enabled_extension) {
				return extension == enabled_extension;
			}) != enabled_extensions.end();
	}
}