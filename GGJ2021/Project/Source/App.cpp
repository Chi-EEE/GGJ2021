#include "App.h"
#include <iostream>

App::App()
	: m_mouse  { }
	, m_state  { State::Menu }
	, m_window { sf::VideoMode(800, 600), "Target Finder" }
	, m_enemies(MaxEnemies)
	, m_crossHair()
{
}

void App::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	setup();

	while (m_window.isOpen()) {
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > FrameTime) {
			timeSinceLastUpdate -= FrameTime;

			handleEvents();
			update();
			render();
		}
	}
}

void App::setup()
{
	// Load Assets
	//m_texMgr.loadAsset("Tex1", "Assets/Texture/test.png");
	m_texMgr.loadAsset("Enemy", "Assets/Texture/targets.png");
	m_texMgr.loadAsset("crosshair", "Assets/Texture/crosshair.png");
	m_texMgr.loadAsset("Buttons", "Assets/Texture/buttons.png");
	m_texMgr.loadAsset("Background", "Assets/Texture/bg.png");

	m_fntMgr.loadAsset("Fnt1", "Assets/Font/test.ttf");

	m_sfxMgr.loadAsset("SniperSound", "Assets/SFX/163460__lemudcrab__sniper-shot.wav");

	m_text.setFont(*m_fntMgr.getAsset("Fnt1"));
	m_text.setFillColor(sf::Color::Yellow);
	m_text.setCharacterSize(50);
	m_text.setPosition(220, 25);
	m_text.setOutlineThickness(1.0f);
	m_text.setString("Target Finder");


	m_livesText.setFont(*m_fntMgr.getAsset("Fnt1"));
	m_livesText.setCharacterSize(50);
	m_livesText.setPosition(500, 25);
	m_livesText.setOutlineThickness(1.0f);
	m_livesText.setString(defaultLivesString + std::to_string(MaxWrongTargets - m_shotWrongTarget) + "/" + std::to_string(MaxWrongTargets));

	m_scoreText.setFont(*m_fntMgr.getAsset("Fnt1"));
	m_scoreText.setCharacterSize(50);
	m_scoreText.setPosition(50, 75);
	m_scoreText.setOutlineThickness(1.0f);
	m_scoreText.setString(defaultScoreString + std::to_string(m_score));
	// Set up the enemeies
	for (auto& e : m_enemies) {
		e.setupSprite(*m_texMgr.getAsset("Enemy"),false);
	}

	for (short i = 0; i < m_level; i++)
	{
		Enemy e;
		e.setupSprite(*m_texMgr.getAsset("Enemy"), true);
		m_enemies.push_back(e);
		m_targetColours.push_back(e.getSprite().getColor());
	}

	m_crossHair.setTexture(*m_texMgr.getAsset("crosshair"));
	m_crossHair.setColor(m_targetColours[0]);
	m_crossHair.setOrigin({ 25, 25 });

	m_background.setTexture(*m_texMgr.getAsset("Background"));
	m_background.setPosition(0.0f, 0.0f);
	//m_background.setTextureRect(sf::IntRect{ 0, 0, 8000, 6000 });

	m_playButton.setupButton(*m_texMgr.getAsset("Buttons"), m_playButtonPosition, m_playButtonNumber);
	m_restartButton.setupButton(*m_texMgr.getAsset("Buttons"), m_restartButtonPosition, m_restartButtonNumber);
	m_tryAgainButton.setupButton(*m_texMgr.getAsset("Buttons"), m_tryAgainButtonPosition, m_tryAgainButtonNumber);
	m_nextLevelButton.setupButton(*m_texMgr.getAsset("Buttons"), m_nextLevelButtonPosition, m_nextLevelButtonNumber);

	m_sniperSound.setBuffer(*m_sfxMgr.getAsset("SniperSound"));
	m_sniperSound.setVolume(75.0f);
	m_sniperSound.setPitch(0.5);
}

void App::handleEvents()
{
	sf::Event e;
	m_mouse.update();
	while (m_window.pollEvent(e)) {
		if (sf::Event::Closed == e.type) {
			m_window.close();
		} else if (sf::Event::MouseButtonPressed == e.type) {
			m_mouse.setKey(e.mouseButton.button, true);
		} else if (sf::Event::MouseButtonReleased == e.type) {
			m_mouse.setKey(e.mouseButton.button, false);
		} else if (sf::Event::MouseMoved == e.type) {
			m_mouse.setPosition({ e.mouseMove.x, e.mouseMove.y });
		}
	}
}

void App::update()
{
	switch (m_state) {
		case App::State::Menu: {
			updateMenu();
		} break;
		case App::State::Game: {
			updateGame();
		} break;
		case App::State::Gameover: {
			updateGameover();
		} break;
		case App::State::NextLevel: {
			updateNextLevel();
		} break;
	}
}

void App::render()
{
	m_window.clear();
	m_window.draw(m_background);
	switch (m_state) {
		case App::State::Menu: {
			renderMenu();
		} break;
		case App::State::Game: {
			renderGame();
		} break;
		case App::State::NextLevel: {
			renderNextLevel();
		} break;
		case App::State::Gameover: {
			renderGameover();
		} break;
	}
	m_window.display();
}

void App::resetGameStage()
{
	m_window.setMouseCursorVisible(false);

	m_text.setPosition(50, 25);
	m_text.setFillColor(sf::Color::White);

	m_state = State::Game;

	m_shotWrongTarget = 0;

	m_enemies.clear();
	m_corpses.clear();
	m_targetColours.clear();

	for (int i = 0; i < m_level; i++)
	{
		Enemy e;
		e.setupSprite(*m_texMgr.getAsset("Enemy"), true);
		m_enemies.push_back(e);
	}
	for (int i = 0; i < m_enemyCount; i++)
	{
		Enemy e;
		e.setupSprite(*m_texMgr.getAsset("Enemy"), false);
		m_enemies.push_back(e);
	}

	getZoom();
	//m_background.setScale(m_zoom, m_zoom);
	for (auto& e : m_enemies) {
		e.reset(m_zoom);
		if (e.isTarget())
		{
			m_targetColours.push_back(e.getSprite().getColor()); //add the target's colour to the crosshair vector
		}
	}

	m_crossHair.setColor(m_targetColours[0]);
	m_text.setString(defaultLevelString + std::to_string(m_level));
	m_scoreText.setString(defaultScoreString + std::to_string(m_score));
	m_livesText.setString(defaultLivesString + std::to_string(MaxWrongTargets - m_shotWrongTarget) + "/" + std::to_string(MaxWrongTargets));
}

void App::changeCrosshairColour()
{
	m_colourCounter++;
	if (m_colourCounter + 1 > m_targetColours.size())
	{
		m_colourCounter = 0;
	}
	m_crossHair.setColor(m_targetColours[m_colourCounter]); // change the cross hair colour based on the current target
}

void App::addScore()
{
	m_score++;
	m_scoreText.setString(defaultScoreString + std::to_string(m_score));
	if (m_score >= m_level)
	{
		m_text.setString("Click to enter next level");
		m_text.setPosition(100, 20);
		m_text.setFillColor(sf::Color::Yellow);
		m_window.setMouseCursorVisible(true);
		m_state = State::NextLevel;
	}
	else
	{
		changeCrosshairColour();
	}
}

void App::updateMenu()
{
	if (m_mouse.checkKeyAndState(sf::Mouse::Button::Left, Mouse::State::Press)) {
		m_playButton.setHold(m_mouse.getPosition());
	}
	if (m_mouse.checkKeyAndState(sf::Mouse::Button::Left, Mouse::State::Release)) {
		if (m_playButton.releaseClick(m_mouse.getPosition()))
		{
			m_level = m_startingLevel;
			m_enemyCount = getEnemyCount();
			resetGameStage();
		}
	}
}

void App::updateGame()
{
	sf::Vector2i mousePosition = m_mouse.getPosition();
	m_crossHair.setPosition(static_cast<sf::Vector2f>(mousePosition));

	if (m_mouse.checkKeyAndState(sf::Mouse::Button::Right, Mouse::State::Press)) {
		changeCrosshairColour();
	}
	if (m_mouse.checkKeyAndState(sf::Mouse::Button::Left, Mouse::State::Press)) {

		m_shot = false;
		m_sniperSound.play();
		for (auto& e : m_enemies) {
			if (!e.isDead() && !m_shot) {
				short shotState = e.detectClick(m_mouse.getPosition());
				if (shotState) {
					m_shot = true;
					Corpse newCorpse = e.die(*m_texMgr.getAsset("Enemy"));
					m_corpses.push_back(newCorpse);
					switch (shotState)
					{
					case 1:
						if (e.getSprite().getColor() == m_crossHair.getColor())
						{
							addScore();
						}
						else
						{
							m_shotWrongTarget++;
							m_livesText.setString(defaultLivesString + std::to_string(MaxWrongTargets - m_shotWrongTarget) + "/" + std::to_string(MaxWrongTargets));
							if (m_shotWrongTarget >= MaxWrongTargets)
							{
								m_window.setMouseCursorVisible(true);
								m_state = State::Gameover;
								m_text.setString("Gameover");
								m_text.setPosition(280, 25);
								m_text.setFillColor(sf::Color::Yellow);
							}
						}
						break;
					case 2:
						addScore();
						break;
					default:
						break;
					}
				}
			}
		}
	};

	for (auto& e : m_enemies) {
		if (!e.isDead())
		{
			e.move(); // Pass FrameTime if needed
		}
	}

	std::sort(m_enemies.begin(), m_enemies.end(), [](const auto& e1, const auto& e2) -> bool {
		return e1.getPosition().y < e2.getPosition().y;
	});
}

void App::updateGameover()
{
	if (m_mouse.checkKeyAndState(sf::Mouse::Button::Left, Mouse::State::Press)) {
		sf::Vector2i mousePosition = m_mouse.getPosition();
		m_tryAgainButton.setHold(mousePosition);
		m_restartButton.setHold(mousePosition);
	}
	if (m_mouse.checkKeyAndState(sf::Mouse::Button::Left, Mouse::State::Release)) {
		sf::Vector2i mousePosition = m_mouse.getPosition();
		if (m_tryAgainButton.releaseClick(mousePosition))
		{
			m_score = 0;
			m_colourCounter = 0;

			resetGameStage();
		}
		else if (m_restartButton.releaseClick(mousePosition))
		{
			m_level = m_startingLevel;
			m_enemyCount = getEnemyCount();
			m_score = 0;
			m_colourCounter = 0;

			resetGameStage();
		}
	}
}

void App::updateNextLevel()
{
	if (m_mouse.checkKeyAndState(sf::Mouse::Button::Left, Mouse::State::Press)) {
		m_nextLevelButton.setHold(m_mouse.getPosition());
	}
	if (m_mouse.checkKeyAndState(sf::Mouse::Button::Left, Mouse::State::Release)) {
		{
			if (m_nextLevelButton.releaseClick(m_mouse.getPosition()))
			{
				m_score = 0;
				m_colourCounter = 0;
				m_level++;
				m_enemyCount = getEnemyCount();

				resetGameStage();
			}
		}
	}
}

void App::renderMenu()
{
	m_window.draw(m_text);
	m_window.draw(m_playButton.getSprite());
}

void App::renderGame()
{
	for (const auto& e : m_corpses) {
		m_window.draw(e.getSprite());
	}
	for (const auto& e : m_enemies) {
		if (!e.isDead())
			m_window.draw(e); // if they're alive then show the enemy
	}
	m_window.draw(m_text);
	m_window.draw(m_livesText);
	m_window.draw(m_scoreText);
	m_window.draw(m_crossHair);
}

void App::renderGameover()
{
	m_window.draw(m_restartButton.getSprite());
	m_window.draw(m_tryAgainButton.getSprite());
	m_window.draw(m_text);
}

void App::renderNextLevel()
{
	m_window.draw(m_nextLevelButton.getSprite());
	m_window.draw(m_text);
}