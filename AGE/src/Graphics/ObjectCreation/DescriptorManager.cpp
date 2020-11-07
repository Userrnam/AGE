
#include "DescriptorManager.hpp"

#include "../Core/Core.hpp"


namespace age {

std::unordered_map<DescriptorInfo, VkDescriptorSetLayout> DescriptorManager::m_layouts;
std::unordered_map<DescriptorInfo, std::vector<DescriptorPool>> DescriptorManager::m_pools;

// FIXME:
#define SET_COUNT 16

VkDescriptorSetLayout DescriptorManager::createLayout(DescriptorInfo info) {
	std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(info.count());

	for (uint32_t bindingIndex = 0; bindingIndex < bindings.size(); ++bindingIndex) {
		bindings[bindingIndex].binding = bindings.size() - bindingIndex - 1;
		bindings[bindingIndex].descriptorCount = 1;
		bindings[bindingIndex].pImmutableSamplers = nullptr;

        info.fetch(bindingIndex, bindings[bindingIndex].descriptorType, bindings[bindingIndex].stageFlags);
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	auto& layout = m_layouts[info];

	if (vkCreateDescriptorSetLayout(core::apiCore.device, &layoutInfo, nullptr, &layout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout");
	}

	return layout;
}

VkDescriptorSet DescriptorManager::createSets(DescriptorInfo info, uint32_t setCount, VkDescriptorSetLayout layout) {
	// create descriptor pool
	std::vector<VkDescriptorPoolSize> poolSizes;
	poolSizes.resize(info.count());

	for (uint32_t bindingIndex = 0; bindingIndex < poolSizes.size(); ++bindingIndex) {
		poolSizes[bindingIndex].descriptorCount = setCount;
		VkShaderStageFlags dummy;
		info.fetch(bindingIndex, poolSizes[bindingIndex].type, dummy);
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = poolSizes.size();
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = setCount;

	auto& pools = m_pools[info];
	pools.push_back(DescriptorPool());

	auto& pool = pools.back();
	pool.freeSets.resize(setCount);

	if (vkCreateDescriptorPool(core::apiCore.device, &poolInfo, nullptr, &pool.pool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool");
	}

    std::vector<VkDescriptorSetLayout> layouts(setCount, layout);

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool.pool;
	allocInfo.descriptorSetCount = layouts.size();
	allocInfo.pSetLayouts = layouts.data();

	if (vkAllocateDescriptorSets(core::apiCore.device, &allocInfo, pool.freeSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets");
	}

	VkDescriptorSet result;

	pool.getSet(result);

	return result;
}

DD DescriptorManager::requestDescriptorSet(DescriptorInfo info) {
	DD result;

	result.info = info.types;

	auto lit = m_layouts.find(info);
	if (lit == m_layouts.end()) {
		result.layout = createLayout(info);
		result.set = createSets(info, SET_COUNT, result.layout);

		return result;
	}

	result.layout = lit->second;

	auto pit = m_pools.find(info);
	if (pit == m_pools.end()) {
		result.set = createSets(info, SET_COUNT, result.layout);
	} else {
		for (auto& pool : pit->second) {
			if (pool.getSet(result.set)) {
				return result;
			}
		}
	}

	result.set = createSets(info, SET_COUNT, result.layout);

	return result;
}

void DescriptorManager::freeDescriptorSet(const DD &dd) {
	auto& pools = m_pools[DescriptorInfo(dd.info)];

	for (auto& pool : pools) {
		if (pool.freeSet(dd.set)) {
			return;
		}
	}

	assert(0);
}

void DescriptorManager::destroy() {
	for (auto layout : m_layouts) {
		vkDestroyDescriptorSetLayout(core::apiCore.device, layout.second, nullptr);
	}

	for (auto& ppool : m_pools) {
		for (auto& pool : ppool.second) {
			if (!pool.isFree()) {
				std::cout << "DescriptorPool is not free!\n";
				pool.status();
			}
			vkDestroyDescriptorPool(core::apiCore.device, pool.pool, nullptr);
		}
	}
}

} // namespace age
