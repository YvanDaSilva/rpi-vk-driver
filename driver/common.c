#include "common.h"

#include "kernel/vc4_packet.h"
#include "brcm/cle/v3d_decoder.h"
#include "brcm/clif/clif_dump.h"

uint32_t getFormatBpp(VkFormat f)
{
	switch(f)
	{
	case VK_FORMAT_R64G64B64_UINT: //padded to 256
	case VK_FORMAT_R64G64B64_SINT:
	case VK_FORMAT_R64G64B64_SFLOAT:
	case VK_FORMAT_R64G64B64A64_UINT:
	case VK_FORMAT_R64G64B64A64_SINT:
	case VK_FORMAT_R64G64B64A64_SFLOAT:
		return 256;
	case VK_FORMAT_R32G32B32_UINT: //padded to 128
	case VK_FORMAT_R32G32B32_SINT:
	case VK_FORMAT_R32G32B32_SFLOAT:
	case VK_FORMAT_R32G32B32A32_UINT:
	case VK_FORMAT_R32G32B32A32_SINT:
	case VK_FORMAT_R32G32B32A32_SFLOAT:
	case VK_FORMAT_R64G64_UINT:
	case VK_FORMAT_R64G64_SINT:
	case VK_FORMAT_R64G64_SFLOAT:
		return 128;
	case VK_FORMAT_R16G16B16A16_SFLOAT:
	case VK_FORMAT_R16G16B16_UNORM: //padded to 64
	case VK_FORMAT_R16G16B16_SNORM: //padded to 64
	case VK_FORMAT_R16G16B16_USCALED: //padded to 64
	case VK_FORMAT_R16G16B16_SSCALED: //padded to 64
	case VK_FORMAT_R16G16B16_UINT: //padded to 64
	case VK_FORMAT_R16G16B16_SINT: //padded to 64
	case VK_FORMAT_R16G16B16_SFLOAT: //padded to 64
	case VK_FORMAT_R16G16B16A16_UNORM:
	case VK_FORMAT_R16G16B16A16_SNORM:
	case VK_FORMAT_R16G16B16A16_USCALED:
	case VK_FORMAT_R16G16B16A16_SSCALED:
	case VK_FORMAT_R16G16B16A16_UINT:
	case VK_FORMAT_R16G16B16A16_SINT:
	case VK_FORMAT_R32G32_UINT:
	case VK_FORMAT_R32G32_SINT:
	case VK_FORMAT_R32G32_SFLOAT:
	case VK_FORMAT_R64_UINT:
	case VK_FORMAT_R64_SINT:
	case VK_FORMAT_R64_SFLOAT:
	case VK_FORMAT_D32_SFLOAT_S8_UINT: //padded to 64
		return 64;
	case VK_FORMAT_R8G8B8_UNORM: //padded to 32
	case VK_FORMAT_R8G8B8A8_UNORM:
	case VK_FORMAT_R32_UINT:
	case VK_FORMAT_R8G8B8A8_UINT:
	case VK_FORMAT_D32_SFLOAT:
	case VK_FORMAT_R8G8B8_SNORM: //padded to 32
	case VK_FORMAT_R8G8B8_USCALED: //padded to 32
	case VK_FORMAT_R8G8B8_SSCALED: //padded to 32
	case VK_FORMAT_R8G8B8_UINT: //padded to 32
	case VK_FORMAT_R8G8B8_SINT: //padded to 32
	case VK_FORMAT_R8G8B8_SRGB: //padded to 32
	case VK_FORMAT_B8G8R8_UNORM: //padded to 32
	case VK_FORMAT_B8G8R8_SNORM: //padded to 32
	case VK_FORMAT_B8G8R8_USCALED: //padded to 32
	case VK_FORMAT_B8G8R8_SSCALED: //padded to 32
	case VK_FORMAT_B8G8R8_UINT: //padded to 32
	case VK_FORMAT_B8G8R8_SINT: //padded to 32
	case VK_FORMAT_B8G8R8_SRGB: //padded to 32
	case VK_FORMAT_R8G8B8A8_SNORM:
	case VK_FORMAT_R8G8B8A8_USCALED:
	case VK_FORMAT_R8G8B8A8_SSCALED:
	case VK_FORMAT_R8G8B8A8_SINT:
	case VK_FORMAT_R8G8B8A8_SRGB:
	case VK_FORMAT_B8G8R8A8_UNORM:
	case VK_FORMAT_B8G8R8A8_SNORM:
	case VK_FORMAT_B8G8R8A8_USCALED:
	case VK_FORMAT_B8G8R8A8_SSCALED:
	case VK_FORMAT_B8G8R8A8_UINT:
	case VK_FORMAT_B8G8R8A8_SINT:
	case VK_FORMAT_B8G8R8A8_SRGB:
	case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
	case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
	case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
	case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
	case VK_FORMAT_A8B8G8R8_UINT_PACK32:
	case VK_FORMAT_A8B8G8R8_SINT_PACK32:
	case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
	case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
	case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
	case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
	case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
	case VK_FORMAT_A2R10G10B10_UINT_PACK32:
	case VK_FORMAT_A2R10G10B10_SINT_PACK32:
	case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
	case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
	case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
	case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
	case VK_FORMAT_A2B10G10R10_UINT_PACK32:
	case VK_FORMAT_A2B10G10R10_SINT_PACK32:
	case VK_FORMAT_R16G16_UNORM:
	case VK_FORMAT_R16G16_SNORM:
	case VK_FORMAT_R16G16_USCALED:
	case VK_FORMAT_R16G16_SSCALED:
	case VK_FORMAT_R16G16_UINT:
	case VK_FORMAT_R16G16_SINT:
	case VK_FORMAT_R16G16_SFLOAT:
	case VK_FORMAT_R32_SINT:
	case VK_FORMAT_R32_SFLOAT:
	case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
	case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
	case VK_FORMAT_X8_D24_UNORM_PACK32:
	case VK_FORMAT_D16_UNORM_S8_UINT: //padded to 32
	case VK_FORMAT_D24_UNORM_S8_UINT:
		return 32;
	case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
	case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
	case VK_FORMAT_R5G6B5_UNORM_PACK16:
	case VK_FORMAT_R8G8_UNORM:
	case VK_FORMAT_R16_SFLOAT:
	case VK_FORMAT_R16_SINT:
	case VK_FORMAT_D16_UNORM:
	case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
	case VK_FORMAT_B5G6R5_UNORM_PACK16:
	case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
	case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
	case VK_FORMAT_R8G8_SNORM:
	case VK_FORMAT_R8G8_USCALED:
	case VK_FORMAT_R8G8_SSCALED:
	case VK_FORMAT_R8G8_UINT:
	case VK_FORMAT_R8G8_SINT:
	case VK_FORMAT_R8G8_SRGB:
	case VK_FORMAT_R16_UNORM:
	case VK_FORMAT_R16_SNORM:
	case VK_FORMAT_R16_USCALED:
	case VK_FORMAT_R16_SSCALED:
	case VK_FORMAT_R16_UINT:
		return 16;
	case VK_FORMAT_R8_UNORM:
	case VK_FORMAT_R8_SINT:
	case VK_FORMAT_S8_UINT:
	case VK_FORMAT_R4G4_UNORM_PACK8:
	case VK_FORMAT_R8_SNORM:
	case VK_FORMAT_R8_USCALED:
	case VK_FORMAT_R8_SSCALED:
	case VK_FORMAT_R8_UINT:
	case VK_FORMAT_R8_SRGB:
	case VK_FORMAT_UNDEFINED: //TODO
		return 8;
	default://
		printf("format %i\n", f);
		assert(0);
		return 0;
	}
}

uint32_t packVec4IntoABGR8(const float rgba[4])
{
	uint8_t r, g, b, a;
	r = rgba[0] * 255.0;
	g = rgba[1] * 255.0;
	b = rgba[2] * 255.0;
	a = rgba[3] * 255.0;

	uint32_t res = 0 |
			(a << 24) |
			(b << 16) |
			(g << 8) |
			(r << 0);

	return res;
}

int findInstanceExtension(char* name)
{
	for(int c = 0; c < numInstanceExtensions; ++c)
	{
		if(strcmp(instanceExtensions[c].extensionName, name) == 0)
		{
			return c;
		}
	}

	return -1;
}

int findDeviceExtension(char* name)
{
	for(int c = 0; c < numDeviceExtensions; ++c)
	{
		if(strcmp(deviceExtensions[c].extensionName, name) == 0)
		{
			return c;
		}
	}

	return -1;
}

//Textures in T format:
//formed out of 4KB tiles, which have 1KB subtiles (see page 105 in VC4 arch guide)
//1KB subtiles have 512b microtiles.
//Width/height of the 512b microtiles is the following:
// 64bpp: 2x4
// 32bpp: 4x4
// 16bpp: 8x4
// 8bpp:  8x8
// 4bpp:  16x8
// 1bpp:  32x16
//Therefore width/height of 1KB subtiles is the following:
// 64bpp: 8x16
// 32bpp: 16x16
// 16bpp: 32x16
// 8bpp:  32x32
// 4bpp:  64x32
// 1bpp:  128x64
//Finally width/height of the 4KB tiles:
// 64bpp: 16x32
// 32bpp: 32x32
// 16bpp: 64x32
// 8bpp:  64x64
// 4bpp:  128x64
// 1bpp:  256x128
void getPaddedTextureDimensionsT(uint32_t width, uint32_t height, uint32_t bpp, uint32_t* paddedWidth, uint32_t* paddedHeight)
{
	assert(paddedWidth);
	assert(paddedHeight);
	uint32_t tileW = 0;
	uint32_t tileH = 0;

	switch(bpp)
	{
	case 256:
	{
		tileW = 8;
		tileH = 16;
		break;
	}
	case 128:
	{
		tileW = 16;
		tileH = 16;
		break;
	}
	case 64:
	{
		tileW = 16;
		tileH = 32;
		break;
	}
	case 32:
	{
		tileW = 32;
		tileH = 32;
		break;
	}
	case 16:
	{
		tileW = 64;
		tileH = 32;
		break;
	}
	case 8:
	{
		tileW = 64;
		tileH = 64;
		break;
	}
	case 4:
	{
		tileW = 128;
		tileH = 64;
		break;
	}
	case 1:
	{
		tileW = 256;
		tileH = 128;
		break;
	}
	default:
	{
		assert(0); //unsupported
	}
	}

	*paddedWidth = ((tileW - (width % tileW)) % tileW) + width;
	*paddedHeight = ((tileH - (height % tileH)) % tileH) + height;
}

/*static inline void util_pack_color(const float rgba[4], enum pipe_format format, union util_color *uc)
{
   ubyte r = 0;
   ubyte g = 0;
   ubyte b = 0;
   ubyte a = 0;

   if (util_format_get_component_bits(format, UTIL_FORMAT_COLORSPACE_RGB, 0) <= 8) {
	  r = float_to_ubyte(rgba[0]);
	  g = float_to_ubyte(rgba[1]);
	  b = float_to_ubyte(rgba[2]);
	  a = float_to_ubyte(rgba[3]);
   }

   switch (format) {
   case PIPE_FORMAT_ABGR8888_UNORM:
	  {
		 uc->ui[0] = (r << 24) | (g << 16) | (b << 8) | a;
	  }
	  return;
   case PIPE_FORMAT_XBGR8888_UNORM:
	  {
		 uc->ui[0] = (r << 24) | (g << 16) | (b << 8) | 0xff;
	  }
	  return;
   case PIPE_FORMAT_BGRA8888_UNORM:
	  {
		 uc->ui[0] = (a << 24) | (r << 16) | (g << 8) | b;
	  }
	  return;
   case PIPE_FORMAT_BGRX8888_UNORM:
	  {
		 uc->ui[0] = (0xffu << 24) | (r << 16) | (g << 8) | b;
	  }
	  return;
   case PIPE_FORMAT_ARGB8888_UNORM:
	  {
		 uc->ui[0] = (b << 24) | (g << 16) | (r << 8) | a;
	  }
	  return;
   case PIPE_FORMAT_XRGB8888_UNORM:
	  {
		 uc->ui[0] = (b << 24) | (g << 16) | (r << 8) | 0xff;
	  }
	  return;
   case PIPE_FORMAT_B5G6R5_UNORM:
	  {
		 uc->us = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3);
	  }
	  return;
   case PIPE_FORMAT_B5G5R5X1_UNORM:
	  {
		 uc->us = ((0x80) << 8) | ((r & 0xf8) << 7) | ((g & 0xf8) << 2) | (b >> 3);
	  }
	  return;
   case PIPE_FORMAT_B5G5R5A1_UNORM:
	  {
		 uc->us = ((a & 0x80) << 8) | ((r & 0xf8) << 7) | ((g & 0xf8) << 2) | (b >> 3);
	  }
	  return;
   case PIPE_FORMAT_B4G4R4A4_UNORM:
	  {
		 uc->us = ((a & 0xf0) << 8) | ((r & 0xf0) << 4) | ((g & 0xf0) << 0) | (b >> 4);
	  }
	  return;
   case PIPE_FORMAT_A8_UNORM:
	  {
		 uc->ub = a;
	  }
	  return;
   case PIPE_FORMAT_L8_UNORM:
   case PIPE_FORMAT_I8_UNORM:
	  {
		 uc->ub = r;
	  }
	  return;
   case PIPE_FORMAT_R32G32B32A32_FLOAT:
	  {
		 uc->f[0] = rgba[0];
		 uc->f[1] = rgba[1];
		 uc->f[2] = rgba[2];
		 uc->f[3] = rgba[3];
	  }
	  return;
   case PIPE_FORMAT_R32G32B32_FLOAT:
	  {
		 uc->f[0] = rgba[0];
		 uc->f[1] = rgba[1];
		 uc->f[2] = rgba[2];
	  }
	  return;

   default:
	  util_format_write_4f(format, rgba, 0, uc, 0, 0, 0, 1, 1);
   }
}*/

int isDepthStencilFormat(VkFormat format)
{
	switch(format)
	{
	case VK_FORMAT_D16_UNORM:
	case VK_FORMAT_X8_D24_UNORM_PACK32:
	case VK_FORMAT_D32_SFLOAT:
	case VK_FORMAT_S8_UINT:
	case VK_FORMAT_D16_UNORM_S8_UINT:
	case VK_FORMAT_D24_UNORM_S8_UINT:
	case VK_FORMAT_D32_SFLOAT_S8_UINT:
		return 1;
	default:
		return 0;
	}
}

uint32_t getDepthCompareOp(VkCompareOp op)
{
	switch(op)
	{
	case VK_COMPARE_OP_NEVER:
		return V3D_COMPARE_FUNC_NEVER;
	case VK_COMPARE_OP_LESS:
		return V3D_COMPARE_FUNC_LESS;
	case VK_COMPARE_OP_EQUAL:
		return V3D_COMPARE_FUNC_EQUAL;
	case VK_COMPARE_OP_LESS_OR_EQUAL:
		return V3D_COMPARE_FUNC_LEQUAL;
	case VK_COMPARE_OP_GREATER:
		return V3D_COMPARE_FUNC_GREATER;
	case VK_COMPARE_OP_NOT_EQUAL:
		return V3D_COMPARE_FUNC_NOTEQUAL;
	case VK_COMPARE_OP_GREATER_OR_EQUAL:
		return V3D_COMPARE_FUNC_GEQUAL;
	case VK_COMPARE_OP_ALWAYS:
		return V3D_COMPARE_FUNC_ALWAYS;
	default:
		return -1;
	}
}

uint32_t getTopology(VkPrimitiveTopology topology)
{
	switch(topology)
	{
	case VK_PRIMITIVE_TOPOLOGY_POINT_LIST:
		return 0;
	case VK_PRIMITIVE_TOPOLOGY_LINE_LIST:
	case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP:
		return 1;
	case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
	case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
	case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
		return 2;
	default:
		return -1;
	}
}

uint32_t getPrimitiveMode(VkPrimitiveTopology topology)
{
	switch(topology)
	{
	case VK_PRIMITIVE_TOPOLOGY_POINT_LIST:
		return V3D_PRIM_POINTS;
	case VK_PRIMITIVE_TOPOLOGY_LINE_LIST:
		return V3D_PRIM_LINES;
	case VK_PRIMITIVE_TOPOLOGY_LINE_STRIP:
		return V3D_PRIM_LINE_STRIP;
	case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST:
		return V3D_PRIM_TRIANGLES;
	case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP:
		return V3D_PRIM_TRIANGLE_STRIP;
	case VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN:
		return V3D_PRIM_TRIANGLE_FAN;
	default:
		return -1;
	}
}

uint32_t getFormatByteSize(VkFormat format)
{
	switch(format)
	{
	case VK_FORMAT_R16_SFLOAT:
		return 2;
	case VK_FORMAT_R16G16_SFLOAT:
		return 4;
	case VK_FORMAT_R16G16B16_SFLOAT:
		return 6;
	case VK_FORMAT_R16G16B16A16_SFLOAT:
		return 8;
	case VK_FORMAT_R32_SFLOAT:
		return 4;
	case VK_FORMAT_R32G32_SFLOAT:
		return 8;
	case VK_FORMAT_R32G32B32_SFLOAT:
		return 8;
	case VK_FORMAT_R32G32B32A32_SFLOAT:
		return 8;
	default:
		return -1;
	}
}

uint32_t ulog2(uint32_t v)
{
	uint32_t ret = 0;
	while(v >>= 1) ret++;
	return ret;
}

void clFit(VkCommandBuffer cb, ControlList* cl, uint32_t commandSize)
{
	if(!clHasEnoughSpace(cl, commandSize))
	{
		uint32_t currSize = clSize(cl);
		cl->buffer = consecutivePoolReAllocate(&cb->cp->cpa, cl->buffer, cl->numBlocks); assert(cl->buffer);
		cl->nextFreeByte = cl->buffer + currSize;
	}
}

void clDump(void* cl, uint32_t size)
{
		struct v3d_device_info devinfo = {
				/* While the driver supports V3D 2.1 and 2.6, we haven't split
				 * off a 2.6 XML yet (there are a couple of fields different
				 * in render target formatting)
				 */
				.ver = 21,
		};
		struct v3d_spec* spec = v3d_spec_load(&devinfo);

		struct clif_dump *clif = clif_dump_init(&devinfo, stderr, true);

		uint32_t offset = 0, hw_offset = 0;
		uint8_t *p = cl;

		while (offset < size) {
				struct v3d_group *inst = v3d_spec_find_instruction(spec, p);
				uint8_t header = *p;
				uint32_t length;

				if (inst == NULL) {
						printf("0x%08x 0x%08x: Unknown packet 0x%02x (%d)!\n",
								offset, hw_offset, header, header);
						return;
				}

				length = v3d_group_get_length(inst);

				printf("0x%08x 0x%08x: 0x%02x %s\n",
						offset, hw_offset, header, v3d_group_get_name(inst));

				v3d_print_group(clif, inst, offset, p);

				switch (header) {
				case VC4_PACKET_HALT:
				case VC4_PACKET_STORE_MS_TILE_BUFFER_AND_EOF:
						return;
				default:
						break;
				}

				offset += length;
				if (header != VC4_PACKET_GEM_HANDLES)
						hw_offset += length;
				p += length;
		}

		clif_dump_destroy(clif);
}



////////////////////////////////////////////////////
////////////////////////////////////////////////////
/// just so we can return a function pointer, TODO
////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties(
	VkPhysicalDevice                            physicalDevice,
	VkFormat                                    format,
	VkImageType                                 type,
	VkSampleCountFlagBits                       samples,
	VkImageUsageFlags                           usage,
	VkImageTiling                               tiling,
	uint32_t*                                   pPropertyCount,
		VkSparseImageFormatProperties*              pProperties)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateInstanceLayerProperties(
	uint32_t*                                   pPropertyCount,
	VkLayerProperties*                          pProperties)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkDestroyQueryPool(
	VkDevice                                    device,
	VkQueryPool                                 queryPool,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdWaitEvents(
	VkCommandBuffer                             commandBuffer,
	uint32_t                                    eventCount,
	const VkEvent*                              pEvents,
	VkPipelineStageFlags                        srcStageMask,
	VkPipelineStageFlags                        dstStageMask,
	uint32_t                                    memoryBarrierCount,
	const VkMemoryBarrier*                      pMemoryBarriers,
	uint32_t                                    bufferMemoryBarrierCount,
	const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
	uint32_t                                    imageMemoryBarrierCount,
	const VkImageMemoryBarrier*                 pImageMemoryBarriers)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorSetLayout(
	VkDevice                                    device,
	const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkDescriptorSetLayout*                      pSetLayout)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexedIndirect(
	VkCommandBuffer                             commandBuffer,
	VkBuffer                                    buffer,
	VkDeviceSize                                offset,
	uint32_t                                    drawCount,
	uint32_t                                    stride)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkGetEventStatus(
	VkDevice                                    device,
	VkEvent                                     event)
{
	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorPool(
	VkDevice                                    device,
	const VkDescriptorPoolCreateInfo*           pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkDescriptorPool*                           pDescriptorPool)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorPool(
	VkDevice                                    device,
	VkDescriptorPool                            descriptorPool,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndirect(
	VkCommandBuffer                             commandBuffer,
	VkBuffer                                    buffer,
	VkDeviceSize                                offset,
	uint32_t                                    drawCount,
	uint32_t                                    stride)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdBindDescriptorSets(
	VkCommandBuffer                             commandBuffer,
	VkPipelineBindPoint                         pipelineBindPoint,
	VkPipelineLayout                            layout,
	uint32_t                                    firstSet,
	uint32_t                                    descriptorSetCount,
	const VkDescriptorSet*                      pDescriptorSets,
	uint32_t                                    dynamicOffsetCount,
	const uint32_t*                             pDynamicOffsets)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdPushConstants(
	VkCommandBuffer                             commandBuffer,
	VkPipelineLayout                            layout,
	VkShaderStageFlags                          stageFlags,
	uint32_t                                    offset,
	uint32_t                                    size,
	const void*                                 pValues)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdResolveImage(
	VkCommandBuffer                             commandBuffer,
	VkImage                                     srcImage,
	VkImageLayout                               srcImageLayout,
	VkImage                                     dstImage,
	VkImageLayout                               dstImageLayout,
	uint32_t                                    regionCount,
	const VkImageResolve*                       pRegions)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties(
	VkPhysicalDevice                            physicalDevice,
	VkFormat                                    format,
	VkImageType                                 type,
	VkImageTiling                               tiling,
	VkImageUsageFlags                           usage,
	VkImageCreateFlags                          flags,
	VkImageFormatProperties*                    pImageFormatProperties)
{
	assert(physicalDevice);
	assert(pImageFormatProperties);

	VkFormat ycbcrConversionRequiredFormats[] =
	{
	VK_FORMAT_G8B8G8R8_422_UNORM
	,VK_FORMAT_B8G8R8G8_422_UNORM
	,VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM
	,VK_FORMAT_G8_B8R8_2PLANE_420_UNORM
	,VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM
	,VK_FORMAT_G8_B8R8_2PLANE_422_UNORM
	,VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM
	,VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16
	,VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16
	,VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16
	,VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16
	,VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16
	,VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16
	,VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16
	,VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16
	,VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16
	,VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16
	,VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16
	,VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16
	,VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16
	,VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16
	,VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16
	,VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16
	,VK_FORMAT_G16B16G16R16_422_UNORM
	,VK_FORMAT_B16G16R16G16_422_UNORM
	,VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM
	,VK_FORMAT_G16_B16R16_2PLANE_420_UNORM
	,VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM
	,VK_FORMAT_G16_B16R16_2PLANE_422_UNORM
	,VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM
	};
	#define numYcbcrConversionRequiredFormats (sizeof(ycbcrConversionRequiredFormats)/sizeof(VkFormat))

	for(uint32_t c = 0; c < numUnsupportedFormats; ++c)
	{
		if(format == unsupportedFormats[c])
		{
			return VK_ERROR_FORMAT_NOT_SUPPORTED;
		}
	}

	pImageFormatProperties->maxArrayLayers = _limits.maxImageArrayLayers;

	pImageFormatProperties->maxExtent.width = 1;
	pImageFormatProperties->maxExtent.height = 1;
	pImageFormatProperties->maxExtent.depth = 1;

	pImageFormatProperties->sampleCounts = _limits.framebufferColorSampleCounts;

	if(type == VK_IMAGE_TYPE_1D)
	{
		pImageFormatProperties->maxExtent.width = _limits.maxImageDimension1D;
		pImageFormatProperties->maxMipLevels = ulog2(_limits.maxImageDimension1D) + 1;
	}
	else if(type == VK_IMAGE_TYPE_2D)
	{
		pImageFormatProperties->maxExtent.width = _limits.maxImageDimension2D;
		pImageFormatProperties->maxExtent.height = _limits.maxImageDimension2D;
		pImageFormatProperties->maxMipLevels = ulog2(_limits.maxImageDimension2D) + 1;
	}
	else
	{
		pImageFormatProperties->maxExtent.width = _limits.maxImageDimension3D;
		pImageFormatProperties->maxExtent.height = _limits.maxImageDimension3D;
		pImageFormatProperties->maxExtent.depth = _limits.maxImageDimension3D;
		pImageFormatProperties->maxMipLevels = ulog2(_limits.maxImageDimension3D) + 1;
	}

	int ycbcrConversionRequired = 0;

	for(uint32_t c = 0; c < numYcbcrConversionRequiredFormats; ++c)
	{
		if(format == ycbcrConversionRequiredFormats[c])
		{
			ycbcrConversionRequired = 1;
			break;
		}
	}

	if(ycbcrConversionRequired ||
	   tiling == VK_IMAGE_TILING_LINEAR ||
	   type != VK_IMAGE_TYPE_2D ||
	   flags & VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ||
	   flags & VK_IMAGE_CREATE_SPARSE_BINDING_BIT
	   )
	{
		pImageFormatProperties->sampleCounts = VK_SAMPLE_COUNT_1_BIT;
	}

	//TODO real max size?
	//2^31
	pImageFormatProperties->maxResourceSize = 1<<31;

	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdEndQuery(
	VkCommandBuffer                             commandBuffer,
	VkQueryPool                                 queryPool,
	uint32_t                                    query)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateSampler(
	VkDevice                                    device,
	const VkSamplerCreateInfo*                  pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkSampler*                                  pSampler)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdExecuteCommands(
	VkCommandBuffer                             commandBuffer,
	uint32_t                                    commandBufferCount,
	const VkCommandBuffer*                      pCommandBuffers)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkMergePipelineCaches(
	VkDevice                                    device,
	VkPipelineCache                             dstCache,
	uint32_t                                    srcCacheCount,
	const VkPipelineCache*                      pSrcCaches)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdDispatchIndirect(
	VkCommandBuffer                             commandBuffer,
	VkBuffer                                    buffer,
	VkDeviceSize                                offset)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPipelineCacheData(
	VkDevice                                    device,
	VkPipelineCache                             pipelineCache,
	size_t*                                     pDataSize,
	void*                                       pData)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineCache(
	VkDevice                                    device,
	VkPipelineCache                             pipelineCache,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR void VKAPI_CALL vkDestroyEvent(
	VkDevice                                    device,
	VkEvent                                     event,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdResetEvent(
	VkCommandBuffer                             commandBuffer,
	VkEvent                                     event,
	VkPipelineStageFlags                        stageMask)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdBeginQuery(
	VkCommandBuffer                             commandBuffer,
	VkQueryPool                                 queryPool,
	uint32_t                                    query,
	VkQueryControlFlags                         flags)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkResetDescriptorPool(
	VkDevice                                    device,
	VkDescriptorPool                            descriptorPool,
	VkDescriptorPoolResetFlags                  flags)
{
	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateEvent(
	VkDevice                                    device,
	const VkEventCreateInfo*                    pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkEvent*                                    pEvent)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdCopyBufferToImage(
	VkCommandBuffer                             commandBuffer,
	VkBuffer                                    srcBuffer,
	VkImage                                     dstImage,
	VkImageLayout                               dstImageLayout,
	uint32_t                                    regionCount,
	const VkBufferImageCopy*                    pRegions)
{

}

VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSets(
	VkDevice                                    device,
	uint32_t                                    descriptorWriteCount,
	const VkWriteDescriptorSet*                 pDescriptorWrites,
	uint32_t                                    descriptorCopyCount,
	const VkCopyDescriptorSet*                  pDescriptorCopies)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdBlitImage(
	VkCommandBuffer                             commandBuffer,
	VkImage                                     srcImage,
	VkImageLayout                               srcImageLayout,
	VkImage                                     dstImage,
	VkImageLayout                               dstImageLayout,
	uint32_t                                    regionCount,
	const VkImageBlit*                          pRegions,
	VkFilter                                    filter)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineLayout(
	VkDevice                                    device,
	const VkPipelineLayoutCreateInfo*           pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkPipelineLayout*                           pPipelineLayout)
{
	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkAllocateDescriptorSets(
	VkDevice                                    device,
	const VkDescriptorSetAllocateInfo*          pAllocateInfo,
	VkDescriptorSet*                            pDescriptorSets)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkDestroySampler(
	VkDevice                                    device,
	VkSampler                                   sampler,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateComputePipelines(
	VkDevice                                    device,
	VkPipelineCache                             pipelineCache,
	uint32_t                                    createInfoCount,
	const VkComputePipelineCreateInfo*          pCreateInfos,
	const VkAllocationCallbacks*                pAllocator,
	VkPipeline*                                 pPipelines)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdDrawIndexed(
	VkCommandBuffer                             commandBuffer,
	uint32_t                                    indexCount,
	uint32_t                                    instanceCount,
	uint32_t                                    firstIndex,
	int32_t                                     vertexOffset,
	uint32_t                                    firstInstance)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdWriteTimestamp(
	VkCommandBuffer                             commandBuffer,
	VkPipelineStageFlagBits                     pipelineStage,
	VkQueryPool                                 queryPool,
	uint32_t                                    query)
{

}

VKAPI_ATTR void VKAPI_CALL vkDestroyPipelineLayout(
	VkDevice                                    device,
	VkPipelineLayout                            pipelineLayout,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkResetEvent(
	VkDevice                                    device,
	VkEvent                                     event)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdCopyQueryPoolResults(
	VkCommandBuffer                             commandBuffer,
	VkQueryPool                                 queryPool,
	uint32_t                                    firstQuery,
	uint32_t                                    queryCount,
	VkBuffer                                    dstBuffer,
	VkDeviceSize                                dstOffset,
	VkDeviceSize                                stride,
	VkQueryResultFlags                          flags)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdCopyImageToBuffer(
	VkCommandBuffer                             commandBuffer,
	VkImage                                     srcImage,
	VkImageLayout                               srcImageLayout,
	VkBuffer                                    dstBuffer,
	uint32_t                                    regionCount,
	const VkBufferImageCopy*                    pRegions)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdCopyImage(
	VkCommandBuffer                             commandBuffer,
	VkImage                                     srcImage,
	VkImageLayout                               srcImageLayout,
	VkImage                                     dstImage,
	VkImageLayout                               dstImageLayout,
	uint32_t                                    regionCount,
	const VkImageCopy*                          pRegions)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdDispatch(
	VkCommandBuffer                             commandBuffer,
	uint32_t                                    groupCountX,
	uint32_t                                    groupCountY,
	uint32_t                                    groupCountZ)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties(
	VkPhysicalDevice                            physicalDevice,
	VkFormat                                    format,
	VkFormatProperties*                         pFormatProperties)
{
	assert(physicalDevice);
	assert(pFormatProperties);

	if(isDepthStencilFormat(format) && format != VK_FORMAT_S8_UINT)
	{
		pFormatProperties->linearTilingFeatures = 0
												| VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT
												| VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT
												| VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT
												| VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
												| VK_FORMAT_FEATURE_BLIT_SRC_BIT
												| VK_FORMAT_FEATURE_BLIT_DST_BIT
												| VK_FORMAT_FEATURE_TRANSFER_SRC_BIT
												| VK_FORMAT_FEATURE_TRANSFER_DST_BIT
												;
		pFormatProperties->optimalTilingFeatures = 0
												| VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT
												| VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT
												| VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT
												| VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
												| VK_FORMAT_FEATURE_BLIT_SRC_BIT
												| VK_FORMAT_FEATURE_BLIT_DST_BIT
												| VK_FORMAT_FEATURE_TRANSFER_SRC_BIT
												| VK_FORMAT_FEATURE_TRANSFER_DST_BIT
												;
		pFormatProperties->bufferFeatures = 0
												| VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT
												| VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT
												| VK_FORMAT_FEATURE_TRANSFER_SRC_BIT
												| VK_FORMAT_FEATURE_TRANSFER_DST_BIT
												;
	}
	else
	{
		pFormatProperties->linearTilingFeatures = 0
												| VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT
												| VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT
												| VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT
												| VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT
												| VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT
												| VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT
												| VK_FORMAT_FEATURE_BLIT_SRC_BIT
												| VK_FORMAT_FEATURE_BLIT_DST_BIT
												| VK_FORMAT_FEATURE_TRANSFER_SRC_BIT
												| VK_FORMAT_FEATURE_TRANSFER_DST_BIT
												;
		pFormatProperties->optimalTilingFeatures = 0
												| VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT
												| VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT
												| VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT
												| VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT
												| VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT
												| VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT
												| VK_FORMAT_FEATURE_BLIT_SRC_BIT
												| VK_FORMAT_FEATURE_BLIT_DST_BIT
												| VK_FORMAT_FEATURE_TRANSFER_SRC_BIT
												| VK_FORMAT_FEATURE_TRANSFER_DST_BIT
												;
		pFormatProperties->bufferFeatures = 0
												| VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT
												| VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT
												| VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT
												| VK_FORMAT_FEATURE_TRANSFER_SRC_BIT
												| VK_FORMAT_FEATURE_TRANSFER_DST_BIT
												;
	}
}

VKAPI_ATTR VkResult VKAPI_CALL vkEnumerateDeviceLayerProperties(
	VkPhysicalDevice                            physicalDevice,
	uint32_t*                                   pPropertyCount,
	VkLayerProperties*                          pProperties)
{
	//deprecated, just return instance layers
	return vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorSetLayout(
	VkDevice                                    device,
	VkDescriptorSetLayout                       descriptorSetLayout,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkGetQueryPoolResults(
	VkDevice                                    device,
	VkQueryPool                                 queryPool,
	uint32_t                                    firstQuery,
	uint32_t                                    queryCount,
	size_t                                      dataSize,
	void*                                       pData,
	VkDeviceSize                                stride,
	VkQueryResultFlags                          flags)
{
	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkFreeDescriptorSets(
	VkDevice                                    device,
	VkDescriptorPool                            descriptorPool,
	uint32_t                                    descriptorSetCount,
	const VkDescriptorSet*                      pDescriptorSets)
{
	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkSetEvent(
	VkDevice                                    device,
	VkEvent                                     event)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdSetEvent(
	VkCommandBuffer                             commandBuffer,
	VkEvent                                     event,
	VkPipelineStageFlags                        stageMask)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetDeviceMemoryCommitment(
	VkDevice                                    device,
	VkDeviceMemory                              memory,
	VkDeviceSize*                               pCommittedMemoryInBytes)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkCreatePipelineCache(
	VkDevice                                    device,
	const VkPipelineCacheCreateInfo*            pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkPipelineCache*                            pPipelineCache)
{
	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateQueryPool(
	VkDevice                                    device,
	const VkQueryPoolCreateInfo*                pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkQueryPool*                                pQueryPool)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkCmdResetQueryPool(
	VkCommandBuffer                             commandBuffer,
	VkQueryPool                                 queryPool,
	uint32_t                                    firstQuery,
	uint32_t                                    queryCount)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetImageSubresourceLayout(
	VkDevice                                    device,
	VkImage                                     image,
	const VkImageSubresource*                   pSubresource,
	VkSubresourceLayout*                        pLayout)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdCopyBuffer(
	VkCommandBuffer                             commandBuffer,
	VkBuffer                                    srcBuffer,
	VkBuffer                                    dstBuffer,
	uint32_t                                    regionCount,
	const VkBufferCopy*                         pRegions)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements(
	VkDevice                                    device,
	VkImage                                     image,
	uint32_t*                                   pSparseMemoryRequirementCount,
	VkSparseImageMemoryRequirements*            pSparseMemoryRequirements)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkQueueBindSparse(
	VkQueue                                     queue,
	uint32_t                                    bindInfoCount,
	const VkBindSparseInfo*                     pBindInfo,
	VkFence                                     fence)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures2(
	VkPhysicalDevice                            physicalDevice,
	VkPhysicalDeviceFeatures2*                  pFeatures)
{
	assert(physicalDevice);
	assert(pFeatures);
	vkGetPhysicalDeviceFeatures(physicalDevice, &pFeatures->features);
}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties2(
	VkPhysicalDevice                            physicalDevice,
	VkPhysicalDeviceProperties2*                pProperties)
{
	assert(physicalDevice);
	assert(pProperties);
	vkGetPhysicalDeviceProperties(physicalDevice, &pProperties->properties);

	if(pProperties->pNext)
	{
		VkPhysicalDeviceDriverPropertiesKHR* ptr = pProperties->pNext;
		if(ptr->sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DRIVER_PROPERTIES_KHR)
		{
			//TODO apparently can't expose my own ID :(
			//has to be "known"
			ptr->driverID = VK_DRIVER_ID_MESA_RADV_KHR;
			const char* driverName = "RPi VK";
			const char* driverInfo = ""; //TODO maybe version number, git info?
			strcpy(ptr->driverName, driverName);
			strcpy(ptr->driverInfo, driverInfo);
			//TODO this is what we are aspiring to pass...
			ptr->conformanceVersion.major = 1;
			ptr->conformanceVersion.minor = 1;
			ptr->conformanceVersion.subminor = 2;
			ptr->conformanceVersion.patch = 1;
		}
	}
}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFormatProperties2(
	VkPhysicalDevice                            physicalDevice,
	VkFormat                                    format,
	VkFormatProperties2*                        pFormatProperties)
{
	assert(physicalDevice);
	assert(pFormatProperties);
	vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &pFormatProperties->formatProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL vkGetPhysicalDeviceImageFormatProperties2(
	VkPhysicalDevice                            physicalDevice,
	const VkPhysicalDeviceImageFormatInfo2*     pImageFormatInfo,
	VkImageFormatProperties2*                   pImageFormatProperties)
{
	assert(physicalDevice);
	assert(pImageFormatProperties);
	assert(pImageFormatInfo);

	//TODO

	return vkGetPhysicalDeviceImageFormatProperties(physicalDevice,
													pImageFormatInfo->format,
													pImageFormatInfo->type,
													pImageFormatInfo->tiling,
													pImageFormatInfo->usage,
													pImageFormatInfo->flags,
													&pImageFormatProperties->imageFormatProperties);
}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceQueueFamilyProperties2(
	VkPhysicalDevice                            physicalDevice,
	uint32_t*                                   pQueueFamilyPropertyCount,
	VkQueueFamilyProperties2*                   pQueueFamilyProperties)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceMemoryProperties2(
	VkPhysicalDevice                            physicalDevice,
	VkPhysicalDeviceMemoryProperties2*          pMemoryProperties)
{

}


VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceSparseImageFormatProperties2(
	VkPhysicalDevice                            physicalDevice,
	const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
	uint32_t*                                   pPropertyCount,
	VkSparseImageFormatProperties2*             pProperties)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalBufferProperties(
	VkPhysicalDevice                            physicalDevice,
	const VkPhysicalDeviceExternalBufferInfo*   pExternalBufferInfo,
	VkExternalBufferProperties*                 pExternalBufferProperties)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalFenceProperties(
	VkPhysicalDevice                            physicalDevice,
	const VkPhysicalDeviceExternalFenceInfo*    pExternalFenceInfo,
	VkExternalFenceProperties*                  pExternalFenceProperties)
{

}


VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceExternalSemaphoreProperties(
	VkPhysicalDevice                            physicalDevice,
	const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
	VkExternalSemaphoreProperties*              pExternalSemaphoreProperties)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkBindImageMemory2(
	VkDevice                                    device,
	uint32_t                                    bindInfoCount,
	const VkBindImageMemoryInfo*                pBindInfos)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkGetDeviceGroupPeerMemoryFeatures(
	VkDevice                                    device,
	uint32_t                                    heapIndex,
	uint32_t                                    localDeviceIndex,
	uint32_t                                    remoteDeviceIndex,
	VkPeerMemoryFeatureFlags*                   pPeerMemoryFeatures)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdSetDeviceMask(
	VkCommandBuffer                             commandBuffer,
	uint32_t                                    deviceMask)
{

}

VKAPI_ATTR void VKAPI_CALL vkCmdDispatchBase(
	VkCommandBuffer                             commandBuffer,
	uint32_t                                    baseGroupX,
	uint32_t                                    baseGroupY,
	uint32_t                                    baseGroupZ,
	uint32_t                                    groupCountX,
	uint32_t                                    groupCountY,
	uint32_t                                    groupCountZ)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetImageMemoryRequirements2(
	VkDevice                                    device,
	const VkImageMemoryRequirementsInfo2*       pInfo,
	VkMemoryRequirements2*                      pMemoryRequirements)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetBufferMemoryRequirements2(
	VkDevice                                    device,
	const VkBufferMemoryRequirementsInfo2*      pInfo,
	VkMemoryRequirements2*                      pMemoryRequirements)
{
	assert(device);
	assert(pInfo);
	assert(pMemoryRequirements);

	vkGetBufferMemoryRequirements(device, pInfo->buffer, &pMemoryRequirements->memoryRequirements);
}

VKAPI_ATTR void VKAPI_CALL vkGetImageSparseMemoryRequirements2(
	VkDevice                                    device,
	const VkImageSparseMemoryRequirementsInfo2* pInfo,
	uint32_t*                                   pSparseMemoryRequirementCount,
	VkSparseImageMemoryRequirements2*           pSparseMemoryRequirements)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetDeviceQueue2(
	VkDevice                                    device,
	const VkDeviceQueueInfo2*                   pQueueInfo,
	VkQueue*                                    pQueue)
{
	assert(device);
	assert(pQueueInfo);
	assert(pQueue);

	vkGetDeviceQueue(device, pQueueInfo->queueFamilyIndex, pQueueInfo->queueIndex, pQueue);
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateSamplerYcbcrConversion(
	VkDevice                                    device,
	const VkSamplerYcbcrConversionCreateInfo*   pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkSamplerYcbcrConversion*                   pYcbcrConversion)
{
	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vkDestroySamplerYcbcrConversion(
	VkDevice                                    device,
	VkSamplerYcbcrConversion                    ycbcrConversion,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDescriptorUpdateTemplate(
	VkDevice                                    device,
	const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkDescriptorUpdateTemplate*                 pDescriptorUpdateTemplate)
{

}

VKAPI_ATTR void VKAPI_CALL vkDestroyDescriptorUpdateTemplate(
	VkDevice                                    device,
	VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
	const VkAllocationCallbacks*                pAllocator)
{

}

VKAPI_ATTR void VKAPI_CALL vkUpdateDescriptorSetWithTemplate(
	VkDevice                                    device,
	VkDescriptorSet                             descriptorSet,
	VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
	const void*                                 pData)
{

}

VKAPI_ATTR void VKAPI_CALL vkGetDescriptorSetLayoutSupport(
	VkDevice                                    device,
	const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
	VkDescriptorSetLayoutSupport*               pSupport)
{

}

VKAPI_ATTR VkResult VKAPI_CALL vkBindBufferMemory2(
	VkDevice                                    device,
	uint32_t                                    bindInfoCount,
	const VkBindBufferMemoryInfo*               pBindInfos)
{
	return VK_SUCCESS;
}
