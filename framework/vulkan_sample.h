#pragma once

#include "application.h"
#include "core/hpp_instance.h"
#include "core/hpp_device.h"
#include "render/hpp_render_context.h"

#include <unordered_map>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1

namespace vkb
{
	class VulkanSample : public vkb::Application
	{
	public:
		VulkanSample() = default;
		~VulkanSample() override;

	protected:
		virtual bool prepare(const Window::Properties& properties) override;

		virtual void update(float delta_time) override;

		virtual std::unique_ptr<vkb::HPPInstance> create_instance();

		virtual std::unique_ptr<vkb::HPPDevice> create_device(vkb::HPPPhysicalDevice& gpu);

		virtual void create_render_context();

		virtual const std::vector<const char*> get_validation_layers();

		virtual void request_gpu_features(HPPPhysicalDevice& gpu) {}

		void add_instance_extension(const char* extension, bool optional = false);

		void add_device_extension(const char* extension, bool optional = false);

		void create_render_context(const std::vector<vk::SurfaceFormatKHR>& surface_priority_list);

	private:
		void create_render_context_impl(const std::vector<vk::SurfaceFormatKHR>& surface_priority_list);

		const std::unordered_map<const char*, bool>& get_instance_extensions() const;

		const std::unordered_map<const char*, bool>& get_device_extensions() const;

	private:
		std::unique_ptr<vkb::HPPInstance> instance;

		std::unique_ptr<vkb::HPPDevice> device;

		std::unique_ptr<vkb::HPPRenderContext> render_context;

		vk::SurfaceKHR surface;

		/**
		* @brief A list of surface formats in order of priority (vector[0] has high priority, vector[size-1] has low priority)
		*/
		std::vector<vk::SurfaceFormatKHR> surface_priority_list = {
			{ vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear },
			{ vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear },
		};

		std::unordered_map<const char*, bool> instance_extensions;

		std::unordered_map<const char*, bool> device_extensions;

		uint32_t api_version = VK_API_VERSION_1_0;

		bool high_priority_graphics_queue{ false };
	};

	inline const std::vector<const char*> VulkanSample::get_validation_layers()
	{
		return {};
	}

	inline const std::unordered_map<const char*, bool>& VulkanSample::get_instance_extensions() const
	{
		return instance_extensions;
	}

	inline const std::unordered_map<const char*, bool>& VulkanSample::get_device_extensions() const
	{
		return device_extensions;
	}

	inline std::unique_ptr<vkb::HPPInstance> VulkanSample::create_instance()
	{
		return std::make_unique<vkb::HPPInstance>(get_name(), get_instance_extensions(), get_validation_layers(), api_version);
	}

	inline std::unique_ptr<vkb::HPPDevice> VulkanSample::create_device(vkb::HPPPhysicalDevice& gpu)
	{
		return std::make_unique<vkb::HPPDevice>(gpu, surface, get_device_extensions());
	}
}