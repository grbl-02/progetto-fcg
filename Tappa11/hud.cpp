#include "hud.hpp"

HUD::HUD(sf::Texture& texture, int healthPoints) {
    update_hud = false;
    animationClock.reset();
    clock_active = false;
    this->healthPoints = healthPoints;
    for (int i = 0; i < healthPoints; i++)
    {
        sf::Sprite sprite = sf::Sprite(texture, sf::IntRect({1, 0}, {10, 10}));
        sprite.setPosition({10.f + i * 11.f, 10.f});
        hearts.push_back(sprite);
    }
}

void HUD::update() {
    if (update_hud) {
        if (!clock_active) {
            animationClock.restart();
            clock_active = true;
        }
        if (animationClock.getElapsedTime().asSeconds() >= spawnFrameTime) {
            bool animationEnds = false;
            hearts[healthPoints - 1].setTextureRect(sf::IntRect({1 + animation_frames * 11, 0}, {10, 10}));
            animation_frames++;
            if (animation_frames >= 3) {
                animationEnds = true;
                animation_frames = 0;
            }

            if (animationEnds) {
                update_hud = false;
                clock_active = false;
                animationClock.reset();
                healthPoints --;
            } else
                animationClock.restart();
        }
    }
}

void HUD::reset(sf::Texture& texture, int healthPoints) {
    update_hud = false;
    animationClock.reset();
    clock_active = false;
    this->healthPoints = healthPoints;
    hearts.clear();
    for (int i = 0; i < healthPoints; i++)
    {
        sf::Sprite sprite = sf::Sprite(texture, sf::IntRect({1, 0}, {10, 10}));
        sprite.setPosition({10.f + i * 11.f, 10.f});
        hearts.push_back(sprite);
    }
}

void HUD::draw(sf::RenderWindow& window)
{
    for (auto& heart : hearts)
        window.draw(heart);
}