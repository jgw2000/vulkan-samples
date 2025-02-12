#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.hpp>

#include <set>

namespace vkb
{
	class HPPDevice;

	struct HPPSwapchainProperties
	{
		vk::SwapchainKHR				old_swapchain;
		uint32_t						image_count{ 3 };
		vk::Extent2D					extent;
		vk::SurfaceFormatKHR			surface_format;
		uint32_t						array_layers;
		vk::ImageUsageFlags				image_usage;
		vk::SurfaceTransformFlagBitsKHR pre_transform;
		vk::CompositeAlphaFlagBitsKHR	composite_alpha;
		vk::PresentModeKHR				present_mode;
	};

	class HPPSwapchain
	{
	public:
		/**
		 * @brief Constructor to create a swapchain by changing the extent
		 *        only and preserving the configuration from the old swapchain.
		 */
		HPPSwapchain(HPPSwapchain& old_swapchain, const vk::Extent2D& extent);

		/**
		 * @brief Constructor to create a swapchain by changing the image count
		 *        only and preserving the configuration from the old swapchain.
		 */
		HPPSwapchain(HPPSwapchain& old_swapchain, const uint32_t image_count);

		/**
		 * @brief Constructor to create a swapchain by changing the image usage
		 * only and preserving the configuration from the old swapchain.
		 */
		HPPSwapchain(HPPSwapchain& old_swapchain, const std::set<vk::ImageUsageFlagBits>& image_usage_flags);

		/**
		 * @brief Constructor to create a swapchain by changing the extent
		 *        and transform only and preserving the configuration from the old swapchain.
		 */
		HPPSwapchain(HPPSwapchain& old_swapchain, const vk::Extent2D& extent, const vk::SurfaceTransformFlagBitsKHR transform);

		HPPSwapchain(HPPDevice& device,
			vk::SurfaceKHR surface,
			const vk::PresentModeKHR present_mode,
			const std::vector<vk::PresentModeKHR>& present_mode_priority_list	  = { vk::PresentModeKHR::eFifo, vk::PresentModeKHR::eMailbox },
			const std::vector<vk::SurfaceFormatKHR>& surface_format_priority_list = {
				{vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear},
				{vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear}
			},
			const vk::Extent2D& extent											  = {},
			const uint32_t image_count											  = 3,
			const vk::SurfaceTransformFlagBitsKHR transform						  = vk::SurfaceTransformFlagBitsKHR::eIdentity,
			const std::set<vk::ImageUsageFlagBits>& image_usage_flags			  = { vk::ImageUsageFlagBits::eColorAttachment, vk::ImageUsageFlagBits::eTransferSrc },
			vk::SwapchainKHR old_swapchain										  = nullptr
		);

		~HPPSwapchain();

		HPPSwapchain(const HPPSwapchain&) = delete;
		HPPSwapchain(HPPSwapchain&&);

		HPPSwapchain& operator=(const HPPSwapchain&) = delete;
		HPPSwapchain& operator=(HPPSwapchain&&) = delete;

		bool is_valid() const { return !!handle; }

		const HPPDevice& get_device() const { return device; }

		vk::SwapchainKHR get_handle() const { return handle; }

		const vk::Extent2D& get_extent() const { return properties.extent; }

		vk::Format get_format() const { return properties.surface_format.format; }

		const std::vector<vk::Image>& get_images() const { return images; }

		vk::SurfaceTransformFlagBitsKHR get_transform() const { return properties.pre_transform; }
		
		vk::ImageUsageFlags get_usage() const { return properties.image_usage; }

		vk::PresentModeKHR get_present_mode() const { return properties.present_mode; }

	private:
		HPPDevice& device;
		
		vk::SurfaceKHR surface;

		vk::SwapchainKHR handle;

		std::vector<vk::Image> images;

		std::vector<vk::ImageView> image_views;

		HPPSwapchainProperties properties;

		std::vector<vk::PresentModeKHR> present_mode_priority_list;

		std::vector<vk::SurfaceFormatKHR> surface_format_priority_list;

		std::set<vk::ImageUsageFlagBits> image_usage_flags;
	};
}