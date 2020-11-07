#pragma once

#include <AGE/Graphics.hpp>
#include <AGE/Scene.hpp>


struct Score : public age::ScriptComponent {
    age::Transformable transformable;
    age::TextComponent text;
    age::StorageComponent<age::Transform> buffer;
    int score = 0;

    std::string getScoreString() {
        return "Score: " + std::to_string(score);
    }
    
    Score(age::Entity e) : age::ScriptComponent(e) {
        text.create(getFont("courier"), 20);
        text.setText(getScoreString());

        transformable.create(e, text.getSize());

        buffer.create();

        addComponent<age::Drawable>(age::RECTANGLE_SHAPE,
            buffer,
            text
        );

        getComponent<age::Drawable>().setInstanceCount(getScoreString().size());
    }

    void incrementScore() {
        score++;
        auto s = getScoreString();
        text.setText(s);
        getComponent<age::Drawable>().setInstanceCount(s.size());
    }

    ~Score() {
        text.destroy();
        buffer.destroy();

        getComponent<age::Drawable>().destroy();
    }
};
