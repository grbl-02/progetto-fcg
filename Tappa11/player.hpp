#pragma once
#include "config.hpp"

struct Player {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;
    sf::FloatRect hitbox;
    float speed;
    int healthPoints;

    int animation_frame = 0;
    sf::Clock animation_clock;
    bool isLeft;
    Dir direction;

    bool isAttacking;
    int attack_animation_frame = 0;
    int aaf_no_mod = 0;
    sf::FloatRect slashHitbox;
    bool slashHit;

    bool isHurt;
    sf::Clock redFlashClock;
    bool isInvincible;
    sf::Clock invincibilityClock;
    sf::Clock invincibilityFlashClock;
    bool spriteVisible;
    bool isFlashing;

    bool dead;
    int death_animation_frame = 0;
    bool deathAnimationEnded;
    bool isDying;

    Player();
    void animation(int row, float frameTime);
    void attack(float elapsed);
    void slash();
    void unslash();
    void enterLeftPos();
    void enterRightPos();
    void enterUpPos();
    void enterDownPos();
    void invincibilityTime();
    void reset(Difficulty difficulty);
    void draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& redflash);
};