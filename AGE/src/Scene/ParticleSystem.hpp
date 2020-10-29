#pragma once

#include <vector>
#include <assert.h>

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

struct ParticleBase : public UnmanagedTransformable {
    Color color;
    float livedTime = 0;
    float lifeTime = 0;

    // void update(float elapsedTime) {}

    // must set life time
    // void spawn() {}
};

struct ParticleInfo {
    // spawn position change
    static constexpr float maxscx = 3.0f;
    static constexpr float maxscy = 3.0f;
    // velocitly
    static constexpr float minvx = 10.0f;
    static constexpr float maxvx = 20.0f;
    static constexpr float minvy = 5.0f;
    static constexpr float maxvy = 6.0f;
    // life time
    static constexpr float minlt = 0.5f;
    static constexpr float maxlt = 1.5f;
};

template<typename ParticleInfoType>
struct Particle : public ParticleBase {
    ParticleInfoType info;
    Vector2f velocity;

    void spawn() {
        // set velocity
        velocity.x = frandom(info.minvx, info.maxvx);
        velocity.y = frandom(info.minvy, info.maxvy);

        // set position
        UnmanagedTransformable::setPosition({
            frandom(0, info.maxscx),
            frandom(0, info.maxscy)
        });

        // set life time
        lifeTime = frandom(info.minlt, info.maxlt);
    }

    void update(float elapsedTime) {
        UnmanagedTransformable::move(velocity * elapsedTime);
    }
};

template<typename ParticleType>
class ParticleSystem : public ScriptComponent {
    Transformable m_transformable;
    StorageComponent<Transform> m_transformBuffer;

    // particles per second
    float m_spawnRate;
    int m_maxParticles;

    std::vector<ParticleType> m_particles;
    ArrayComponent<Bundle<Transform, Color>, PER_INSTANCE> m_particlesBuffer;
public:
    ParticleSystem(Entity e, float spawnRate, ShapeId shapeId = RECTANGLE_SHAPE) : ScriptComponent(e) {
        m_spawnRate = spawnRate;

        m_maxParticles = ParticleType::getMaximalLifeTime() * spawnRate + 1;
        m_particlesBuffer.create(m_maxParticles);
        m_particles.reserve(m_maxParticles);

        // assuming all shapes have same size (1, 1)
        // fixme: it may not be true
        m_transformable.create(e);
        m_transformBuffer.create(m_transformable.getTransform());

        addComponent<Drawable>(shapeId,
            m_transformBuffer,
            m_particlesBuffer
        );
    }

    ~ParticleSystem() {
        m_transformable.destroy();

        getComponent<Drawable>().destroy();
        m_particlesBuffer.destroy();
    }

    virtual void onUpdate(float elapsedTime) override;
};

template<typename ParticleType>
void ParticleSystem<ParticleType>::onUpdate(float elapsedTime) {
    // remove dead particles
    m_particlesBuffer.clear();
    for (int i = 0; i < m_particles.size(); ) {
        // we know that fields in particle are the same as in our buffer
        // ParticleType& particle = *((ParticleType*)&m_particlesBuffer[i]);
        m_particles[i].livedTime += elapsedTime;
        if (m_particles[i].livedTime >= m_particles[i].lifeTime) {
            m_particles.erase(i);
        } else {
            m_particles[i].update(elapsedTime);
            m_particlesBuffer.add({ m_particles[i].getTransform(), m_particles[i].color });
            ++i;
        }
    }

    // spawn new particles
    int spawnCount = elapsedTime * m_spawnRate;
    for (int i = 0; i < spawnCount; ++i) {
        m_particles.push_back(ParticleType());

        m_particles.back().spawn();

        // force cast particle to bundle
        m_particlesBuffer.add({ m_particles.back().getTransform(), m_particles.push_back().color });
    }

    assert(m_particles.size() <= m_maxParticles);

    m_particlesBuffer.upload();
    getComponent<Drawable>().setInstanceCount(m_particlesBuffer.count());
}

} // namespace age
