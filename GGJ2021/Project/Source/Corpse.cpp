#include "Corpse.h"
#include <SFML/Graphics.hpp>

Corpse::Corpse()
{
}

/// <summary>
/// sets up the corpse sprite
/// </summary>
/// <param name="texture">the texture of the corpse</param>
/// <param name="position">the position of the corpse</param>
/// <param name="colour">the colour which the corpse will be</param>
void Corpse::setupCorpse(const sf::Texture& texture, const sf::Vector2f& position, const sf::Color& colour)
{
	m_sprite.setTexture(texture);
	m_sprite.setTextureRect(corpseSize);
	m_sprite.setColor(colour);
	m_sprite.setOrigin(corpseSize.width / 2.0f, 0);
	m_sprite.setPosition(position);
};

/// <summary>
/// sets the size of the corpse
/// </summary>
/// <param name="factorX">the scale of the corpse</param>
void Corpse::setScale(const float& factorX, const float& factorY)
{
	m_sprite.setScale(factorX, factorY);
};
