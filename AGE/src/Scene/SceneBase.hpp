#pragma once

#include "Application.hpp"
#include "../External/entt.hpp"
#include "../Utils/utils.hpp"


namespace age {

class SceneBase {
protected:
	std::vector<uint64_t> m_viewIds = { hash("default") };
    Application* parent = nullptr;
	entt::registry m_registry;

	friend class Entity;
public:
	inline void addViewId(uint64_t id) { m_viewIds.push_back(id); }
	inline const std::vector<uint64_t>& getViewIds() const { return m_viewIds; }

	inline entt::registry* getRegistry() { return &m_registry; }
	inline FontComponent* getFont(const std::string& font) { return parent->getFont(font); }
	inline Texture& getTexture(const std::string& texture) { return parent->getTexture(texture); }

	Application* getApplication() { return parent; }

	inline void loadTexture(const std::string& path, const std::string& textureName, const Sampler& sampler = Sampler::getDefault()) {
		parent->loadTexture(path, textureName, sampler);
	}

	inline void freeTexture(const std::string& textureName) {
		parent->freeTexture(textureName);
	}
};

} // namespace age
