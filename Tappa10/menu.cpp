#include "menu.hpp"

Menu::Menu(sf::Font& font) : start_game(font, "START GAME", 8.f), quit(font, "QUIT", 8.f)
{
    option = 0;
}

void Menu::draw(sf::RenderWindow& window)
{
    start_game.setFillColor(sf::Color::White);
    start_game.setPosition({((float)window_width / 3.f - start_game.getGlobalBounds().size.x) / 2.f, (float)window_height / 3.f * 0.4f});
    window.draw(start_game);
    quit.setFillColor(sf::Color::White);
    quit.setPosition({((float)window_width / 3.f - quit.getGlobalBounds().size.x) / 2.f, (float)window_height / 3.f * 0.6f});
    window.draw(quit);
    
    sf::RectangleShape rect;
    if (option == 0)
    {
        rect = sf::RectangleShape({start_game.getGlobalBounds().size.x + 4.f, start_game.getGlobalBounds().size.y + 4.f});
        rect.setPosition({start_game.getGlobalBounds().position.x - 2.f, start_game.getGlobalBounds().position.y - 2.f});
    }
    else
    {
        rect = sf::RectangleShape({quit.getGlobalBounds().size.x + 4.f, quit.getGlobalBounds().size.y + 4.f});
        rect.setPosition({quit.getGlobalBounds().position.x - 2.f, quit.getGlobalBounds().position.y - 2.f});
    }
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(1.f);

    window.draw(rect);
}