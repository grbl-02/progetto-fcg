#pragma once
#include "config.hpp"
#include "player.hpp"
#include "room.hpp"

struct State
{
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

    State(sf::Shader& flash);
    void draw(sf::RenderWindow& window, sf::Shader& flash);
    void room_transition();
    void collisions(bool isX);
    void hit();
    void update(float elapsed);
};