#pragma once

#include "Application.hpp"
#include "../External/entt.hpp"
#include "../Utils/utils.hpp"
#include "../UI/UIManager.hpp"
#include "../Animation/Animator.hpp"
#include "../Graphics/PositionManager.hpp"

namespace age {

class SceneBase {
protected:
	std::vector<uint64_t> m_viewIds = { hash("default") };
    Application* parent = nullptr;
	entt::registry m_registry;
	UIManager m_uiManager;
	Animator m_animator;
	PositionManager m_positionManager;

	friend class Entity;
public:
	virtual ~SceneBase() { m_uiManager.destroy(); }

	PositionManager* getPositionManager() { return &m_positionManager; }

	void handleEvent(const Event& e) { m_uiManager.update(e); }

	// Animation

	template<typename Anim>
	inline uint64_t addAnimation(const Anim& animation) { m_animator.addAnimation(animation); }
	inline void pauseAnimation(uint64_t id) { m_animator.pauseAnimation(id); }
	inline void continueAnimation(uint64_t id) { m_animator.continueAnimation(id); }
	inline void stopAnimation(uint64_t id) { m_animator.stopAnimation(id); }

	// UI

	inline uint64_t addUIBlock(const UIBlock& block) { return m_uiManager.addBlock(block); }
	inline UIBlock& getUIBlock(uint64_t id) { return m_uiManager.getBlock(id); }
	inline void eraseUIBlock(uint64_t id) { m_uiManager.eraseBlock(id); }

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
