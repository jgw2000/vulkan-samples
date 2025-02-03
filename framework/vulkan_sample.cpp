#include "vulkan_sample.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace vkb
{
	VulkanSample::~VulkanSample()
	{

	}

	bool VulkanSample::prepare(const Window::Properties& properties)
	{
		Application::prepare(properties);

		static vk::DynamicLoader dl;
		VULKAN_HPP_DEFAULT_DISPATCHER.init(dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"));

		// Creating the vulkan instance
		for (const char* extension_name : window->get_required_surface_extensions())
		{
			add_instance_extension(extension_name);
		}

		instance = create_instance();


		return true;
	}

	void VulkanSample::update(float delta_time)
	{
		
	}

	void VulkanSample::add_instance_extension(const char* extension, bool optional)
	{
		instance_extensions[extension] = optional;
	}
}