#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My Window!");

	// get the size of the window
	sf::Vector2u size = window.getSize();
	unsigned int width = size.x;
	unsigned int height = size.y;

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

	int centerX = (desktop.width - width) / 2;
	int centerY = (desktop.height - height) / 2;

	window.setPosition(sf::Vector2i(centerX, centerY));

	// full size image sprite
	int frameWidth = 960 / 3;
	int frameHeight = 320;

	sf::Texture texture;
	if (!texture.loadFromFile("assets/save-knight-with-walk.png"))
	{
		std::cout << "Could not load texture\n";
		return 1;
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect({
										  0,
										  0,
									  },
									  {frameWidth, frameHeight}));

	float sprite_speed = 0.25f;
	sf::Clock clock;

	sprite.setScale(sf::Vector2f(0.35f, 0.35f));

	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	sprite.setOrigin({spriteBounds.width / 2.f, spriteBounds.height / 2.f});

	sprite.setPosition(sf::Vector2f(400.f, 300.f));

	float speed = 300.f;

	float sprite_animation_timer = 0.f;

	while (window.isOpen())
	{
		float delta_time = clock.restart().asSeconds();

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
		bool is_move = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
		{
			sprite.setScale({-0.35f, 0.35f});
			is_move = true;
			movement.x -= speed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
		{
			sprite.setScale({0.35f, 0.35f});
			is_move = true;
			movement.x += speed;
		}

		sprite_animation_timer += delta_time;

		if (is_move)
		{
			if (sprite_animation_timer >= sprite_speed)
			{
				sprite_animation_timer = 0.f;

				int current_left = sprite.getTextureRect().left;
				int next_left = current_left + frameWidth;

				if (next_left >= (frameWidth * 3))
					next_left = 0;

				sprite.setTextureRect(sf::IntRect(
					{
						next_left,
						0,
					},
					{frameWidth, frameHeight}));
			}
		}
		else
			sprite.setTextureRect(sf::IntRect(
				{
					0,
					0,
				},
				{frameWidth, frameHeight}));

		sprite.move(movement * delta_time);
		window.clear(sf::Color::White);

		window.draw(sprite);

		window.display();
	}

	return 0;
}
