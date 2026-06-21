#pragma once
#include "config.hpp"
#include "projectiles.hpp"

struct Player;

struct Enemy {

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

    int id;
    int health_points;

    bool hurt;
    bool is_dead;
    bool death_finished;
    sf::Clock flash_clock;

    bool is_jumping;
    float jump_timer;
    float cooldown_timer;
    sf::Vector2f jump_start;
    sf::Vector2f jump_target;

    Dir enemy_dir;

    Enemy(sf::Vector2f pos, const sf::Texture& texture, std::string name, Dir enemy_dir, int id);
    virtual ~Enemy() = default;
    virtual void enemyLogic(const Player& player, float elapsed) {};
    void animation(int row, float frame_time);
    virtual void onCollisionResponse() {};
    virtual void draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash);
};

struct BlueSlime : Enemy {
    bool is_starting_to_jump = false;

    BlueSlime(sf::Vector2f pos, const sf::Texture& texture, Dir enemy_dir, int id);
    void jumpTowardsPlayer(sf::Vector2f player_pos, float elapsed);
    void enemyLogic(const Player& player, float elapsed) override;
};

struct RedSlime : Enemy {
    sf::Texture fireballTexture;
    std::vector<Fireball> fireballs;
    bool has_shot;
    bool is_starting_to_spit = false;

    RedSlime(sf::Vector2f pos, const sf::Texture& texture, Dir enemy_dir, int id);
    void enemyLogic(const Player& player, float elapsed) override;
    void facesPlayer(const Player& player);
    void spitFire(const Player& player);
    void deleteFire();
    void draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash) override;
};