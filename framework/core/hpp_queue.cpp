#include "hpp_queueu.h"
#include "hpp_device.h"

namespace vkb
{
	HPPQueue::HPPQueue(vkb::HPPDevice& device, uint32_t family_index, vk::QueueFamilyProperties properties, vk::Bool32 can_present, uint32_t index) :
		device{ device },
		family_index{ family_index },
		index{ index },
		can_present{ can_present },
		properties{ properties }
	{
		handle = device.get_handle().getQueue(family_index, index);
	}

	HPPQueue::HPPQueue(HPPQueue&& other):
		device(other.device),
		handle(std::exchange(other.handle, {})),
		family_index(std::exchange(other.family_index, {})),
		index(std::exchange(other.index, 0)),
		can_present(std::exchange(other.can_present, false)),
		properties(std::exchange(other.properties, {}))
	{ }

	const HPPDevice& HPPQueue::get_device() const
	{
		return device;
	}

	vk::Queue HPPQueue::get_handle() const
	{
		return handle;
	}

	uint32_t HPPQueue::get_family_index() const
	{
		return family_index;
	}

	uint32_t HPPQueue::get_index() const
	{
		return index;
	}

	const vk::QueueFamilyProperties& HPPQueue::get_properties() const
	{
		return properties;
	}

	vk::Bool32 HPPQueue::support_present() const
	{
		return can_present;
	}
}