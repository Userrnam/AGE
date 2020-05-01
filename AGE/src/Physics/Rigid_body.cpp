#include "Rigid_body.hpp"

namespace age {
    Rigid_body::Rigid_body(Vector2D_t position_v_, Vector2D_t direction_v_, double speed_, double radius_)
        : position_v(position_v_), direction_v(direction_v_), speed(speed_), radius(radius_) {
            ++num_of_obj;
            obj.setRadius(radius);
            obj.setPosition(position_v.x - radius, position_v.y - radius);

            dir_v = sf::VertexArray(sf::Lines, 2);
            dir_v[0].position.x = position_v.x;
            dir_v[0].position.y = position_v.y;
            dir_v[1].position.x = position_v.x + direction_v.x * radius;
            dir_v[1].position.y = position_v.y + direction_v.y * radius;
            dir_v[1].color = sf::Color::Red;
    }

    Rigid_body::~Rigid_body() {
        --num_of_obj;
    }

    void Rigid_body::move() {
        direction_v = this->normalize(direction_v);

        position_v.x += direction_v.x * speed;
        position_v.y += direction_v.y * speed;

        dir_v[0].position.x = position_v.x;
        dir_v[0].position.y = position_v.y;
        dir_v[1].position.x = position_v.x + direction_v.x * radius;
        dir_v[1].position.y = position_v.y + direction_v.y * radius;
        obj.setPosition(position_v.x - radius, position_v.y - radius);
    }

    Vector2D_t Rigid_body::normalize(Vector2D_t direction_v_) {
        Vector2D_t temp_direction_v;
        double x = direction_v_.x;
        double y = direction_v_.y;
        double len = sqrt(x * x + y * y);
        temp_direction_v.x = x / len;
        temp_direction_v.y = y / len;
        return temp_direction_v;
    }

    bool Rigid_body::isColision(Rigid_body r_body) {
        double r1 = this->radius;
        double r2 = r_body.get_rad();
        double x1 = this->position_v.x;
        double y1 = this->position_v.y;
        double x2 = r_body.get_pos().x;
        double y2 = r_body.get_pos().y;
        double dist = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        if ((r1 + r2) > dist) {
            return false;
        }
        return false;
    }

    double Rigid_body::get_rad() {
        return radius;
    }

    Vector2D_t Rigid_body::get_pos() {
        return position_v;
    }

    sf::CircleShape Rigid_body::get_sphere() {
        return obj;
    }

    sf::VertexArray Rigid_body::get_dir_v() {
        return dir_v;
    }
}
