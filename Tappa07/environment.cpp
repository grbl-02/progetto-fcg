#include "environment.hpp"

Tile::Tile(sf::Vector2f pos, const sf::Texture& texture, sf::IntRect textureRect, std::string name) : sprite(texture)
{
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(textureRect);
    this->pos = pos;
    this->name = name;
}

Asset::Asset(sf::Vector2f pos, const sf::Texture& texture, sf::IntRect textureRect, std::string name, sf::Vector2f size) : sprite(texture)
{
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(textureRect);
    this->pos = pos;
    this->name = name;
    this->size = size;
}

void Tile::draw(sf::RenderWindow& window)
{
    sprite.setPosition(pos);
    window.draw(sprite);
}

void Asset::draw(sf::RenderWindow& window)
{
    sprite.setPosition(pos);
    window.draw(sprite);
}