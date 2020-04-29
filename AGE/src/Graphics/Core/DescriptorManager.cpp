#include <vector>

#include "DescriptorManager.hpp"

#include "Core.hpp"


namespace age::core {

VkDescriptorPool requestDescriptorPool(uint32_t uboCount, uint32_t samplerCount) {
    for (auto& pool : apiCore.descriptor.pools) {
        if (pool.uboCount >= uboCount && pool.samplerCount >= samplerCount) {
            return pool.pool;
        }
    }
    return createDescriptorPool(uboCount, samplerCount);
}

VkDescriptorSetLayout requestDescriptorSetLayout(uint32_t uboCount, uint32_t samplerCount) {
    for (auto& layout : apiCore.descriptor.layouts) {
        if (layout.uboCount == uboCount && layout.samplerCount == samplerCount) {
            return layout.layout;
        }
    }
    return createDescriptorSetLayout(uboCount, samplerCount);
}

VkDescriptorPool createDescriptorPool(uint32_t uboCount, uint32_t samplerCount) {
	VkDescriptorPoolSize poolSizes[2];
	
	uint32_t i = 0;
	if (uboCount) {
		poolSizes[i].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[i].descriptorCount = uboCount;
		i++;
	}
	if (samplerCount) {
		poolSizes[i].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[i].descriptorCount = samplerCount;
		i++;
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = i;
	poolInfo.pPoolSizes = poolSizes;
	poolInfo.maxSets = 1;

	VkDescriptorPool descriptorPool;
	if (vkCreateDescriptorPool(apiCore.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool");
	}

	apiCore.descriptor.pools.push_back({descriptorPool, uboCount, samplerCount});

    return descriptorPool;
}

VkDescriptorSetLayout createDescriptorSetLayout(uint32_t uboCount, uint32_t samplerCount) {
	uint32_t binding = 0;
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	if (uboCount > 0) {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = binding;
		uboLayoutBinding.descriptorCount = uboCount;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		bindings.push_back(uboLayoutBinding);
		binding++;
	}

	if (samplerCount > 0) {
		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = samplerCount;
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
	apiCore.descriptor.layouts.push_back({descriptorSetLayout, uboCount, samplerCount});

    return apiCore.descriptor.layouts[apiCore.descriptor.layouts.size()-1].layout;
}

}
