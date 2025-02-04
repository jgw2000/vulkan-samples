#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>
#include <unordered_map>

namespace vkb
{
	class HPPPhysicalDevice;

	class HPPInstance
	{
	public:
		HPPInstance(const std::string& application_name,
					const std::unordered_map<const char*, bool>& required_extensions = {},
					const std::vector<const char*>& required_validation_layers = {},
					uint32_t									 api_version = VK_API_VERSION_1_0);
		~HPPInstance();

		HPPInstance(const HPPInstance&) = delete;
		HPPInstance(HPPInstance&&) = delete;

		HPPInstance& operator=(const HPPInstance&) = delete;
		HPPInstance& operator=(HPPInstance&&) = delete;

		vk::Instance get_handle() const;

		HPPPhysicalDevice& get_suitable_gpu(vk::SurfaceKHR surface, bool headless_surface);

	private:
		void query_gpus();

	private:
		vk::Instance handle;

#ifdef _DEBUG
		vk::DebugUtilsMessengerEXT debug_utils_messenger;

		vk::DebugReportCallbackEXT debug_report_callback;
#endif

		std::vector<const char*> enabled_extensions;

		std::vector<std::unique_ptr<HPPPhysicalDevice>> gpus;
	};
}