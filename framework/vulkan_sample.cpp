#include "vulkan_sample.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace vkb
{
	VulkanSample::~VulkanSample()
	{
		if (surface)
		{
			instance->get_handle().destroySurfaceKHR(surface);
		}

		device.reset();
		instance.reset();
	}

	bool VulkanSample::prepare(const Window::Properties& properties)
	{
		Application::prepare(properties);

		// Create instance
		static vk::DynamicLoader dl;
		VULKAN_HPP_DEFAULT_DISPATCHER.init(dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr"));

		for (const char* extension_name : window->get_required_surface_extensions())
		{
			add_instance_extension(extension_name);
		}

		instance = create_instance();

		surface = static_cast<vk::SurfaceKHR>(window->create_surface(instance->get_handle()));
		if (!surface)
		{
			throw std::runtime_error("Failed to create window surface.");
		}

		bool headless = window->get_window_mode() == Window::Mode::Headless;
		auto& gpu = instance->get_suitable_gpu(surface, headless);
		gpu.set_high_priority_graphics_queue_enable(high_priority_graphics_queue);

		request_gpu_features(gpu);

		// Create device
		add_device_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		if (instance_extensions.find(VK_KHR_DISPLAY_EXTENSION_NAME) != instance_extensions.end())
		{
			add_device_extension(VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME, true);
		}

		device = create_device(gpu);

		VULKAN_HPP_DEFAULT_DISPATCHER.init(device->get_handle());

		return true;
	}

	void VulkanSample::update(float delta_time)
	{
		
	}

	void VulkanSample::add_instance_extension(const char* extension, bool optional)
	{
		instance_extensions[extension] = optional;
	}

	void VulkanSample::add_device_extension(const char* extension, bool optional)
	{
		device_extensions[extension] = optional;
	}
}