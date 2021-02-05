#include "Mouse.h"

void Mouse::update()
{
	m_prevState = m_currState;
}

void Mouse::setKey(sf::Mouse::Button btn, bool btnDown)
{
	m_currState[btn] = btnDown;
}

void Mouse::setPosition(sf::Vector2i position)
{
	m_position = position;
}

bool Mouse::checkKeyAndState(sf::Mouse::Button btn, const State state) const
{
    State currentState = State::None;
    if (m_prevState[btn]) {
        currentState = !m_currState[btn] ? State::Release : State::Hold;
    }
    else if (m_currState[btn]) {
        currentState = State::Press;
    }
    return currentState == state;
}
