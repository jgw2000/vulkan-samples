#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

namespace vkb
{
	class HPPDevice;

	template <typename Handle>
	class VulkanResource
	{
	public:
		VulkanResource(Handle handle_ = nullptr, vkb::HPPDevice* device_ = nullptr);
		virtual ~VulkanResource() = default;

		VulkanResource(VulkanResource&&);
		VulkanResource& operator=(VulkanResource&&);

		VulkanResource(const VulkanResource&) = delete;
		VulkanResource& operator=(const VulkanResource&) = delete;

		vkb::HPPDevice&			get_device()			{ return *device; }
		const vkb::HPPDevice&	get_device() const		{ return *device; }
		Handle&					get_handle()			{ return handle; }
		const Handle&			get_handle() const		{ return handle; }
		bool					has_device() const		{ return device != nullptr; }
		bool					has_handle() const		{ return handle != nullptr; }
		void					set_handle(Handle hdl)  { handle = hdl; }

	private:
		HPPDevice* device;
		Handle handle;
	};

	template <typename Handle>
	inline VulkanResource<Handle>::VulkanResource(Handle handle_, vkb::HPPDevice* device_) :
		handle{ handle_ }
	{
		device = device_;
	}

	template <typename Handle>
	inline VulkanResource<Handle>::VulkanResource(VulkanResource&& other) :
		handle(std::exchange(other.handle, {})),
		device(std::exchange(other.device, {}))
	{ }

	template <typename Handle>
	inline VulkanResource<Handle>& VulkanResource<Handle>::operator=(VulkanResource&& other)
	{
		handle = std::exchange(other.handle, {});
		device = std::exchange(other.device, {});
		return *this;
	}
}