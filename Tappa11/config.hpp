#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// finestra
inline const char* window_title = "Tappa 11";
inline const unsigned window_width = 1152;
inline const unsigned window_height = 864;
inline const float max_frame_rate = 60;

// giocatore
inline const char* player_texture = "Risorse/sprites/player.png";
inline const float player_speed = 120.0;
inline const float invincibility = 1.f;
inline const float invincibility_flash_interval = 0.025f;

// animazione
inline const float mov_frame_time = 0.1;
inline const float idle_frame_time = 0.2;
inline const float slash_frame_time = 0.075;
inline const float spawn_frame_time = 0.05f;
inline float screen_shake_duration = 0.f;
inline const float intensity = 10.f;
inline const sf::Vector2f center = {192.f, 144.f};
inline bool shake_done = false;

// stanza
inline const char* all_tiles = "Risorse/sprites/tiles-all-32x32.png";
inline const sf::Vector2i floor_tile_num = {12, 9};
inline std::string room0 = "Risorse/maps-09/room0.json";
inline const float fireball_speed = 60.f;
inline const char* grass_tiles = "Risorse/sprites/basic-tiles.png";

// oggetti
inline const char* all_assets = "Risorse/sprites/assets-all.png";
inline const char* spikes = "Risorse/sprites/spikes.png";
inline const float chest_falling_speed = 100.0;

// nemici
inline const char* blue_slime_path = "Risorse/sprites/slime.png";
inline const char* red_slime_path = "Risorse/sprites/redslime.png";
inline const char* fireball_path = "Risorse/sprites/fireballs.png";
inline const char* spawn_path = "Risorse/sprites/smoke.png";
inline const float jump_distance = 64.f;
inline const float jump_time = 0.5f;
inline const float cooldown = 1.f;
inline const float slime_mov_frame_time = jump_time / 6.f;
inline const float flash_duration = 0.1f;

// hud
inline const char* hearts_path = "Risorse/sprites/HealthUI.png";

enum Dir { UP, DOWN, LEFT, RIGHT };
enum GameMode { MENU, PLAYING, GAME_OVER, VICTORY };
enum Difficulty { NORMAL, HARD };

struct FloatCircle {
    sf::Vector2f center;
    float radius;
};

inline bool intersects(FloatCircle& circle, const sf::Vector2f& point) {
    sf::Vector2f difference = point - circle.center;
    float distance_squared = (difference.x * difference.x) + (difference.y * difference.y);
    float radius_squared = circle.radius * circle.radius;
    return distance_squared <= radius_squared;
}