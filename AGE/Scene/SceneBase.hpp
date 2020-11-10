#pragma once

#include "Application.hpp"
#include "../External/entt.hpp"
#include "../Utils/utils.hpp"
#include "../UI/UIManager.hpp"
#include "../Animation/Animator.hpp"
#include "../Graphics/PositionManager.hpp"
#include "../Graphics/View/View.hpp"

namespace age {

class SceneBase {
	EVENT_CALLBACK(SceneBase, handleResize);
	EVENT_CALLBACK(SceneBase, update);

protected:
	View m_dynamicView;
	View m_staticView;
	float m_runTime;

    Application* parent = nullptr;
	entt::registry m_registry;
	UIManager m_uiManager;
	Animator m_animator;
	PositionManager m_positionManager;

	friend class Entity;
public:
	SceneBase() { m_dynamicView.create(-10000); m_staticView.create(10000); selectStaticView(); }
	virtual ~SceneBase() { m_uiManager.destroy(); m_dynamicView.destroy(); m_staticView.destroy(); }

	void selectDynamicView();
	void selectStaticView();

	View& getDynamicView() { return m_dynamicView; }
	View& getStaticView() { return m_staticView; }

	PositionManager* getPositionManager() { return &m_positionManager; }

	void handleResize(const event::Resize& e) {
		m_dynamicView.handleWindowResize(e.oldSize, e.newSize);
		m_staticView.handleWindowResize(e.oldSize, e.newSize);
	}

	void update(const event::Update& e) {
		m_runTime = e.runTime;
		m_staticView.update(e.elapsedTime, e.runTime);
		m_dynamicView.update(e.elapsedTime, e.runTime);
	}

	float getRunTime() const { return m_runTime; }

	// Animation

	template<typename Anim>
	inline uint64_t addAnimation(const Anim& animation) { return m_animator.addAnimation(animation); }
	inline void pauseAnimation(uint64_t id) { m_animator.pauseAnimation(id); }
	inline void continueAnimation(uint64_t id) { m_animator.continueAnimation(id); }
	inline void stopAnimation(uint64_t id) { m_animator.stopAnimation(id); }

	// UI

	inline uint64_t addUIBlock(const UIBlock& block) { return m_uiManager.addBlock(block); }
	inline UIBlock& getUIBlock(uint64_t id) { return m_uiManager.getBlock(id); }
	inline void eraseUIBlock(uint64_t id) { m_uiManager.eraseBlock(id); }

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
