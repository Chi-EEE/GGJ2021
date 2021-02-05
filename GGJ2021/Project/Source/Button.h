#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	const sf::Sprite& getSprite() const { return m_sprite; };
	void setupButton(const sf::Texture& texture, const sf::Vector2f& position, const short& buttonNumber);
	void setHold(const sf::Vector2i& t_mousePosition);
	bool releaseClick(const sf::Vector2i& t_mousePosition);
private:
	sf::Sprite m_sprite;
	sf::IntRect m_buttonSize{ 0, 0, 242 ,85};
	short m_buttonNumber{ 0 };
	bool m_hold{ false };
};