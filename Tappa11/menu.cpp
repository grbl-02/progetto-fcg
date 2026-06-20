#include "menu.hpp"

Menu::Menu(sf::Font& font) : start_game(font, "START GAME", 8.f),
                             hard_mode(font, "HARD MODE", 8.f),
                             description(font, "You only have 1 HP. You get hit, you die.", 8.f),
                             quit(font, "QUIT", 8.f) {
    option = 0;
}

void Menu::draw(sf::RenderWindow& window) {
    start_game.setFillColor(sf::Color::White);
    start_game.setPosition({((float)window_width / 3.f - start_game.getGlobalBounds().size.x) / 2.f, (float)window_height / 3.f * 0.3f});
    window.draw(start_game);

    hard_mode.setFillColor(sf::Color::White);
    hard_mode.setPosition({((float)window_width / 3.f - hard_mode.getGlobalBounds().size.x) / 2.f, (float)window_height / 3.f * 0.5f});
    window.draw(hard_mode);

    quit.setFillColor(sf::Color::White);
    quit.setPosition({((float)window_width / 3.f - quit.getGlobalBounds().size.x) / 2.f, (float)window_height / 3.f * 0.7f});
    window.draw(quit);

    if (option == 1) {
        description.setFillColor(sf::Color::White);
        description.setPosition({((float)window_width / 3.f - description.getGlobalBounds().size.x) / 2.f, (float)window_height / 3.f - description.getGlobalBounds().size.y - 10.f});
        window.draw(description);
    }

    sf::RectangleShape rect;
    if (option == 0) {
        rect = sf::RectangleShape({start_game.getGlobalBounds().size.x + 4.f, start_game.getGlobalBounds().size.y + 4.f});
        rect.setPosition({start_game.getGlobalBounds().position.x - 2.f, start_game.getGlobalBounds().position.y - 2.f});
    } else if (option == 1) {
        rect = sf::RectangleShape({hard_mode.getGlobalBounds().size.x + 4.f, hard_mode.getGlobalBounds().size.y + 4.f});
        rect.setPosition({hard_mode.getGlobalBounds().position.x - 2.f, hard_mode.getGlobalBounds().position.y - 2.f});
    } else {
        rect = sf::RectangleShape({quit.getGlobalBounds().size.x + 4.f, quit.getGlobalBounds().size.y + 4.f});
        rect.setPosition({quit.getGlobalBounds().position.x - 2.f, quit.getGlobalBounds().position.y - 2.f});
    }
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(1.f);

    window.draw(rect);
}