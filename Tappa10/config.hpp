#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

// finestra
inline const char* window_title = "Tappa 10";
inline const unsigned window_width = 1152;
inline const unsigned window_height = 864;
inline const float max_frame_rate = 60;

// giocatore
inline const char* player_texture = "Risorse/sprites/player.png";
inline const float player_speed = 120.0;
inline const float invincibility = 1.f;
inline const float invincibilityFlashInterval = 0.025f;

// animazione
inline const float movFrameTime = 0.1;
inline const float idleFrameTime = 0.2;
inline const float slashFrameTime = 0.075;
inline const float spawnFrameTime = 0.05f;
inline float screenShakeDuration = 0.f;
inline const float intensity = 10.f;
inline const sf::Vector2f center = {192.f, 144.f};
inline bool shakeDone = false;

// stanza
inline const char* all_tiles = "Risorse/sprites/tiles-all-32x32.png";
inline const sf::Vector2f displacement = {0.0, 0.0};
inline const sf::Vector2i floor_tile_num = {12, 9};
inline std::string room0 = "Risorse/maps-09/room0.json";
inline const float fireball_speed = 60.f;
inline const char* grass_tiles = "Risorse/sprites/basic-tiles.png";

// oggetti
inline const char* all_assets = "Risorse/sprites/assets-all.png";
inline const char* spikes = "Risorse/sprites/spikes.png";
inline const float chest_falling_speed = 100.0;

// nemici
inline const char* blueSlime = "Risorse/sprites/slime.png";
inline const char* redSlime = "Risorse/sprites/redslime.png";
inline const char* fireballSprites = "Risorse/sprites/fireballs.png";
inline const char* spawnSprites = "Risorse/sprites/smoke.png";
inline const float jumpdist = 64.f;
inline const float jumptime = 0.5f;
inline const float cooldown = 1.f;
inline const float slimeMovFrameTime = jumptime / 6.f;
inline const float flash_duration = 0.1f;

inline const char* heartsSprites = "Risorse/sprites/HealthUI.png";

enum dir { UP, DOWN, LEFT, RIGHT };
enum GameMode { MENU, PLAYING, GAME_OVER, VICTORY };
enum Difficulty { NORMAL, HARD };

struct FloatCircle {
    sf::Vector2f center;
    float radius;
};

inline bool intersects(FloatCircle& circle, const sf::Vector2f& point)
{
    sf::Vector2f difference = point - circle.center;
    float distanceSquared = (difference.x * difference.x) + (difference.y * difference.y);
    float radiusSquared = circle.radius * circle.radius;
    return distanceSquared <= radiusSquared;
}