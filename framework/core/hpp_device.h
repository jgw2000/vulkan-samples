#pragma once

#include "vulkan_resource.h"
#include "hpp_physical_device.h"
#include "hpp_queueu.h"

#include <unordered_map>

namespace vkb
{
	class HPPDevice : public vkb::VulkanResource<vk::Device>
	{
	public:
		HPPDevice(vkb::HPPPhysicalDevice& gpu, vk::SurfaceKHR surface,
				  std::unordered_map<const char*, bool> requested_extensions = {});
		~HPPDevice();

		HPPDevice(const HPPDevice&) = delete;
		HPPDevice(HPPDevice&&) = delete;

		HPPDevice& operator=(const HPPDevice&) = delete;
		HPPDevice& operator=(HPPDevice&&) = delete;

		const vkb::HPPPhysicalDevice& get_gpu() const { return gpu; }

		const vkb::HPPQueue& get_queue(uint32_t queue_family_index, uint32_t queue_index) const;

		const vkb::HPPQueue& get_queue_by_flags(vk::QueueFlags queue_flags, uint32_t queue_index) const;

		const vkb::HPPQueue& get_queue_by_present(uint32_t queue_index) const;

		const vkb::HPPQueue& get_suitable_graphics_queue() const;

		bool is_extension_supported(const std::string& extension) const { return gpu.is_extension_supported(extension); }

		bool is_enabled(const std::string& extension) const;

		uint32_t get_queue_family_index(vk::QueueFlagBits queue_flag) const;

	private:
		const vkb::HPPPhysicalDevice& gpu;

		vk::SurfaceKHR surface{ nullptr };

		std::vector<const char*> enabled_extensions{};

		std::vector<std::vector<vkb::HPPQueue>> queues;
	};
}