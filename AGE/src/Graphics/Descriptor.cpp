#include "Descriptor.hpp"
#include "Pool.hpp"
#include "Core.hpp"

namespace age {

VkDescriptorSetLayout createDescriptorSetLayout(const DescriptorInfo& info) {
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	if (info.m_ubos.size() > 0) {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = info.m_ubos.size();
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		bindings.push_back(uboLayoutBinding);
	}

	if (info.m_textures.size() > 0) {
		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = info.m_textures.size();
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
	if (vkCreateDescriptorSetLayout(core::apiCore.device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}

	DescriptorLayout dl;
	dl.layout = descriptorSetLayout;
	dl.uboBinding = 0;
	dl.uboCount = info.m_ubos.size();
	dl.samplerBinding = 1;
	dl.samplerCount = info.m_textures.size();

	core::apiCore.descriptor.layouts.push_back(dl);

    return core::apiCore.descriptor.layouts[core::apiCore.descriptor.layouts.size()-1].layout;
}

VkDescriptorSetLayout requestDescriptorSetLayout(const DescriptorInfo& info) {
    for (auto& layout : core::apiCore.descriptor.layouts) {
        if (layout.samplerCount == info.m_textures.size() && layout.uboCount == info.m_ubos.size()) {
            return layout.layout;
        }
    }
    return createDescriptorSetLayout(info);
}

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
                m_pool = &pool;
				return pool.sets[pool.remainingSize];
			}
			if (pool.freeIndicies.size() > 0) {
				uint32_t freeIndex = pool.freeIndicies.back();
				pool.freeIndicies.pop_back();
                m_pool = &pool;
				return pool.sets[freeIndex];
			}
        }
    }
	return createDescriptorSets(info);
}

VkDescriptorSet Descriptor::createDescriptorSets(const DescriptorInfo& info) {
    std::vector<VkDescriptorSetLayout> layouts(info.m_setCount, m_layout);

	auto& pool = createDescriptorPool(info.m_ubos.size(), info.m_textures.size(), info.m_setCount);
	pool.layout = m_layout;

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool.pool;
	allocInfo.descriptorSetCount = layouts.size();
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(core::apiCore.device, &allocInfo, pool.sets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets");
	}

    m_pool = &pool;
	pool.remainingSize--;
	return pool.sets[pool.remainingSize];
}

Descriptor& Descriptor::get(const DescriptorInfo& info) {
	m_layout = requestDescriptorSetLayout(info);
	m_set = requestDescriptorSet(info);

	std::vector<VkDescriptorBufferInfo> bufferInfos;
	for (auto& ubo : info.m_ubos) {
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = ubo->getBuffer();
		bufferInfo.range = ubo->getSize();
		bufferInfos.push_back(bufferInfo);
	}

	std::vector<VkDescriptorImageInfo> imageInfos;
	for (auto& sampler : info.m_textures) {
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = sampler->getImage().getView();
		imageInfo.sampler = sampler->getSampler();
		imageInfos.push_back(imageInfo);
	}

	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	if (info.m_ubos.size() != 0 && info.m_textures.size() != 0) {
		descriptorWrites.resize(2);
	} else {
		descriptorWrites.resize(1);
	}

	uint32_t i = 0;
	if (info.m_ubos.size() != 0) {
		descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[i].dstSet = m_set;
		descriptorWrites[i].dstBinding = 0;
		descriptorWrites[i].dstArrayElement = 0;
		descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[i].descriptorCount = bufferInfos.size();
		descriptorWrites[i].pBufferInfo = bufferInfos.data();
		i++;
	}

	if (info.m_textures.size() != 0) {
		descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[i].dstSet = m_set;
		descriptorWrites[i].dstBinding = 1;
		descriptorWrites[i].dstArrayElement = 0;
		descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[i].descriptorCount = imageInfos.size();
		descriptorWrites[i].pImageInfo = imageInfos.data();
	}

	vkUpdateDescriptorSets(core::apiCore.device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);

	return *this;
}

void freeDescriptor(void* pool, VkDescriptorSet descriptor) {
    auto p = reinterpret_cast<core::Pool*>(pool);
    for (size_t i = 0; i < p->sets.size(); ++i) {
        if (p->sets[i] == descriptor) {
            p->freeIndicies.push_back(i);
            return;
        }
    }
}

} // namespace age
