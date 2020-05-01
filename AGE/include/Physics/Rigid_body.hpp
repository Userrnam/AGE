#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>

namespace age {
typedef struct {
    double x = 0.;
    double y = 0.;
} Vector2D_t;

class Rigid_body {
private:
    double radius;
    double mass;
    double speed;
    size_t num_of_obj;
    Vector2D_t direction_v;
    Vector2D_t position_v;
    // sfml obj
    sf::CircleShape obj;
    sf::VertexArray dir_v;
public:
    Rigid_body(Vector2D_t position_v_, Vector2D_t direction_v_, double speed_, double radius_);
    ~Rigid_body();
    void move();
    Vector2D_t normalize(Vector2D_t direction_v);
    Vector2D_t sum(Vector2D_t l, Vector2D_t r);
    bool isColision(Rigid_body r_body);
    double get_rad();
    Vector2D_t get_pos();
    sf::CircleShape get_sphere();
    sf::VertexArray get_dir_v();
};
}
