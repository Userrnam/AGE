#pragma  once

#include <AGE/Scene.hpp>

#include <vector>
#include <utility>

#include "Bird.hpp"
#include "Column.hpp"
#include "Score.hpp"

#include "../Background.hpp"


struct FlappyBirdScene : public age::Scene {
    EVENT_CALLBACK(FlappyBirdScene, update);

    std::vector<std::pair<Column*, Column*>> columns;
    Score* scoreScript;
    Bird* birdScript;
    float speed = -50.0f;
    float miny, maxy;

    FlappyBirdScene(age::Application* app) : age::Scene(app) {
        scoreScript = createEntity<Score>();
        birdScript = createEntity<Bird>();
        createColumns();
        createEntity<Background>(age::Color(0.0f, 0.0f, 0.0f, 1.0f));
    }

    void update(const age::event::Update& e) {
        if (birdScript->dead) {
            return;
        }
        for (auto& p : columns) {
            p.first->move(speed * e.elapsedTime);
            p.second->move(speed * e.elapsedTime);

            if (!p.first->isVisible()) {
                float x = columns.size() * 300 + p.first->transformable.getPosition().x;
                float y = age::frandom(miny, maxy);

                p.first->setBottom(age::Vector2f(x, y));
                p.second->setTop(age::Vector2f(x, y - 200));

                scoreScript->incrementScore();
            }
        }
    }

    void createColumns() {
        auto windowSize = getApplication()->getWindowSize();

        miny = 200;
        maxy = windowSize.y - 100;
        
        int count = windowSize.x / 300 + 2;
    
        for (int i = 1; i < count; ++i) {
            auto top = createEntity<Column>();
            auto bottom = createEntity<Column>();

            float y = age::frandom(miny, maxy);
            top->setBottom(age::Vector2f(300 * i, y));
            bottom->setTop(age::Vector2f(300 * i, y - 200));

            columns.push_back(std::make_pair(top, bottom));
        }
    }
};
