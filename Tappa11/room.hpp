#pragma once
#include "config.hpp"
#include "environment.hpp"
#include "enemies.hpp"
#include "flags.hpp"
#include "conditionEvaluator.hpp"

struct Room {
    std::string name;

    sf::Texture roomTexture = sf::Texture(all_tiles);
    sf::Texture assetsTexture = sf::Texture(all_assets);
    sf::Texture blueSlimeTexture = sf::Texture(blue_slime_path);
    sf::Texture redSlimeTexture = sf::Texture(red_slime_path);
    sf::Texture spikesTexture = sf::Texture(spikes);
    sf::Texture grassTexture = sf::Texture(grass_tiles);

    std::vector<Tile> tiles;
    std::vector<Asset> assets;
    std::vector<std::unique_ptr<Enemy>> enemies;

    std::vector<sf::FloatRect> door_hitboxes;

    sf::IntRect left_exit;
    sf::IntRect right_exit;
    sf::IntRect up_exit;
    sf::IntRect down_exit;

    bool start_animations = false;
    bool clock_active = false;
    sf::Clock animation_clock;
    int animation_frames[2] = {0, 0};
    bool spikesDisappearing = false;
    bool animationEnded = false;
    bool chestFalls = false;

    std::string room_left;
    std::string room_right;
    std::string room_up;
    std::string room_down;

    Flags* flags;
    ConditionEvaluator conditionEvaluator;

    Room(std::string& filename, Flags& flags);
    sf::IntRect stringToIntRect(std::string tileID);
    void unload();
    void load(std::string& new_room);
    void enemyCollisions();
    void enemyWallCollisions();
    void enemyDeathCleanUp();
    void spikesAnimation();
    void spikesDisappearingAnimation();
    int chestFalling(float elapsed);
    void draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash);
};