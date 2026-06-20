#pragma once
#include "config.hpp"

struct SoundManager {
    sf::SoundBuffer sword = sf::SoundBuffer("Risorse/sound_effects/snd_board_sword1.wav");
    sf::SoundBuffer hurt = sf::SoundBuffer("Risorse/sound_effects/snd_board_playerhurt.wav");
    sf::SoundBuffer chest_open = sf::SoundBuffer("Risorse/sound_effects/snd_locker.wav");
    sf::SoundBuffer kill = sf::SoundBuffer("Risorse/sound_effects/snd_board_kill.wav");
    sf::SoundBuffer rumble = sf::SoundBuffer("Risorse/sound_effects/snd_rumble.wav");
    sf::SoundBuffer enemy_spawn = sf::SoundBuffer("Risorse/sound_effects/snd_board_splash.wav");
    sf::SoundBuffer spit_fire = sf::SoundBuffer("Risorse/sound_effects/snd_board_torch_low.wav");
    sf::SoundBuffer jump = sf::SoundBuffer("Risorse/sound_effects/snd_board_throw.wav");
    sf::SoundBuffer scroll = sf::SoundBuffer("Risorse/sound_effects/snd_board_text_main.wav");
    sf::SoundBuffer select = sf::SoundBuffer("Risorse/sound_effects/snd_board_text_main_end.wav");

    sf::Sound sword_sound;
    sf::Sound hurt_sound;
    sf::Sound chest_open_sound;
    sf::Sound kill_sound;
    sf::Sound rumble_sound;
    sf::Sound enemy_spawn_sound;
    sf::Sound spit_fire_sound;
    sf::Sound jump_sound;
    sf::Sound scroll_sound;
    sf::Sound select_sound;

    bool play_sword = false;
    bool play_hurt = false;
    bool play_chest_open = false;
    bool play_kill = false;
    bool play_rumble = false;
    bool play_enemy_spawn = false;
    bool play_spit_fire = false;
    bool play_jump = false;
    bool play_scroll = false;
    bool play_select = false;

    SoundManager();
    void playSword();
    void playHurt();
    void playChestOpen();
    void playKill();
    void playRumble();
    void playEnemySpawn();
    void playSpitFire();
    void playJump();
    void playScroll();
    void playSelect();
};