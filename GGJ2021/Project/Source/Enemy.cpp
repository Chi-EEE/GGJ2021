#include "Enemy.h"

Enemy::Enemy()
{
	setRandomPosition();
	setVelocity();
}

/// <summary>
/// sets up the sprite for the enemy
/// </summary>
/// <param name="texture">the texture which the enemy uses</param>
/// <param name="isTarget">sets whether if the enemy is the target</param>
void Enemy::setupSprite(const sf::Texture& texture, const bool& isTarget)
{
	m_sprite.setTexture(texture);
	m_sprite.setTextureRect(m_spriteTextureRect);
	m_sprite.setOrigin(m_spriteTextureRect.width/ 2.0f, m_spriteTextureRect.height/ 2.0f);
	m_sprite.setScale(m_spriteSize, m_spriteSize);
	m_isTarget = isTarget;
	m_sprite.setColor(sf::Color(Random::getValue(60, 255), Random::getValue(75, 255), Random::getValue(60, 255)));
};

/// <summary>
/// changes the colour of the enemy and sets their spawn location and velocity and makes them alive again
/// </summary>
void Enemy::reset(float& zoom)
{
	m_spriteSize = zoom;
	m_speed = m_defaultSpeed * m_spriteSize;
	m_sprite.setScale(m_spriteSize, m_spriteSize);
	m_sprite.setColor(sf::Color(Random::getValue(60, 255), Random::getValue(75, 255), Random::getValue(60, 255)));
	setRandomPosition();
	setVelocity();
	m_dead = false;
};

/// <summary>
/// if there's extra sprites then draw here
/// </summary>
void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite);
}

/// <summary>
/// sets the random spawn location for the enemy
/// </summary>
void Enemy::setRandomPosition()
{
	Random::getValue(0, 1) == 0 ? m_direction = Direction::Left : m_direction = Direction::Right;

	sf::Vector2f randomPositionRight = sf::Vector2f{ 800.0f, static_cast <float> (Random::getValue(0,600)) };
	sf::Vector2f randomPositionLeft = sf::Vector2f{ 0.0f, static_cast <float> (Random::getValue(0,600)) };

	if (m_direction == Direction::Left) {
		m_sprite.setScale(-m_spriteSize, m_spriteSize);
		m_position = randomPositionRight;
		m_desiredPosition = randomPositionLeft;
	} else {
		m_sprite.setScale(m_spriteSize, m_spriteSize);
		m_position = randomPositionLeft;
		m_desiredPosition = randomPositionRight;
	}

	m_sprite.setPosition(m_position);
};

/// <summary>
/// turns the enemy beackwards and makes them walk that direction
/// </summary>
void Enemy::changeDesiredPosition()
{
	switch (m_direction) {
	case Direction::Left:
		m_sprite.setScale(m_spriteSize, m_spriteSize);
		m_desiredPosition = sf::Vector2f{ 800.0f, static_cast <float> (Random::getValue(0,600)) };
		m_direction = Direction::Right;
		break;
	case Direction::Right:
		m_sprite.setScale(-m_spriteSize, m_spriteSize);
		m_desiredPosition = sf::Vector2f{ 0.0f, static_cast <float> (Random::getValue(0,600)) };
		m_direction = Direction::Left;
		break;
	}
};

/// <summary>
/// sets the moving velocity of the enemy
/// </summary>
void Enemy::setVelocity()
{
	sf::Vector2f headingVector{ 0.0f,0.0f };
	float vectorLength{ 0.0f };

	headingVector = m_desiredPosition - m_position;
	vectorLength = std::sqrtf(headingVector.x * headingVector.x + headingVector.y * headingVector.y);
	headingVector = headingVector / vectorLength;

	m_velocity = headingVector * m_speed;
}

/// <summary>
/// moves the enemy cooresponding to their direction
/// there's a small chance for the enemy to stop moving or a small chance to turn direction
/// </summary>
void Enemy::move()
{
	int randomNumber = Random::getValue(0, 250);
	if (randomNumber < 3) // Chance to turn around
	{
		changeDesiredPosition();
		setVelocity();
	}
	else if (randomNumber < 4) // Chance to stop
	{
		m_stop = !m_stop;
		m_sprite.setTextureRect(m_spriteTextureRect);
	}
	else if (!m_stop) //if the enemy is moving
	{
		switch (m_direction)// depending on their direction
		{
		case Direction::Left:
			m_position += m_velocity;
			if (m_desiredPosition.x > m_position.x)
			{
				changeDesiredPosition();
				setVelocity();
			}
			else
			{
				m_sprite.setPosition(m_position);
			}
			break;
		case Direction::Right:
			m_position += m_velocity;
			if (m_desiredPosition.x < m_position.x)
			{
				changeDesiredPosition();
				setVelocity();
			}
			else
			{
				m_sprite.setPosition(m_position);
			}
			break;
		}
		animate();
	}
}

/// <summary>
/// checks if the mouse has clicked on the sprite and 
/// whether if the mouse clicks not on the sprite / on the sprite but not a target / on the sprite and is a target
/// </summary>
/// <param name="t_mousePosition">position of the mouse</param>
/// <returns>3 different values that mean something different</returns>
short Enemy::detectClick(const sf::Vector2i&t_mousePosition)
{
	sf::Vector2f mousePositionFloat = sf::Vector2f(t_mousePosition);
	if (m_sprite.getGlobalBounds().contains(mousePositionFloat)) 
	{
		if (isTarget()) {
			return 2;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

/// <summary>
/// animates the sprite
/// </summary>
void Enemy::animate()
{		
	m_frameCounter += m_frameIncrement;
	m_frame = static_cast<int>(m_frameCounter) % 4;
	m_sprite.setTextureRect(sf::IntRect{ m_frame * 90 + 90, 0, 90, 150 });
}

/// <summary>
/// makes the enemy disappear and
/// spawns a corpse at it's place
/// </summary>
/// <param name="texture">the corpse's texture</param>
/// <returns>the corpse</returns>
Corpse Enemy::die(const sf::Texture& texture)
{
	m_dead = true;
	Corpse newCorpse;
	newCorpse.setupCorpse(texture, m_position,m_sprite.getColor());
	switch (m_direction) {
	case Direction::Left:
		newCorpse.setScale(-m_spriteSize, m_spriteSize);
		break;
	case Direction::Right:
		newCorpse.setScale(m_spriteSize, m_spriteSize);
		break;
	}
	return newCorpse;
}