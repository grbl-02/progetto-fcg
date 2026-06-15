#pragma once
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

// finestra
inline const char* window_title = "Tappa 07";
inline const unsigned window_width = 1152;
inline const unsigned window_height = 864;
inline const float max_frame_rate = 60;

// giocatore
inline const char* player_texture = "Risorse/sprites/characters/player.png";
inline const float player_speed = 120.0;

// animazione
inline const float movFrameTime = 0.1;
inline const float idleFrameTime = 0.2;

// stanza
inline const char* all_tiles = "Risorse/basic_caves_and_dungeons/tiles/tiles-all-32x32.png";
inline const sf::Vector2f displacement = {0.0, 0.0};
inline const sf::Vector2i floor_tile_num = {12, 9};
inline std::string room1 = "Risorse/maps/room1.json";
inline const float fireball_speed = 100.0;

// oggetti
inline const char* all_assets = "Risorse/basic_caves_and_dungeons/assets/assets-all.png";

// nemici
inline const char* blueSlime = "Risorse/sprites/characters/slime.png";
inline const char* redSlime = "Risorse/sprites/characters/redslime.png";
inline const char* fireballSprites = "Risorse/sprites/characters/fireballs.png";
inline const float jumpdist = 64.f;
inline const float jumptime = 0.5f;
inline const float cooldown = 1.0f;
inline const float slimeMovFrameTime = jumptime / 6.f;

enum dir { UP, DOWN, LEFT, RIGHT };

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