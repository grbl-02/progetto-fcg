#include "projectiles.hpp"

Fireball::Fireball(const sf::Texture& texture, sf::IntRect textureRect, sf::Vector2f pos, sf::Vector2f direction) : sprite(texture)
{
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(textureRect);
    this->pos = pos;
    this->direction = direction;
    animation_clock.start();
    isDestroyed = false;
    destructionFinished = false;
}

void Fireball::draw(sf::RenderWindow& window)
{
    sprite.setPosition(pos);
    window.draw(sprite);
}

void Fireball::goTowardsPlayer(float elapsed)
{
    pos = pos + (direction * fireball_speed * elapsed);
    animation(movFrameTime);
}

void Fireball::animation(float frameTime)
{
    if (isDestroyed)
    {
        if (animation_clock.getElapsedTime().asSeconds() >= frameTime)
        {
            animation_clock.restart();
            sf::IntRect curFrame = sf::IntRect({2 +  8 * destruction_animation_frame, 11}, {8, 8});
            sprite.setTextureRect(curFrame);
            if (destruction_animation_frame == 4)
            {
                destructionFinished = true;
            }
            destruction_animation_frame = (destruction_animation_frame + 1) % 5;
        }
    }
    if (animation_clock.getElapsedTime().asSeconds() >= frameTime)
    {
        animation_clock.restart();
        sf::IntRect curFrame;
        if (animation_frame == 5)
        {
            curFrame = sf::IntRect({11, 2}, {8, 8});
        }
        else
        {
            curFrame = sf::IntRect({2, 2}, {8, 8});
        }
        sprite.setTextureRect(curFrame);
        animation_frame = (animation_frame + 1) % 6;
    }
}