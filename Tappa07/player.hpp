#pragma once
#include "config.hpp"

struct Player
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;
    float speed;
    int animation_frame = 0;
    bool isLeft;
    sf::Clock animation_clock;
    sf::FloatRect hitbox;

    Player();
    void draw(sf::RenderWindow& window, bool hitboxes);
    void animation(int row, float frameTime);
    void move_up(float elapsed);
    void move_down(float elapsed);
    void move_left(float elapsed);
    void move_right(float elapsed);
    void enter_left_pos();
    void enter_right_pos();
    void enter_up_pos();
    void enter_down_pos();
};