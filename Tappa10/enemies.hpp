#pragma once
#include "config.hpp"
#include "projectiles.hpp"

struct Player;

struct Enemy
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;
    std::string name;
    int animation_frame = 0;
    int jump_animation_frame = 0;
    int death_animation_frame = 0;
    sf::Clock animation_clock;
    FloatCircle aggro_range;
    sf::FloatRect hitbox;
    sf::FloatRect hurtbox;
    sf::Vector2f hurtbox_offset;
    int healthPoints;
    bool hurt;
    int id;
    bool isDead;
    bool deathFinished;

    bool isJumping;
    float jumptimer;
    float cooldownTimer;
    sf::Vector2f jumpStart;
    sf::Vector2f jumpTarget;
    dir enemyDir;
    sf::Clock flashClock;

    Enemy(sf::Vector2f pos, const sf::Texture& texture, std::string name, dir enemyDir, int id);
    virtual ~Enemy() = default;

    virtual void enemy_logic(const Player& player, float elapsed) {};
    void animation(int row, float frameTime);
    virtual void onCollisionResponse() {};
    virtual void draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash);
};

struct BlueSlime : Enemy
{
    BlueSlime(sf::Vector2f pos, const sf::Texture& texture, dir enemyDir, int id);
    void jump_towards_player(sf::Vector2f playerPos, float elapsed);
    void enemy_logic(const Player& player, float elapsed) override;
};

struct RedSlime : Enemy
{
    sf::Texture fireballTexture;
    std::vector<Fireball> fireballs;
    bool hasShot;

    RedSlime(sf::Vector2f pos, const sf::Texture& texture, dir enemyDir, int id);

    void facesPlayer(const Player& player);
    void enemy_logic(const Player& player, float elapsed) override;
    void spitFire(const Player& player);
    void deleteFire();
    void draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash) override;
};