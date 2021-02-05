#pragma once

#include "AssetManager.h"
#include "Enemy.h"
#include "Mouse.h"
#include "Corpse.h"
#include "Button.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class App {
    static inline const sf::Time FrameTime = sf::seconds(1.0f / 60.f);
    static constexpr auto MaxEnemies = 20000;
    static constexpr auto MaxWrongTargets = 3;
public:
    App();
    void run();

private:
    enum class State {
        Menu, Game, Gameover, NextLevel
    };

    void setup();

    void handleEvents();
    void update();        
    void render();

    void updateMenu();
    void updateGame();
    void updateGameover();
    void updateNextLevel();

    void renderMenu();
    void renderGame();
    void renderGameover();
    void renderNextLevel();

    void resetGameStage();
    void changeCrosshairColour();
    void addScore();

    int getEnemyCount() { return ((5 * (m_level * m_level)) - (10 * m_level) + (10)); }; // the equation to get the number of enemies (not including targets)
    void getZoom() { m_zoom = 1.0f/(1 + static_cast <int> (m_level/3.5f)); }; // the equation to get the zoom distance
    Mouse m_mouse;
    State m_state;
    sf::RenderWindow m_window;

    sf::Text m_text;

    // Game Scene Vars
    std::vector<Enemy> m_enemies;
    std::vector<Corpse> m_corpses;

    std::vector<sf::Color> m_targetColours; // the colours that the crosshair will go through
    int m_colourCounter{ 0 };

    sf::Sprite m_crossHair;
    sf::Sprite m_background;

    sf::Sound m_sniperSound;

    short m_shotWrongTarget{ 0 };
    int m_score{ 0 }; //how many targets you have killed in the round

    int m_level{ 1 };
    const int m_startingLevel{ 40 }; // starting level

    bool m_shot{ false };

    sf::Text m_livesText;
    sf::Text m_scoreText;

    const std::string defaultScoreString{ "Score: " };
    const std::string defaultLivesString{ "Lives: " };
    const std::string defaultLevelString{ "Level: " };

    int m_enemyCount{ 5 };
    float m_zoom{ 1.0f };

    Button m_playButton;
    const sf::Vector2f m_playButtonPosition{ 262.0f,300.0f };
    const short m_playButtonNumber{ 1 };

    Button m_restartButton;
    const sf::Vector2f m_restartButtonPosition{ 484.0f,400.0f };
    const short m_restartButtonNumber{ 2 };

    Button m_tryAgainButton;
    const sf::Vector2f m_tryAgainButtonPosition{ 121.0f,400.0f };
    const short m_tryAgainButtonNumber{ 0 };

    Button m_nextLevelButton;
    const sf::Vector2f m_nextLevelButtonPosition{ 262.0f,300.0f };
    const short m_nextLevelButtonNumber{ 3 };

    FontManager    m_fntMgr;
    SfxManager     m_sfxMgr;
    TextureManager m_texMgr;
};