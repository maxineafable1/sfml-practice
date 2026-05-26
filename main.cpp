#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My Window!");
	window.setFramerateLimit(60);

	// get the size of the window
	sf::Vector2u size = window.getSize();
	unsigned int width = size.x;
	unsigned int height = size.y;

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

	int centerX = (desktop.width - width) / 2;
	int centerY = (desktop.height - height) / 2;

	window.setPosition(sf::Vector2i(centerX, centerY));

	sf::Texture texture;
	if (!texture.loadFromFile("assets/knight.png"))
	{
		std::cout << "Could not load texture\n";
		return 1;
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);

	sprite.setScale(sf::Vector2f(0.35f, 0.35f));

	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	sprite.setOrigin(spriteBounds.width / 2.f, spriteBounds.height / 2.f);

	sprite.setPosition(sf::Vector2f(400.f, 300.f));

	float speed = 5.f;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
			{
				if (event.key.scancode == sf::Keyboard::Scan::Space)
				{
					std::cout << "hello test code: " << event.key.scancode << '\n';
					std::cout << "description: " << sf::Keyboard::getDescription(event.key.scancode).toAnsiString() << '\n';
				}
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << "right click" << '\n';
				}
				else if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "left click!\n";
				}
				break;
			}

			default:
				break;
			}
		}

		sf::Vector2f movement(0.f, 0.f);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			movement.y -= speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			movement.y += speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			movement.x -= speed;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			movement.x += speed;

		float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
		if (length != 0.f)
			movement = (movement / length) * speed;

		sprite.move(movement);
		window.clear(sf::Color::White);

		window.draw(sprite);

		window.display();
	}

	return 0;
}
