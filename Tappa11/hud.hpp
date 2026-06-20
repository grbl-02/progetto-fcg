#pragma once
#include "config.hpp"

struct HUD {
    std::vector<sf::Sprite> hearts;
    bool update_hud;
    sf::Clock animationClock;
    bool clock_active;
    int animation_frames = 0;
    int healthPoints;

    HUD(sf::Texture& texture, int healthPoints);
    void update();
    void reset(sf::Texture& texture, int healthPoints);
    void draw(sf::RenderWindow& window);
};