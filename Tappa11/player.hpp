#pragma once
#include "config.hpp"

struct Player {
    sf::Texture texture = sf::Texture(player_texture);
    sf::Sprite sprite;
    sf::Vector2f pos;
    sf::FloatRect hitbox;
    float speed = player_speed;
    int healthPoints = 10;

    int animation_frame = 0;
    sf::Clock animation_clock;
    bool isLeft = false;
    Dir direction = UP;

    bool isAttacking = false;
    int attack_animation_frame = 0;
    int aaf_no_mod = 0;
    sf::FloatRect slashHitbox = sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
    bool slashHit = false;

    bool isHurt = false;
    sf::Clock redFlashClock;
    bool isInvincible = false;
    sf::Clock invincibilityClock;
    sf::Clock invincibilityFlashClock;
    bool spriteVisible = false;
    bool isFlashing = false;

    bool dead = false;
    int death_animation_frame = 0;
    bool deathAnimationEnded = false;
    bool isDying = false;

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