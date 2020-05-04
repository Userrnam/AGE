#include <vector>
#include <array>
#include <utility>

#include "DescriptorManager.hpp"

#include "Core.hpp"

namespace age::core {

// !! Layouts must be same
VkDescriptorSet requestDescriptorSet(const DescriptorInfo& info, VkDescriptorSetLayout layout) {
	for (auto& pool : apiCore.descriptor.pools) {
        if (pool.layout == layout && pool.remainingSize > 0) {
			pool.remainingSize--;
			return pool.sets[pool.remainingSize];
        }
    }
	return createDescriptorSets(info, layout);
}

VkDescriptorSet createDescriptorSets(const DescriptorInfo& info, VkDescriptorSetLayout layout) {
	std::vector<VkDescriptorSetLayout> layouts(info.setCount, layout);

	auto& pool = createDescriptorPool(info.ubos.size(), info.samplers.size(), info.setCount);
	pool.layout = layout;

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool.pool;
	allocInfo.descriptorSetCount = layouts.size();
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(apiCore.device, &allocInfo, pool.sets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets");
	}

	pool.remainingSize--;
	return pool.sets[pool.remainingSize];
}

Descriptor getDescriptor(const DescriptorInfo& info) {
	Descriptor out;
	out.layouts.resize(1);
	out.sets.resize(1);

	out.layouts[0] = requestDescriptorSetLayout(info);
	out.sets[0] = requestDescriptorSet(info, out.layouts[0]);

	std::vector<VkDescriptorBufferInfo> bufferInfos;
	for (auto& ubo : info.ubos) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = ubo.buffer;
		bufferInfo.range = ubo.size;
		bufferInfos.push_back(bufferInfo);
	}

	std::vector<VkDescriptorImageInfo> imageInfos;
	for (auto& sampler : info.samplers) {
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = sampler.view;
		imageInfo.sampler = sampler.sampler;
		imageInfos.push_back(imageInfo);
	}

	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	if (info.ubos.size() != 0 && info.samplers.size() != 0) {
		descriptorWrites.resize(2);
	} else {
		descriptorWrites.resize(1);
	}

	uint32_t i = 0;
	if (info.ubos.size() != 0) {
		descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[i].dstSet = out.sets[0];
		descriptorWrites[i].dstBinding = info.ubosBinding;
		descriptorWrites[i].dstArrayElement = 0;
		descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[i].descriptorCount = bufferInfos.size();
		descriptorWrites[i].pBufferInfo = bufferInfos.data();
		i++;
	}

	if (info.samplers.size() != 0) {
		descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[i].dstSet = out.sets[0];
		descriptorWrites[i].dstBinding = info.samplersBinding;
		descriptorWrites[i].dstArrayElement = 0;
		descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[i].descriptorCount = imageInfos.size();
		descriptorWrites[i].pImageInfo = imageInfos.data();
	}

	vkUpdateDescriptorSets(apiCore.device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

	return out;
}

// VkDescriptorPool requestDescriptorPool(uint32_t uboCount, uint32_t samplerCount, uint32_t setCount) {
//     for (auto& pool : apiCore.descriptor.pools) {
//         if (pool.uboCount >= uboCount && pool.samplerCount >= samplerCount && pool.remainingSize > 0) {
// 			pool.remainingSize--;
//             return pool.pool;
//         }
//     }
//     return createDescriptorPool(uboCount, samplerCount, setCount);
// }

// FIXME:
VkDescriptorSetLayout requestDescriptorSetLayout(const DescriptorInfo& info) {
    for (auto& layout : apiCore.descriptor.layouts) {
        if (layout.samplerBinding == info.samplersBinding && layout.samplerCount == info.samplers.size() &&
			layout.uboBinding == info.ubosBinding && layout.uboCount == info.ubos.size()) {
            return layout.layout;
        }
    }
    return createDescriptorSetLayout(info);
}

// also allocates sets
Pool& createDescriptorPool(uint32_t uboCount, uint32_t samplerCount, uint32_t setCount) {
	VkDescriptorPoolSize poolSizes[2];
	
	uint32_t i = 0;
	if (uboCount) {
		poolSizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[i].descriptorCount = setCount * uboCount;
		i++;
	}
	if (samplerCount) {
		poolSizes[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[i].descriptorCount = setCount * samplerCount;
		i++;
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = i;
	poolInfo.pPoolSizes = poolSizes;
	poolInfo.maxSets = setCount;

	VkDescriptorPool descriptorPool;
	if (vkCreateDescriptorPool(apiCore.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool");
	}

	Pool pool;
	pool.pool = descriptorPool;
	pool.remainingSize = setCount;
	pool.sets.resize(setCount);

	apiCore.descriptor.pools.push_back(pool);

    return apiCore.descriptor.pools.back();
}

VkDescriptorSetLayout createDescriptorSetLayout(const DescriptorInfo& info) {
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	if (info.ubos.size() > 0) {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = info.ubosBinding;
		uboLayoutBinding.descriptorCount = info.ubos.size();
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		bindings.push_back(uboLayoutBinding);
	}

	if (info.samplers.size() > 0) {
		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = info.samplersBinding;
		samplerLayoutBinding.descriptorCount = info.samplers.size();
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		bindings.push_back(samplerLayoutBinding);
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	VkDescriptorSetLayout descriptorSetLayout;
	if (vkCreateDescriptorSetLayout(apiCore.device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}

	DescriptorLayout dl;
	dl.layout = descriptorSetLayout;
	dl.uboBinding = info.ubosBinding;
	dl.uboCount = info.ubos.size();
	dl.samplerBinding = info.samplersBinding;
	dl.samplerCount = info.samplers.size();

	apiCore.descriptor.layouts.push_back(dl);

    return apiCore.descriptor.layouts[apiCore.descriptor.layouts.size()-1].layout;
}

}
