#pragma once

#include "application.h"
#include "core/hpp_instance.h"

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

		virtual const std::vector<const char*> get_validation_layers();

		void add_instance_extension(const char* extension, bool optional = false);

	private:
		const std::unordered_map<const char*, bool>& get_instance_extensions() const;

	private:
		std::unique_ptr<vkb::HPPInstance> instance;

		std::unordered_map<const char*, bool> instance_extensions;

		std::unordered_map<const char*, bool> device_extensions;

		uint32_t api_version = VK_API_VERSION_1_0;
	};

	inline const std::vector<const char*> VulkanSample::get_validation_layers()
	{
		return {};
	}

	inline const std::unordered_map<const char*, bool>& VulkanSample::get_instance_extensions() const
	{
		return instance_extensions;
	}

	inline std::unique_ptr<vkb::HPPInstance> VulkanSample::create_instance()
	{
		return std::make_unique<vkb::HPPInstance>(get_name(), get_instance_extensions(), get_validation_layers(), api_version);
	}
}