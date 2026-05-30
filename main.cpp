#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

const int TILE_SIZE = 16;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int MAP_WIDTH = WINDOW_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = std::ceil(static_cast<float>(WINDOW_HEIGHT) / TILE_SIZE);

enum TileType
{
	AIR = 0,
	DIRT = 1
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My Window!");

	// get the size of the window
	sf::Vector2u size = window.getSize();
	unsigned int width = size.x;
	unsigned int height = size.y;

	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

	int centerX = (desktop.width - width) / 2;
	int centerY = (desktop.height - height) / 2;

	window.setPosition(sf::Vector2i(centerX, centerY));

	// full size image sprite
	int frameWidth = 48 / 3;
	int frameHeight = 16;

	sf::Texture texture;
	if (!texture.loadFromFile("assets/knight.png"))
	{
		std::cout << "Could not load texture\n";
		return 1;
	}
	texture.setSmooth(false);

	sf::Texture dirt_texture;
	if (!dirt_texture.loadFromFile("assets/concrete.png"))
	{
		std::cout << "Could not load dirt texture\n";
		return 1;
	}
	dirt_texture.setSmooth(false);

	std::vector<std::vector<int>> map(MAP_WIDTH, std::vector<int>(MAP_HEIGHT, AIR));
	int groundLevel = MAP_HEIGHT / 2;

	for (int x = 0; x < MAP_WIDTH; ++x)
	{
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			if (y >= groundLevel)
				map[x][y] = DIRT;
			else
				map[x][y] = AIR;
		}
	}

	sf::VertexArray va(sf::Quads);

	for (int x = 0; x < MAP_WIDTH; ++x)
	{
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			if (map[x][y] == AIR)
				continue;

			sf::Vertex quad[4];

			quad[0].position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
			quad[1].position = sf::Vector2f((x + 1) * TILE_SIZE, y * TILE_SIZE);
			quad[2].position = sf::Vector2f((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
			quad[3].position = sf::Vector2f(x * TILE_SIZE, (y + 1) * TILE_SIZE);

			quad[0].texCoords = sf::Vector2f(0, 0);
			quad[1].texCoords = sf::Vector2f(TILE_SIZE, 0);
			quad[2].texCoords = sf::Vector2f(TILE_SIZE, TILE_SIZE);
			quad[3].texCoords = sf::Vector2f(0, TILE_SIZE);

			va.append(quad[0]);
			va.append(quad[1]);
			va.append(quad[2]);
			va.append(quad[3]);
		}
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

	float groundYPixel = groundLevel * TILE_SIZE;
	// float scaledSpriteHeight = frameHeight * sprite.getScale().y;
	// float characterFeetY = groundYPixel - scaledSpriteHeight;

	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	sprite.setOrigin({spriteBounds.width / 2.f, spriteBounds.height});

	// sprite.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f));
	sprite.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f, groundYPixel));

	sprite.setScale(sf::Vector2f(2.f, 2.f));

	float speed = 300.f;

	float sprite_animation_timer = 0.f;

	// for player jump settings
	sf::Vector2f velocity(0.f, 0.f);
	const float GRAVITY = 1000.f;
	const float JUMP_FORCE = -500.f;

	while (window.isOpen())
	{
		float delta_time = clock.restart().asSeconds();

		sf::Vector2f movement(0.f, 0.f);

		bool is_move = false;
		bool is_ground = true;

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
					if (is_ground && (velocity.y == 0))
					{
						velocity.y = JUMP_FORCE;
						is_ground = false;
					}
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

		velocity.y += GRAVITY * delta_time;
		movement.y = velocity.y;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A))
		{
			sprite.setScale({-2.f, 2.f});
			is_move = true;
			movement.x -= speed;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D))
		{
			sprite.setScale({2.f, 2.f});
			is_move = true;
			movement.x += speed;
		}

		// ANIMATION
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

		// Find out which grid tile the player's feet are currently occupying
		int player_grid_x = static_cast<int>(sprite.getPosition().x / TILE_SIZE);
		int player_grid_y = static_cast<int>(sprite.getPosition().y / TILE_SIZE);

		if (player_grid_x >= 0 && player_grid_x < MAP_WIDTH &&
			player_grid_y >= 0 && player_grid_y < MAP_HEIGHT)
		{
			if (map[player_grid_x][player_grid_y] == DIRT)
			{
				velocity.y = 0.f;
				is_ground = true;

				float player_after_jump_y = player_grid_y * TILE_SIZE;
				sprite.setPosition(sf::Vector2f(sprite.getPosition().x, player_after_jump_y));
			}
			else
				is_ground = false;
		}

		if (sprite.getPosition().x > WINDOW_WIDTH)
			sprite.setPosition(sf::Vector2f(static_cast<float>(WINDOW_WIDTH - 1.f), sprite.getPosition().y));
		else if (sprite.getPosition().x < 0)
			sprite.setPosition(sf::Vector2f(0.f, sprite.getPosition().y));

		window.clear(sf::Color(135, 206, 235)); // Sky Blue

		window.draw(va, &dirt_texture);
		window.draw(sprite);

		window.display();
	}
	return 0;
}