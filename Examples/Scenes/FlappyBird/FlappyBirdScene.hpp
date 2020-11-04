#pragma  once

#include <AGE/Scene.hpp>

#include <vector>
#include <utility>

#include "Bird.hpp"
#include "Column.hpp"

#include "../Background.hpp"


struct FlappyBirdScene : public age::Scene {
    std::vector<std::pair<Column*, Column*>> columns;
    float speed = -50.0f;
    float miny, maxy;

    FlappyBirdScene(age::Application* app) : age::Scene(app) {
        createEntity<Bird>();
        createColumns();
        createStaticEntity<Background>(age::Color(0.0f, 0.0f, 0.0f, 1.0f));
    }

    virtual void onUpdate(float elapsedTime) {
        for (auto& p : columns) {
            p.first->move(speed * elapsedTime);
            p.second->move(speed * elapsedTime);

            if (!p.first->isVisible()) {
                float x = columns.size() * 300 + p.first->transformable.getPosition().x;
                float y = age::frandom(miny, maxy);

                p.first->setBottom(age::Vector2f(x, y));
                p.second->setTop(age::Vector2f(x, y - 200));
            }
        }
    }

    void createColumns() {
        auto windowSize = getApplication()->getWindowSize();

        miny = 200;
        maxy = windowSize.y - 100;
        
        int count = windowSize.x / 300 + 2;
    
        for (int i = 1; i < count; ++i) {
            auto _top = createStaticEntity<Column>();
            auto _bottom = createStaticEntity<Column>();

            auto top = std::get<1>(_top);
            auto bottom = std::get<1>(_bottom);

            float y = age::frandom(miny, maxy);
            top->setBottom(age::Vector2f(300 * i, y));
            bottom->setTop(age::Vector2f(300 * i, y - 200));

            columns.push_back(std::make_pair(top, bottom));
        }
    }
};
