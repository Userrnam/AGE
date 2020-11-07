#include "Descriptor.hpp"
#include "DescriptorManager.hpp"
#include "../Core/Core.hpp"

namespace age {

DescriptorSet& DescriptorSet::get(const DescriptorSetInfo& info) {
	DescriptorInfo descriptorInfo;

	for (auto& binding : info.m_bindings) {
		descriptorInfo.addType(binding.getDescriptorType());
	}

	m_dd = DescriptorManager::requestDescriptorSet(descriptorInfo);
	
	std::vector<VkWriteDescriptorSet> descriptorWrites = {};
	descriptorWrites.resize(info.m_bindings.size());

	// Fixme
	std::vector<VkDescriptorBufferInfo> bufferInfos;
	std::vector<VkDescriptorImageInfo> imageInfos;
	bufferInfos.resize(128);
	imageInfos.resize(128);
	size_t imageIndex = 0;
	size_t bufferIndex = 0;

	for (size_t i = 0; i < info.m_bindings.size(); ++i) {
		descriptorWrites[i] = {};
		descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[i].dstSet = m_dd.set;
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

void DescriptorSet::destroy() {
	DescriptorManager::freeDescriptorSet(m_dd);
}

} // namespace age
