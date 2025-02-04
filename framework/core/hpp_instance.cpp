#include "hpp_instance.h"
#include "hpp_physical_device.h"
#include "spdlog/spdlog.h"

namespace vkb
{
	namespace
	{
#ifdef _DEBUG
		VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, VkDebugUtilsMessageTypeFlagsEXT message_type,
																	  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
																	  void* user_data)
		{
			if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			{
				spdlog::warn("{} - {}: {}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);
			}
			else if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
			{
				spdlog::error("{} - {}: {}", callback_data->messageIdNumber, callback_data->pMessageIdName, callback_data->pMessage);
			}
			return VK_FALSE;
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT /*type*/,
															 uint64_t /*object*/, size_t /*location*/, int32_t /*message_code*/,
															 const char* layer_prefix, const char* message, void* /*user_data*/)
		{
			if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
			{
				spdlog::error("{}: {}", layer_prefix, message);
			}
			else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
			{
				spdlog::warn("{}: {}", layer_prefix, message);
			}
			else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
			{
				spdlog::warn("{}: {}", layer_prefix, message);
			}
			else
			{
				spdlog::info("{}: {}", layer_prefix, message);
			}
			return VK_FALSE;
		}
#endif

		bool enable_extension(const char* required_ext_name,
							  const std::vector<vk::ExtensionProperties>& available_exts,
							  std::vector<const char*>& enabled_extensions)
		{
			for (auto& avail_ext_it : available_exts)
			{
				if (strcmp(avail_ext_it.extensionName, required_ext_name) == 0)
				{
					auto it = std::find_if(enabled_extensions.begin(), enabled_extensions.end(),
										  [required_ext_name](const char* enabled_ext_name) {
											  return strcmp(enabled_ext_name, required_ext_name) == 0;
										  });
					if (it != enabled_extensions.end())
					{
						// Extension is already enabled
					}
					else
					{
						spdlog::info("Extension {} found, enabling it", required_ext_name);
						enabled_extensions.emplace_back(required_ext_name);
					}
					return true;
				}
			}

			spdlog::info("Extension {} not found");
			return false;
		}

		bool validate_layers(const std::vector<const char*>& required,
							 const std::vector<vk::LayerProperties>& available)
		{
			for (auto layer : required)
			{
				bool found = false;
				for (auto& available_layer : available)
				{
					if (strcmp(available_layer.layerName, layer) == 0)
					{
						found = true;
						break;
					}
				}

				if (!found)
				{
					spdlog::info("Validation layer {} not found", layer);
					return false;
				}
			}

			return true;
		}
	}

	HPPInstance::HPPInstance(const std::string& application_name,
							 const std::unordered_map<const char*, bool>& required_extensions,
							 const std::vector<const char*>& required_validation_layers,
							 uint32_t api_version)
	{
		// Check and decide instance extensions
		std::vector<vk::ExtensionProperties> available_instance_extensions = vk::enumerateInstanceExtensionProperties();

#ifdef _DEBUG
		// Check if VK_EXT_debug_utils is supported, which supersedes VK_EXT_Debug_Report
		bool has_debug_utils = enable_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, available_instance_extensions, enabled_extensions);
		bool has_debug_report = false;

		if (!has_debug_utils)
		{
			has_debug_report = enable_extension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME, available_instance_extensions, enabled_extensions);
			if (!has_debug_report)
			{
				spdlog::warn("Neither of {} or {} are available; disabling debug reporting",
					VK_EXT_DEBUG_UTILS_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			}
		}
#endif

		auto extension_error = false;
		for (auto extension : required_extensions)
		{
			auto extension_name = extension.first;
			auto extension_is_optional = extension.second;
			if (!enable_extension(extension_name, available_instance_extensions, enabled_extensions))
			{
				if (!extension_is_optional)
				{
					extension_error = true;
				}
			}
		}

		if (extension_error)
		{
			throw std::runtime_error("Required instance extensions are missing.");
		}

		// Check and decide validation layers
		std::vector<vk::LayerProperties> supported_validation_layers = vk::enumerateInstanceLayerProperties();
		std::vector<const char*> requested_validation_layers(required_validation_layers);

#ifdef _DEBUG
		requested_validation_layers.emplace_back("VK_LAYER_KHRONOS_validation");
#endif

		if (validate_layers(requested_validation_layers, supported_validation_layers))
		{
			spdlog::info("Enabled Validation Layers:");
			for (const auto& layer : requested_validation_layers)
			{
				spdlog::info("  \t{}", layer);
			}
		}
		else
		{
			throw std::runtime_error("Required validation layers are missing.");
		}

		// Create instance

		vk::ApplicationInfo app_info(application_name.c_str(), 0, "Vulkan Samples", 0, api_version);
		vk::InstanceCreateInfo instance_info({}, &app_info, requested_validation_layers, enabled_extensions);

#ifdef _DEBUG
		vk::DebugUtilsMessengerCreateInfoEXT debug_utils_create_info;
		vk::DebugReportCallbackCreateInfoEXT debug_report_create_info;
		if (has_debug_utils)
		{
			debug_utils_create_info =
				vk::DebugUtilsMessengerCreateInfoEXT({},
													 vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
													 vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
													 debug_utils_messenger_callback);

			instance_info.pNext = &debug_utils_create_info;
		}
		else if (has_debug_report)
		{
			debug_report_create_info = vk::DebugReportCallbackCreateInfoEXT(
				vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning, debug_callback);

			instance_info.pNext = &debug_report_create_info;
		}
#endif

		handle = vk::createInstance(instance_info);

		VULKAN_HPP_DEFAULT_DISPATCHER.init(handle);

#ifdef _DEBUG
		if (has_debug_utils)
		{
			debug_utils_messenger = handle.createDebugUtilsMessengerEXT(debug_utils_create_info);
		}
		else if (has_debug_report)
		{
			debug_report_callback = handle.createDebugReportCallbackEXT(debug_report_create_info);
		}
#endif

		query_gpus();
	}

	HPPInstance::~HPPInstance()
	{
#ifdef _DEBUG
		if (debug_utils_messenger)
		{
			handle.destroyDebugUtilsMessengerEXT(debug_utils_messenger);
		}
		if (debug_report_callback)
		{
			handle.destroyDebugReportCallbackEXT(debug_report_callback);
		}
#endif

		if (handle)
		{
			handle.destroy();
		}
	}

	vk::Instance HPPInstance::get_handle() const
	{
		return handle;
	}

	HPPPhysicalDevice& HPPInstance::get_suitable_gpu(vk::SurfaceKHR surface, bool headless)
	{
		for (auto& gpu : gpus)
		{
			if (gpu->get_properties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
			{
				size_t queue_count = gpu->get_queue_family_properties().size();
				for (uint32_t queue_idx = 0; static_cast<size_t>(queue_idx) < queue_count; ++queue_idx)
				{
					if (gpu->get_handle().getSurfaceSupportKHR(queue_idx, surface))
					{
						return *gpu;
					}
				}
			}
		}

		spdlog::warn("Couldn't find a discrete physical device, picking default GPU");
		return *gpus[0];
	}

	void HPPInstance::query_gpus()
	{
		auto physical_devices = handle.enumeratePhysicalDevices();
		if (physical_devices.empty())
		{
			throw std::runtime_error("Couldn't find a physical device that supports Vulkan.");
		}

		for (auto& physical_device : physical_devices)
		{
			gpus.push_back(std::make_unique<vkb::HPPPhysicalDevice>(*this, physical_device));
		}
	}
}