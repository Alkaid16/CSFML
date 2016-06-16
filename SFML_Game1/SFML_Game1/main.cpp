#include "SFML\Graphics.hpp"
#include <vector>

bool movePlayer(sf::RectangleShape& player, const sf::RectangleShape& floor, const sf::Vector2<float>& vel, sf::Time& dt);

int main() {
	sf::Vector2<float> vel(0,0);
	float speed = 100;
	float gravity = 100;
	std::vector<sf::Shape*> shapes(10);

	sf::RenderWindow window(sf::VideoMode(800,600), "test");
	window.setFramerateLimit(30);
	sf::RectangleShape square(sf::Vector2<float>(30, 30));
	sf::RectangleShape floor(sf::Vector2<float>(200, 50));
	shapes.push_back(&square);
	shapes.push_back(&floor);
	floor.setPosition(0, 300);

	square.setFillColor(sf::Color::Green);
	sf::Clock deltaClock;

	while (window.isOpen()) {
		sf::Time dt = deltaClock.restart();
		sf::Event event;
		vel.x = 0;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space && vel.y == 0) {
					vel.y = -100;
				}
			}
		}	

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			vel.x = speed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			vel.x = -speed;
		}

		bool collision = movePlayer(square, floor, vel, dt);
		if (!collision)
			vel.y += gravity*dt.asSeconds();
		else
			vel.y = 0;

		

		window.clear();
		for (std::vector<sf::Shape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
			if(*it != nullptr) window.draw(**it);
		}
		window.display();
	}

	return 0;
}

bool movePlayer(sf::RectangleShape& player, const sf::RectangleShape& floor, const sf::Vector2<float>& vel, sf::Time& dt) {
	player.move(vel*dt.asSeconds());
	sf::FloatRect pBounds = player.getGlobalBounds();
	sf::FloatRect fBounds = floor.getGlobalBounds();
	
	if (pBounds.intersects(fBounds)) {
		float xCorr=0, yCorr=0, aux;

		//Top intersection
		aux = fBounds.top + fBounds.height - pBounds.top;
		if (vel.y<0 && pBounds.top>fBounds.top && aux > 0) yCorr = aux;
		//Bot intersection
		aux = pBounds.top + pBounds.height - fBounds.top;
		if (vel.y>0 && pBounds.top<fBounds.top && aux > 0) yCorr = -aux;

		//Left intersection
		aux = fBounds.left + fBounds.width - pBounds.left;
		if (vel.x<0 && pBounds.left>fBounds.left && aux > 0) xCorr = aux;
		//Right intersection
		aux = pBounds.left + pBounds.width - fBounds.left;
		if (vel.y>0 && pBounds.left<fBounds.left && aux > 0) xCorr = -aux;
		player.move(xCorr, yCorr);
		return true;
	}
	else {
		return false;
	}
}