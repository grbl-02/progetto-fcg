#pragma once
#include "config.hpp"
#include "player.hpp"
#include "room.hpp"
#include "flags.hpp"
#include "textbox.hpp"
#include "dialogueManager.hpp"
#include "hud.hpp"
#include "menu.hpp"
#include "soundManager.hpp"

struct RenderCommand;

struct State {
    GameMode gameMode;
    Flags flags;
    Menu menu;
    Textbox textbox;
    DialogueManager dialogueManager;
    SoundManager soundManager;
    HUD hud;
    sf::Texture heartsTexture;
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
    sf::Text victoryText;
    bool interactionIsHappening;
    bool continueDialogue;
    bool checkingChest3;
    bool checkingChest5;
    bool checkingChest6;
    sf::Vector2f spritePositions[4] = {{32.f, 96.f}, {320.f, 96.f}, {32.f, 224.f}, {320.f, 224.f}}; 
    sf::Texture spawningSmoke;
    std::vector<sf::Sprite> spawningSmokeSprites;
    bool start_animations;
    sf::Clock animation_clock;
    bool clockActive;
    int animation_frames[4] = {0, 0, 0, 0};
    Difficulty difficulty;
    bool play_the_victory_music = false;
    bool music_played = false;

    State(sf::Shader& flash, sf::Shader& redflash, sf::Font& font);
    void draw(sf::RenderWindow& window, sf::Shader& flash, sf::Shader& redflash);
    void room_transition();
    void processMovement(sf::Vector2f velocity, float elapsed);
    void hit();
    void update(float elapsed, sf::View& camera);
    void reset(Difficulty difficulty);

    void trigger_gauntlet3();
    void trigger_gauntlet5();
    void trigger_gauntlet6();
    void clear_gauntlet3();
    void clear_gauntlet5();
    void clear_gauntlet6();

    void chestCheck();
    void interaction(std::string dialogue);
    void spawnAnimation();
};