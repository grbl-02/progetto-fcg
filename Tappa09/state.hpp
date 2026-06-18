#pragma once
#include "config.hpp"
#include "player.hpp"
#include "room.hpp"
#include "flags.hpp"
#include "textbox.hpp"
#include "dialogueManager.hpp"

struct State
{
    GameMode gameMode;
    Flags flags;
    Textbox textbox;
    DialogueManager dialogueManager;
    std::string roomname;
    Player player;
    Room room;

    bool move_player_up;
    bool move_player_down;
    bool move_player_left;
    bool move_player_right;
    bool playerAttacks;
    bool playerMoving;
    bool playerInteracting;
    bool hitboxes;
    dir lastPressed;
    sf::Shader* flash;
    sf::Shader* redflash;
    sf::Text gameOverText;
    sf::Text restartText;
    bool interactionIsHappening;
    bool continueDialogue;

    State(sf::Shader& flash, sf::Shader& redflash, sf::Font& font);
    void draw(sf::RenderWindow& window, sf::Shader& flash, sf::Shader& redflash);
    void room_transition();
    void collisions(bool isX);
    void hit();
    void update(float elapsed);
    void reset();

    void trigger_gauntlet3();
    void trigger_gauntlet5();
    void trigger_gauntlet6();
    void clear_gauntlet3();
    void clear_gauntlet5();
    void clear_gauntlet6();

    void chestCheck();
    void interaction(std::string dialogue);
};