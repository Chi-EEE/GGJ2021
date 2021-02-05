#pragma once

#include <SFML/Graphics.hpp>
#include "Random.h"
#include "Corpse.h"
#include <iostream>

class Enemy : public sf::Drawable {
	enum class Direction { Left, Right };

public:
	Enemy();

	sf::Vector2f getPosition() const { return m_position; }
	const sf::Sprite& getSprite() const { return m_sprite; }

	bool isDead() const { return m_dead; };
	bool isTarget() const { return m_isTarget; };

	void setupSprite(const sf::Texture& texture, const bool& isTarget);
	void reset(float& zoom);

	void changeDesiredPosition();
	void setVelocity();
	void move();

	Corpse die(const sf::Texture& texture);

	short detectClick(const sf::Vector2i& t_mousePosition);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	float m_spriteSize{ 1.0f };
	sf::IntRect m_spriteTextureRect = sf::IntRect(0, 0, 90, 150);
	float m_frameIncrement{ 0.1f };

	void setRandomPosition();
	void animate();

	short m_frame{ 0 };

	float m_speed{ 3.0f };
	float m_defaultSpeed{ 3.0f };

	float m_frameCounter{ 0.0f };
	bool m_isTarget{ false };
	bool m_dead{ false };
	bool m_stop{ false };
	sf::Sprite m_sprite{ };

	Direction    m_direction{ Direction::Left };
	sf::Vector2f m_position{ 0, 0 };
	sf::Vector2f m_velocity{ 1, 1 };
	sf::Vector2f m_desiredPosition{ 0, 0 };

};