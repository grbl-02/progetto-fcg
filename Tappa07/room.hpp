#pragma once
#include "config.hpp"
#include "environment.hpp"
#include "enemies.hpp"

struct Room
{
    std::string name;
    sf::Texture roomTexture;
    sf::Texture assetsTexture;
    sf::Texture blueSlimeTexture;
    sf::Texture redSlimeTexture;
    std::vector<Tile> tiles;
    std::vector<Asset> assets;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<sf::FloatRect> door_hitboxes;
    sf::IntRect left_exit;
    sf::IntRect right_exit;
    sf::IntRect up_exit;
    sf::IntRect down_exit;

    std::string room_left;
    std::string room_right;
    std::string room_up;
    std::string room_down;

    Room(std::string& filename);
    sf::IntRect stringToIntRect(std::string tileID);
    void unload();
    void load(std::string& new_room);
    void enemyCollisions();
    void enemyWallCollisions();
    void enemyDeathCleanUp();
    void draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash);
};