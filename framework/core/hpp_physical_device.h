#pragma once

#include "hpp_instance.h"

#include <vector>

namespace vkb
{
	class HPPPhysicalDevice
	{
	public:
		HPPPhysicalDevice(HPPInstance& instance, vk::PhysicalDevice physical_device);

		HPPPhysicalDevice(const HPPPhysicalDevice&) = delete;
		HPPPhysicalDevice(HPPPhysicalDevice&&) = delete;

		HPPPhysicalDevice& operator=(const HPPPhysicalDevice&) = delete;
		HPPPhysicalDevice& operator=(HPPPhysicalDevice&&) = delete;

		vk::PhysicalDevice get_handle() const { return handle; }

		bool is_extension_supported(const std::string& requested_extension) const;

		const vk::PhysicalDeviceProperties& get_properties() const { return properties; }

		const std::vector<vk::QueueFamilyProperties>& get_queue_family_properties() const { return queue_family_properties; }

		const vk::PhysicalDeviceFeatures get_requested_features() const { return requested_features; }

		vk::PhysicalDeviceFeatures& get_mutable_requested_features() { return requested_features; }

		void* get_extension_feature_chain() const
		{
			return last_requested_extension_feature;
		}

		void set_high_priority_graphics_queue_enable(bool enable)
		{
			high_priority_graphics_queue = enable;
		}

		bool has_high_priority_graphics_queue() const
		{
			return high_priority_graphics_queue;
		}

	private:
		HPPInstance& instance;

		vk::PhysicalDevice handle;

		// The features that this GPU supports
		vk::PhysicalDeviceFeatures features;

		// The extensions that this GPU supports
		std::vector<vk::ExtensionProperties> device_extensions;

		// The GPU properties
		vk::PhysicalDeviceProperties properties;

		// The GPU memory properties
		vk::PhysicalDeviceMemoryProperties memory_properties;

		// The GPU queue family properties
		std::vector<vk::QueueFamilyProperties> queue_family_properties;

		// The features that will be requested to be enabled in the logical device
		vk::PhysicalDeviceFeatures requested_features;

		// The extension feature pointer
		void* last_requested_extension_feature{ nullptr };

		bool high_priority_graphics_queue{ false };
	};
}