#pragma once

#include <array>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>

class Mouse {
public:
    enum class State {
        None, Press, Release, Hold
    };

    void update();
    void setKey(sf::Mouse::Button btn, bool keyDown);
    void setPosition(sf::Vector2i position);
    bool checkKeyAndState(sf::Mouse::Button btn, const State state) const;

    sf::Vector2i getPosition() const { return m_position; }

private:
    sf::Vector2i m_position;
    std::array<bool, sf::Mouse::ButtonCount> m_currState;
    std::array<bool, sf::Mouse::ButtonCount> m_prevState;
};


