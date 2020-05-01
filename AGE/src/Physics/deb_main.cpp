#include "Rigid_body.hpp"
#include <unistd.h>

int main() {
    sf::RenderWindow win(sf::VideoMode(640, 480), "Debug winodw");
    age::Vector2D_t pv = {640 / 2, 480 / 2};
    age::Vector2D_t dv = {100, -10};    // вектор направления (задается только направление, при вызове move() нормализуется)
    age::Rigid_body object = age::Rigid_body(pv, dv, 0.1, 50);

    while (win.isOpen()) {

		sf::Event event;
		while (win.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				win.close();
		}

        object.move();

        win.clear();

        win.draw(object.get_sphere());
        win.draw(object.get_dir_v());

        win.display();
        usleep(1666);
    }

    return 0;
}