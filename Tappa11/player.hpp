#pragma once
#include "config.hpp"

struct Player {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;
    float speed;
    int animation_frame = 0;
    bool isLeft;
    sf::Clock animation_clock;
    sf::FloatRect hitbox;
    dir direction;
    bool isAttacking;
    int attack_animation_frame = 0;
    int aaf_no_mod = 0;
    sf::FloatRect slashHitbox;
    bool slashHit;
    int healthPoints;
    bool isInvincible;
    bool isHurt;
    sf::Clock redFlashClock;
    sf::Clock invincibilityClock;
    bool dead;
    int death_animation_frame = 0;
    bool deathAnimationEnded;
    sf::Clock invincibilityFlashClock;
    bool spriteVisible;
    bool isFlashing;
    bool isDying;

    Player();
    void draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& redflash);
    void animation(int row, float frameTime);
    void attack(float elapsed);
    void slash();
    void unslash();
    void enter_left_pos();
    void enter_right_pos();
    void enter_up_pos();
    void enter_down_pos();
    void invincibilityTime();
    void reset(Difficulty difficulty);
};