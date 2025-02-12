#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

namespace vkb
{
	class HPPDevice;

	class HPPQueue
	{
	public:
		HPPQueue(HPPDevice& device, uint32_t family_index, vk::QueueFamilyProperties properties, vk::Bool32 can_present, uint32_t index);

		HPPQueue(const HPPQueue&) = default;
		HPPQueue(HPPQueue&&);

		HPPQueue& operator=(const HPPQueue&) = delete;
		HPPQueue& operator=(HPPQueue&&) = delete;

		const HPPDevice& get_device() const;

		vk::Queue get_handle() const;

		uint32_t get_family_index() const;

		uint32_t get_index() const;

		const vk::QueueFamilyProperties& get_properties() const;

		vk::Bool32 support_present() const;

	private:
		HPPDevice& device;

		vk::Queue handle;

		uint32_t family_index{ 0 };

		uint32_t index{ 0 };

		vk::Bool32 can_present = false;

		vk::QueueFamilyProperties properties{};
	};
}