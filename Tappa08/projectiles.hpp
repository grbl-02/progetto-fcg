#pragma once
#include "config.hpp"

struct Fireball
{
    sf::Sprite sprite;
    sf::Vector2f pos;
    sf::Vector2f direction;
    int animation_frame = 0;
    sf::Clock animation_clock;
    bool isDestroyed;
    bool destructionFinished;
    int destruction_animation_frame = 0;
    sf::FloatRect hitbox;

    Fireball(const sf::Texture& texture, sf::IntRect textureRect, sf::Vector2f pos, sf::Vector2f direction);
    void goTowardsPlayer(float elapsed);
    void animation(float frameTime);
    void draw(sf::RenderWindow& window, bool hitboxes);
};