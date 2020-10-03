#pragma once

#include "Application.hpp"
#include "External/entt.hpp"

namespace age {

class SceneAPI {
protected:
    Application* parent = nullptr;
	entt::registry m_registry;

	friend class Entity;
public:
	inline entt::registry* getRegistry() { return &m_registry; }

	inline FontComponent* getFont(const std::string& font) {
		return parent->getFont(font);
	}

	inline Texture& getTexture(const std::string& texture) {
		return parent->getTexture(texture);
	}

	inline void loadTexture(const std::string& path, const std::string& textureName, const Sampler& sampler = Sampler::getDefault()) {
		parent->loadTexture(path, textureName, sampler);
	}

	inline void freeTexture(const std::string& textureName) {
		parent->freeTexture(textureName);
	}
};

} // namespace age
