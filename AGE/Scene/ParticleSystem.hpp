#pragma once

#include <vector>
#include <assert.h>
#include <stdio.h>

#include "ScriptComponent.hpp"
#include "../Graphics/ObjectCreation/Components/TransformComponent.hpp"
#include "../Graphics/ObjectCreation/Components/ColorComponent.hpp"
#include "../Graphics/ObjectCreation/Components/ArrayComponent.hpp"
#include "../Graphics/ObjectCreation/Components/BundleComponent.hpp"
#include "../Graphics/ObjectCreation/Drawable.hpp"
#include "../Graphics/ObjectCreation/ShapeManager.hpp"
#include "../Graphics/Transformable.hpp"
#include "../Graphics/ObjectCreation/Components/StorageComponent.hpp"
#include "../Math/Vector.hpp"
#include "../Utils/utils.hpp"


namespace age {

/*

ParticleSystem usage:

    1. create class that inherits from age::ParticleBase
    2. define these methods:
        - void spawn()
        - static float maxLifeTime()
        - void update(float elapsedTime)
    
    ParticleBase inherits from UnmanagedTransformable, so all functions for tranrmations are available
    ParticleBase has following members:
        - color
        - livedTime
        - lifeTime

    class MyParticle : public ParticleBase {
        void spawn() {
            // spawn particle
        }

        float maxLifeTime() {
            return maxLifeTime;
        }

        void update(float elapsedTime) {
            // update particle's transform and color
        }
    };

    In your class will have access to all methods of unmanagedTransformable, but you should
    avoid using set methods, because if you for example use setPosition in your particle
    class you will not be able to change position of ParticleSystem

*/

struct ParticleBase : public UnmanagedTransformable {
    Color color;
    float livedTime = 0;
    float lifeTime = 0;

    // void update(float elapsedTime) {}

    // must set life time
    // void spawn() {}
    // static float maxLifeTime();
    // static Vector2f maxSize();
};

template<typename ParticleType>
class ParticleSystem : public ScriptComponent {
    Transformable m_transformable;

    // particles per second
    float m_spawnRate;
    int m_maxParticles;

    float m_elapsedTotal = 0;
    bool m_stopped = false;

    std::vector<ParticleType> m_particles;
    ArrayComponent<Bundle<Transform, Color>, PER_INSTANCE> m_particlesBuffer;
public:
    ParticleSystem(Entity e, float spawnRate, ShapeId shapeId = RECTANGLE_SHAPE) : ScriptComponent(e) {
        m_spawnRate = spawnRate;

        m_maxParticles = ParticleType::maxLifeTime() * spawnRate + 1;
        m_particlesBuffer.create(m_maxParticles);
        m_particles.reserve(m_maxParticles);

        m_transformable.create(e, ParticleType::maxSize());
        m_transformable.setOrigin(0.5f, 0.0f);

        addComponent<Drawable>(shapeId,
            m_particlesBuffer
        );
    }

    ~ParticleSystem() {
        m_transformable.destroy();

        getComponent<Drawable>().destroy();
        m_particlesBuffer.destroy();
    }

    virtual void onUpdate(float elapsedTime) override;

    Transformable& getTransformable() { return m_transformable; }
    void stop() {
        m_stopped = true;
    }

    void resume() {
        m_stopped = false;
    }
};

template<typename ParticleType>
void ParticleSystem<ParticleType>::onUpdate(float elapsedTime) {
    // remove dead particles
    m_particlesBuffer.clear();
    for (int i = 0; i < m_particles.size(); ) {
        m_particles[i].livedTime += elapsedTime;
        if (m_particles[i].livedTime >= m_particles[i].lifeTime) {
            m_particles.erase(m_particles.begin() + i);
        } else {
            m_particles[i].update(elapsedTime);

            Bundle<Transform, Color> b;
            b.set<Transform>(m_particles[i].getTransform());
            b.set<Color>(m_particles[i].color);

            m_particlesBuffer.add(b);
            ++i;
        }
    }

    if (m_stopped) {
        m_particlesBuffer.upload();
        getComponent<Drawable>().setInstanceCount(m_particlesBuffer.count());
        return;
    }

    // spawn new particles
    m_elapsedTotal += elapsedTime;
    int spawnCount = m_elapsedTotal * m_spawnRate;
    if (spawnCount > 0) {
        int dif = m_particles.size() + spawnCount - m_maxParticles;
        if (dif > 0) {
            spawnCount -= dif;
        }
        for (int i = 0; i < spawnCount; ++i) {
            m_particles.push_back(ParticleType());

            m_particles.back().setPosition(m_transformable.getPosition());
            m_particles.back().setScale(m_transformable.getScale());
            m_particles.back().setOrigin(m_transformable.getOrigin());
            m_particles.back().setRotation(m_transformable.getRotation());

            m_particles.back().spawn();

            Bundle<Transform, Color> b;
            b.set<Transform>(m_particles.back().getTransform());
            b.set<Color>(m_particles.back().color);

            m_particlesBuffer.add(b);

        }
        m_elapsedTotal = 0;
    }

    assert(m_particles.size() <= m_maxParticles);

    m_particlesBuffer.upload();
    getComponent<Drawable>().setInstanceCount(m_particlesBuffer.count());
}

} // namespace age
