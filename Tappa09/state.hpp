#pragma once
#include "config.hpp"
#include "player.hpp"
#include "room.hpp"

struct State
{
    GameMode gameMode;
    std::string roomname;
    Player player;
    Room room;

    bool move_player_up;
    bool move_player_down;
    bool move_player_left;
    bool move_player_right;
    bool playerAttacks;
    bool playerMoving;
    bool hitboxes;
    dir lastPressed;
    sf::Shader* flash;
    sf::Shader* redflash;
    sf::Text gameOverText;
    sf::Text restartText;

    // Progression tracking
    bool chest_3_opened = false;
    bool chest_5_opened = false;
    bool chest_6_opened = false;

    bool room_3_gauntlet_triggered = false;
    bool room_3_gauntlet_cleared = false;
    bool room_5_gauntlet_triggered = false;
    bool room_5_gauntlet_cleared = false;
    bool room_6_gauntlet_triggered = false;
    bool room_6_gauntlet_cleared = false;

    State(sf::Shader& flash, sf::Shader& redflash, sf::Font& font);
    void draw(sf::RenderWindow& window, sf::Shader& flash, sf::Shader& redflash);
    void room_transition();
    void collisions(bool isX);
    void hit();
    void update(float elapsed);
    void reset();
};