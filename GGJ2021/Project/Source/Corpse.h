#pragma once
#include "Corpse.h"
#include <SFML/Graphics.hpp>

class Corpse
{
public:
    Corpse();
    const sf::Sprite& getSprite() const { return m_sprite; }

    void setupCorpse(const sf::Texture& texture, const sf::Vector2f& position, const sf::Color& colour);
    void setScale(const float& factorX, const float& factorY);

private:
    sf::Sprite m_sprite{ };
    const sf::IntRect corpseSize{ 602, 6, 150, 150 };
};

