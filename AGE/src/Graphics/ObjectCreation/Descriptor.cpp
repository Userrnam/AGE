#include "Descriptor.hpp"
#include "Core/Pool.hpp"
#include "Core/Core.hpp"

namespace age {

VkDescriptorSetLayout createDescriptorSetLayout(DescriptorLayout& info) {
	std::vector<VkDescriptorSetLayoutBinding> bindings;

	uint32_t bindingIndex = 0;
	for (auto& binding : info.layoutInfo) {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = bindingIndex;
		uboLayoutBinding.descriptorCount = binding.info.count; // I think for age it's always 1
		uboLayoutBinding.descriptorType = static_cast<VkDescriptorType>(binding.info.type);
		uboLayoutBinding.stageFlags = binding.info.stage; // for age stage is determined by the type
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

	// TODO: simplify stored info about layout
	core::apiCore.descriptor.layouts.push_back(info);

    return core::apiCore.descriptor.layouts[core::apiCore.descriptor.layouts.size()-1].layout;
}

inline bool descriptorSetLayoutsSame(const DescriptorLayout& layout1, const DescriptorLayout& layout2) {
	if (layout1.layoutInfo.size() != layout2.layoutInfo.size()) {
		return false;
	}

	// Fixme: descriptions may be not in order
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

core::Pool& createDescriptorPool(std::vector<VkDescriptorPoolSize>& poolSizes, uint32_t setCount) {
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();
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

VkDescriptorSet DescriptorSet::requestDescriptorSet(const DescriptorSetInfo& info) {
    for (auto& pool : core::apiCore.descriptor.pools) {
		// not sure about it
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

VkDescriptorSet DescriptorSet::createDescriptorSets(const DescriptorSetInfo& info) {
    std::vector<VkDescriptorSetLayout> layouts(info.m_setCount, m_layout);
	std::vector<VkDescriptorPoolSize> poolSizes;

	for (auto& binding : info.m_bindings) {
		VkDescriptorPoolSize ps;
		ps.type = binding.getDescriptorType();
		ps.descriptorCount = binding.getDescriptors().size() * info.m_setCount;
		poolSizes.push_back(ps);
	}

	auto& pool = createDescriptorPool(poolSizes, info.m_setCount);
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

DescriptorSet& DescriptorSet::get(const DescriptorSetInfo& info) {
	DescriptorLayout layout;

	layout.layoutInfo.resize(info.m_bindings.size());
	for (uint32_t bindingIndex = 0; bindingIndex < info.m_bindings.size(); ++bindingIndex) {
		layout.layoutInfo[bindingIndex].info.type = info.m_bindings[bindingIndex].getDescriptorType();
		layout.layoutInfo[bindingIndex].info.count = info.m_bindings[bindingIndex].getDescriptors().size();
		layout.layoutInfo[bindingIndex].info.stage = info.m_bindings[bindingIndex].getStage();
	}

	m_layout = requestDescriptorSetLayout(layout);
	m_set = requestDescriptorSet(info);
	
	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	descriptorWrites.resize(info.m_bindings.size());

	// @Fixme
	std::vector<VkDescriptorBufferInfo> bufferInfos;
	std::vector<VkDescriptorImageInfo> imageInfos;
	bufferInfos.resize(128);
	imageInfos.resize(128);
	size_t imageIndex = 0;
	size_t bufferIndex = 0;

	for (size_t i = 0; i < info.m_bindings.size(); ++i) {
		descriptorWrites[i] = {};
		descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[i].dstSet = m_set;
		descriptorWrites[i].dstBinding = i;
		descriptorWrites[i].dstArrayElement = 0;
		descriptorWrites[i].descriptorType = info.m_bindings[i].getDescriptorType();
		descriptorWrites[i].descriptorCount = info.m_bindings[i].getDescriptors().size();

		if (std::holds_alternative<Buffer>(info.m_bindings[i].getDescriptors()[0])) {
			auto pInfos = &bufferInfos[bufferIndex];
			for (size_t j = 0; j < info.m_bindings[i].getDescriptors().size(); ++j) {
				auto buffer = std::get<Buffer>(info.m_bindings[i].getDescriptors()[j]);

				VkDescriptorBufferInfo bufferInfo = {};
				bufferInfo.buffer = buffer.getBuffer();
				bufferInfo.range = buffer.getSize();
				bufferInfo.offset = buffer.getBufferOffset();

				bufferInfos[bufferIndex] = bufferInfo;
				bufferIndex++;
			}
			descriptorWrites[i].pBufferInfo = pInfos;
		} else {
			auto pInfos = &imageInfos[imageIndex];
			for (size_t j = 0; j < info.m_bindings[i].getDescriptors().size(); ++j) {
				auto image = std::get<Texture>(info.m_bindings[i].getDescriptors()[j]);

				VkDescriptorImageInfo imageInfo = {};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = image.getImage().getView();
				imageInfo.sampler = image.getSampler();

				imageInfos[imageIndex] = imageInfo;
				imageIndex++;
			}
			descriptorWrites[i].pImageInfo = pInfos;
		}
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
