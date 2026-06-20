#include "soundManager.hpp"

SoundManager::SoundManager() :
    sword_sound(sword), hurt_sound(hurt), chest_open_sound(chest_open),
    kill_sound(kill), rumble_sound(rumble), enemy_spawn_sound(enemy_spawn),
    spit_fire_sound(spit_fire), jump_sound(jump), scroll_sound(scroll), select_sound(select) {}

void SoundManager::playSword() {
    if (play_sword) {
        sword_sound.play();
        play_sword = false;
    }
}

void SoundManager::playHurt() {
    if (play_hurt) {
        hurt_sound.play();
        play_hurt = false;
    }
}

void SoundManager::playChestOpen() {
    if (play_chest_open) {
        chest_open_sound.play();
        play_chest_open = false;
    }
}

void SoundManager::playKill() {
    if (play_kill) {
        kill_sound.play();
        play_kill = false;
    }
}

void SoundManager::playRumble() {
    if (play_rumble) {
        rumble_sound.play();
        play_rumble = false;
    }
}

void SoundManager::playEnemySpawn() {
    if (play_enemy_spawn) {
        enemy_spawn_sound.play();
        play_enemy_spawn = false;
    }
}

void SoundManager::playSpitFire() {
    if (play_spit_fire) {
        spit_fire_sound.play();
        play_spit_fire = false;
    }
}

void SoundManager::playJump() {
    if (play_jump) {
        jump_sound.play();
        play_jump = false;
    }
}

void SoundManager::playScroll() {
    if (play_scroll) {
        scroll_sound.play();
        play_scroll = false;
    }
}

void SoundManager::playSelect() {
    if (play_select) {
        select_sound.play();
        play_select = false;
    }
}