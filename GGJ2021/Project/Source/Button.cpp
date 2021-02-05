#include "Button.h"
#include <iostream>
/// <summary>
/// sets up button
/// </summary>
/// <param name="texture">texture required for button</param>
/// <param name="position">position where the button wilol be at</param>
/// <param name="buttonSize">the texture rect of the button</param>
void Button::setupButton(const sf::Texture& texture, const sf::Vector2f& position, const short& buttonNumber)
{
	m_buttonNumber = buttonNumber;
	m_sprite.setTexture(texture);
	m_sprite.setTextureRect(sf::IntRect{ 0, buttonNumber * m_buttonSize.height,m_buttonSize.width,m_buttonSize.height });
	m_sprite.setPosition(position);
}

void Button::setHold(const sf::Vector2i& t_mousePosition)
{
	sf::Vector2f mousePositionFloat = sf::Vector2f(t_mousePosition);
	if (m_sprite.getGlobalBounds().contains(mousePositionFloat))
	{
		m_hold = true;
		m_sprite.setTextureRect(sf::IntRect{ m_buttonSize.width, m_buttonNumber * m_buttonSize.height,m_buttonSize.width,m_buttonSize.height });
	}
}

/// <summary>
/// checks if the mouse has clicked on the sprite
/// </summary>
/// <param name="t_mousePosition">position of the mouse</param>
/// <returns>bool if clicked on</returns>
bool Button::releaseClick(const sf::Vector2i& t_mousePosition)
{
	m_sprite.setTextureRect(sf::IntRect{ 0, m_buttonNumber * m_buttonSize.height,m_buttonSize.width,m_buttonSize.height });
	sf::Vector2f mousePositionFloat = sf::Vector2f(t_mousePosition);
	if (m_hold && m_sprite.getGlobalBounds().contains(mousePositionFloat))
	{
		m_hold = false;
		return true;
	}
	m_hold = false;
	return false;
}