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
    GameMode gameMode = MENU;
    Difficulty difficulty = NORMAL;
    Flags flags;
    Menu menu;
    Textbox textbox;
    DialogueManager dialogueManager;
    SoundManager soundManager;
    Player player;
    Room room;
    std::string roomname = room0;

    HUD hud;
    sf::Texture heartsTexture = sf::Texture(hearts_path);

    bool move_player_up = false;
    bool move_player_down = false;
    bool move_player_left = false;
    bool move_player_right = false;
    bool playerAttacks = false;
    bool playerMoving = false;
    bool playerInteracting = false;
    Dir lastPressed = UP;

    sf::Shader* flash;
    sf::Shader* redflash;

    sf::Text gameOverText;
    sf::Text restartText;
    sf::Text victoryText;

    bool interactionIsHappening = false;
    bool continueDialogue = false;
    bool checkingChest3 = false;
    bool checkingChest5 = false;
    bool checkingChest6 = false;

    sf::Vector2f spritePositions[4] = {{32.f, 96.f}, {320.f, 96.f}, {32.f, 224.f}, {320.f, 224.f}}; 
    sf::Texture spawningSmoke = sf::Texture(spawn_path);
    std::vector<sf::Sprite> spawningSmokeSprites;
    bool start_animations = false;
    sf::Clock animation_clock;
    bool clockActive = false;
    int animation_frames[4] = {0, 0, 0, 0};

    bool play_the_victory_music = false;
    bool music_played = false;

    bool hitboxes = false;

    State(sf::Shader& flash, sf::Shader& redflash, sf::Font& font);

    void roomTransition();
    void processMovement(sf::Vector2f velocity, float elapsed);
    void hit();
    void update(float elapsed, sf::View& camera);
    void reset(Difficulty difficulty);

    void triggerGauntlet3();
    void triggerGauntlet5();
    void triggerGauntlet6();
    void clearGauntlet3();
    void clearGauntlet5();
    void clearGauntlet6();

    void chestCheck();
    void interaction(std::string dialogue);
    void spawnAnimation();

    void draw(sf::RenderWindow& window, sf::Shader& flash, sf::Shader& redflash);
};