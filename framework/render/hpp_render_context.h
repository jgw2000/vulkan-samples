#pragma once

#include "core/hpp_device.h"
#include "core/hpp_swapchain.h"
#include "window.h"

namespace vkb
{
	class HPPRenderContext
	{
	public:
		static vk::Format DEFAULT_VK_FORMAT;

		HPPRenderContext(vkb::HPPDevice& device, vk::SurfaceKHR surface,
						 const vkb::Window& window,
						 vk::PresentModeKHR present_mode = vk::PresentModeKHR::eFifo,
						 const std::vector<vk::PresentModeKHR>& present_mode_priority_list = { vk::PresentModeKHR::eFifo, vk::PresentModeKHR::eMailbox },
						 const std::vector<vk::SurfaceFormatKHR>& surface_format_priority_list = {
							 {vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}, {vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}
						 });
		virtual ~HPPRenderContext() = default;

		HPPRenderContext(const HPPRenderContext&) = delete;
		HPPRenderContext(HPPRenderContext&&) = delete;

		HPPRenderContext& operator=(const HPPRenderContext&) = delete;
		HPPRenderContext& operator=(HPPRenderContext&&) = delete;

	protected:
		vk::Extent2D surface_extent;

	private:
		vkb::HPPDevice& device;

		const vkb::Window& window;

		const vkb::HPPQueue& queue;

		std::unique_ptr<vkb::HPPSwapchain> swapchain;

		vkb::HPPSwapchainProperties swapchain_properties;

		bool prepared{ false };
	};
}