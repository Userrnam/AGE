#include "Descriptor.hpp"
#include "Pool.hpp"
#include "Core.hpp"

namespace age {

VkDescriptorSetLayout createDescriptorSetLayout(DescriptorLayout& info) {
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	uint32_t bindingIndex = 0;
	for (auto& binding : info.layoutInfo) {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = bindingIndex;
		uboLayoutBinding.descriptorCount = binding.info.count;
		uboLayoutBinding.descriptorType = static_cast<VkDescriptorType>(binding.info.type);
		uboLayoutBinding.stageFlags = binding.info.stage;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		bindings.push_back(uboLayoutBinding);

		bindingIndex++;
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(core::apiCore.device, &layoutInfo, nullptr, &info.layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}

	core::apiCore.descriptor.layouts.push_back(info);

    return core::apiCore.descriptor.layouts[core::apiCore.descriptor.layouts.size()-1].layout;
}

bool inVector(const DescriptorLayoutInfo& info, const std::vector<DescriptorLayoutInfo>& v) {
	for (auto& i : v) {
		if (i.description == info.description) {
			return true;
		}
	}
	return false;
}

inline bool descriptorSetLayoutsSame(const DescriptorLayout& layout1, const DescriptorLayout& layout2) {
	if (layout1.layoutInfo.size() != layout2.layoutInfo.size()) {
		return false;
	}

	for (size_t i = 0; i < layout1.layoutInfo.size(); ++i) {
		if (layout1.layoutInfo[i].description != layout2.layoutInfo[i].description) {
			return false;
		}
	}

	return true;
}

VkDescriptorSetLayout requestDescriptorSetLayout(DescriptorLayout& requiredLayout) {
    for (auto& layout : core::apiCore.descriptor.layouts) {
        if (descriptorSetLayoutsSame(layout, requiredLayout)) {
            return layout.layout;
        }
    }
    return createDescriptorSetLayout(requiredLayout);
}

// fix pool managment
// also allocates sets
core::Pool& createDescriptorPool(uint32_t uboCount, uint32_t samplerCount, uint32_t setCount) {
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
	if (vkCreateDescriptorPool(core::apiCore.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool");
	}

	core::Pool pool;
	pool.pool = descriptorPool;
	pool.remainingSize = setCount;
	pool.sets.resize(setCount);

	core::apiCore.descriptor.pools.push_back(pool);

    return core::apiCore.descriptor.pools.back();
}

VkDescriptorSet Descriptor::requestDescriptorSet(const DescriptorInfo& info) {
    for (auto& pool : core::apiCore.descriptor.pools) {
        if (pool.layout == m_layout) {
			if (pool.remainingSize > 0) {
				pool.remainingSize--;
				return pool.sets[pool.remainingSize];
			}
			if (pool.freeIndicies.size() > 0) {
				uint32_t freeIndex = pool.freeIndicies.back();
				pool.freeIndicies.pop_back();
				return pool.sets[freeIndex];
			}
        }
		m_poolIndex++;
    }
	return createDescriptorSets(info);
}

VkDescriptorSet Descriptor::createDescriptorSets(const DescriptorInfo& info) {
    std::vector<VkDescriptorSetLayout> layouts(info.m_setCount, m_layout);

	uint32_t totalBufferSize = 0;
	uint32_t totalSamplerSize = 0;

	for (auto& binding : info.m_ubosBindings) {
		totalBufferSize += binding.getBuffers().size();
	}

	for (auto& binding : info.m_texturesBindings) {
		totalSamplerSize += binding.getTextures().size();
	}

	auto& pool = createDescriptorPool(totalBufferSize, totalSamplerSize, info.m_setCount);
	pool.layout = m_layout;

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool.pool;
	allocInfo.descriptorSetCount = layouts.size();
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(core::apiCore.device, &allocInfo, pool.sets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets");
	}

	pool.remainingSize--;
	return pool.sets[pool.remainingSize];
}

Descriptor& Descriptor::get(const DescriptorInfo& info) {
	DescriptorLayout layout;

	layout.layoutInfo.resize(info.m_ubosBindings.size() + info.m_texturesBindings.size());
	uint32_t bindingIndex;

	for (bindingIndex = 0; bindingIndex < info.m_ubosBindings.size(); ++bindingIndex) {
		layout.layoutInfo[bindingIndex].info.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		layout.layoutInfo[bindingIndex].info.count = info.m_ubosBindings[bindingIndex].getBuffers().size();
		layout.layoutInfo[bindingIndex].info.stage = info.m_ubosBindings[bindingIndex].getStage();
	}

	for (size_t i = 0; i < info.m_texturesBindings.size(); ++i) {
		layout.layoutInfo[bindingIndex].info.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		layout.layoutInfo[bindingIndex].info.count = info.m_texturesBindings[i].getTextures().size();
		layout.layoutInfo[bindingIndex].info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

		bindingIndex++;
	}

	m_layout = requestDescriptorSetLayout(layout);
	m_set = requestDescriptorSet(info);

	uint32_t writeDescriptorIndex = 0;
	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	descriptorWrites.resize(info.m_ubosBindings.size() + info.m_texturesBindings.size());

	std::vector<std::vector<VkDescriptorBufferInfo>> bufferInfos;
	bufferInfos.resize(info.m_ubosBindings.size());
	for (size_t i = 0; i < info.m_ubosBindings.size(); ++i) {
		bufferInfos[i].resize(info.m_ubosBindings[i].getBuffers().size());
		for (size_t j = 0; j < info.m_ubosBindings[i].getBuffers().size(); ++j) {
			auto& ubo = info.m_ubosBindings[i].getBuffers()[j];

			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = ubo->getBuffer();
			bufferInfo.range = ubo->getSize();
			bufferInfos[i][j] = bufferInfo;
		}

		descriptorWrites[writeDescriptorIndex] = {};
		descriptorWrites[writeDescriptorIndex].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[writeDescriptorIndex].dstSet = m_set;
		descriptorWrites[writeDescriptorIndex].dstBinding = writeDescriptorIndex;
		descriptorWrites[writeDescriptorIndex].dstArrayElement = 0;
		descriptorWrites[writeDescriptorIndex].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[writeDescriptorIndex].descriptorCount = bufferInfos[i].size();
		descriptorWrites[writeDescriptorIndex].pBufferInfo = bufferInfos[i].data();

		writeDescriptorIndex++;
	}

	std::vector<std::vector<VkDescriptorImageInfo>> imageInfos;
	imageInfos.resize(info.m_texturesBindings.size());
	for (size_t i = 0; i < info.m_texturesBindings.size(); ++i) {
		imageInfos[i].resize(info.m_texturesBindings[i].getTextures().size());
		for (size_t j = 0; j < info.m_texturesBindings[i].getTextures().size(); ++j) {
			auto& image = info.m_texturesBindings[i].getTextures()[j];

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = image->getImage().getView();
			imageInfo.sampler = image->getSampler();
			imageInfos[i][j] = imageInfo;
		}

		descriptorWrites[writeDescriptorIndex] = {};
		descriptorWrites[writeDescriptorIndex].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[writeDescriptorIndex].dstSet = m_set;
		descriptorWrites[writeDescriptorIndex].dstBinding = writeDescriptorIndex;
		descriptorWrites[writeDescriptorIndex].dstArrayElement = 0;
		descriptorWrites[writeDescriptorIndex].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[writeDescriptorIndex].descriptorCount = imageInfos[i].size();
		descriptorWrites[writeDescriptorIndex].pImageInfo = imageInfos[i].data();
	}

	vkUpdateDescriptorSets(core::apiCore.device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

	return *this;
}

void freeDescriptor(uint32_t poolIndex, VkDescriptorSet descriptor) {
	auto& pool = core::apiCore.descriptor.pools[poolIndex];
    for (size_t i = 0; i < pool.sets.size(); ++i) {
        if (pool.sets[i] == descriptor) {
			pool.freeIndicies.push_back(i);
            return;
        }
    }
}

} // namespace age
