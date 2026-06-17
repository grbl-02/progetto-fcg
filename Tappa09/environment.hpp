#pragma once
#include "config.hpp"

struct Tile
{
    sf::Sprite sprite;
    sf::Vector2f pos;
    std::string name;

    Tile(sf::Vector2f pos, const sf::Texture& texture, sf::IntRect textureRect, std::string name);
    void draw(sf::RenderWindow& window);
};

struct Asset
{
    sf::Sprite sprite;
    sf::Vector2f pos;
    sf::Vector2f size;
    std::string name;

    Asset(sf::Vector2f pos, const sf::Texture& texture, sf::IntRect textureRect, std::string name, sf::Vector2f size);
    void draw(sf::RenderWindow& window);
};