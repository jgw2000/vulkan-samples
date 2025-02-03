#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>
#include <unordered_map>

namespace vkb
{
	class HPPInstance
	{
	public:
		HPPInstance(const HPPInstance&) = delete;
		HPPInstance(HPPInstance&&) = delete;
		~HPPInstance();

		HPPInstance& operator=(const HPPInstance&) = delete;
		HPPInstance& operator=(HPPInstance&&) = delete;

		HPPInstance(const std::string& application_name,
					const std::unordered_map<const char*, bool>& required_extensions = {},
					const std::vector<const char*>&				 required_validation_layers = {},
					uint32_t									 api_version = VK_API_VERSION_1_0);

		vk::Instance get_handle() const;

	private:
		void query_gpus();

	private:
		vk::Instance handle;

#ifdef _DEBUG
		vk::DebugUtilsMessengerEXT debug_utils_messenger;

		vk::DebugReportCallbackEXT debug_report_callback;
#endif

		std::vector<const char*> enabled_extensions;
	};
}