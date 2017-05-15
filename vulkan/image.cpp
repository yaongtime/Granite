#include "image.hpp"
#include "device.hpp"

using namespace std;

namespace Vulkan
{

ImageView::ImageView(Device *device, VkImageView view, const ImageViewCreateInfo &info)
    : Cookie(device)
    , device(device)
    , view(view)
    , info(info)
{
}

ImageView::~ImageView()
{
	device->destroy_image_view(view);
	if (depth_view != VK_NULL_HANDLE)
		device->destroy_image_view(depth_view);
	if (stencil_view != VK_NULL_HANDLE)
		device->destroy_image_view(stencil_view);
}

Image::Image(Device *device, VkImage image, VkImageView default_view, const DeviceAllocation &alloc,
             const ImageCreateInfo &create_info)
    : Cookie(device)
    , device(device)
    , image(image)
    , alloc(alloc)
    , create_info(create_info)
{
	if (default_view != VK_NULL_HANDLE)
	{
		ImageViewCreateInfo info;
		info.image = this;
		info.format = create_info.format;
		info.base_level = 0;
		info.levels = create_info.levels;
		info.base_layer = 0;
		info.layers = create_info.layers;
		view = Util::make_handle<ImageView>(device, default_view, info);
	}
}

Image::~Image()
{
	if (alloc.get_memory())
	{
		device->destroy_image(image);
		device->free_memory(alloc);
	}
}
}
