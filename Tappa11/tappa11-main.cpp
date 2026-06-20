#include "config.hpp"
#include "state.hpp"

// eventi

void handle_close(sf::RenderWindow& window) {
    window.close();
}

// mantiene lo stesso aspect ratio quando si ridimensiona la finestra
void handle_resize(const sf::Event::Resized& resized, sf::RenderWindow& window) {
    float aspect = static_cast<float>(window_width)/static_cast<float>(window_height);
    sf::Vector2u ws = resized.size;
    float new_aspect = static_cast<float>(ws.x)/static_cast<float>(ws.y);
    if (new_aspect < aspect)
        ws = {ws.x, static_cast<unsigned>(ws.x/aspect)};
    else
        ws = {static_cast<unsigned>(ws.y*aspect), ws.y};
    window.setSize(ws);
}

template <typename T>
void handle(T& event, State& state, sf::RenderWindow& window) {}

void handle(const sf::Event::KeyPressed& key, State& state, sf::RenderWindow& window) {
    switch (key.scancode) {
        case sf::Keyboard::Scancode::Up:
            if (state.gameMode == MENU) {
                if (state.menu.option == 0)
                    state.menu.option = 2;
                else state.menu.option --;
                state.soundManager.play_scroll = true;
            } else {
                state.move_player_up = true;
                state.playerMoving = true;
            }
            return;
        case sf::Keyboard::Scancode::Down:
            if (state.gameMode == MENU) {
                state.menu.option = (state.menu.option + 1) % 3;
                state.soundManager.play_scroll = true;
            } else {
                state.move_player_down = true;
                state.playerMoving = true;
            }
            return;
        case sf::Keyboard::Scancode::Left:
            state.move_player_left = true;
            state.playerMoving = true;
            return;
        case sf::Keyboard::Scancode::Right:
            state.move_player_right = true;
            state.playerMoving = true;
            return;
        case sf::Keyboard::Scancode::H:
            state.hitboxes = !state.hitboxes;
            return;
        case sf::Keyboard::Scancode::X:
            state.playerAttacks = true;
            return;
        case sf::Keyboard::Scancode::Z:
            if (state.gameMode == MENU) {
                if (state.menu.option == 0)
                    state.reset(NORMAL);
                else if (state.menu.option == 1)
                    state.reset(HARD);
                else
                    window.close();
                state.soundManager.play_select = true;
            } else {
                if (state.interactionIsHappening)
                    state.continueDialogue = true;
                if (state.gameMode != VICTORY)
                    state.playerInteracting = true;
            }
            return;
        case sf::Keyboard::Scancode::Enter:
            if (state.gameMode == GAME_OVER || state.gameMode == VICTORY) {
                state.soundManager.stopDungeon();
                state.soundManager.stopVictory();
                state.reset(state.difficulty);
            }
            return;
        case sf::Keyboard::Scancode::Escape:
            if (state.gameMode == GAME_OVER || state.gameMode == VICTORY) {
                state.soundManager.stopDungeon();
                state.soundManager.stopVictory();
                state.gameMode = MENU;
            }
            return;
        default:
            return;
    }
}

void handle(const sf::Event::KeyReleased& key, State& state, sf::RenderWindow& window)
{
    switch (key.scancode) {
        case sf::Keyboard::Scancode::Up:
            state.move_player_up = false;
            state.lastPressed = UP;
            state.playerMoving = false;
            return;
        case sf::Keyboard::Scancode::Down:
            state.move_player_down = false;
            state.lastPressed = DOWN;
            state.playerMoving = false;
            return;
        case sf::Keyboard::Scancode::Left:
            state.move_player_left = false;
            state.lastPressed = LEFT;
            state.playerMoving = false;
            return;
        case sf::Keyboard::Scancode::Right:
            state.move_player_right = false;
            state.lastPressed = RIGHT;
            state.playerMoving = false;
            return;
        default:
            return;
    }
}


// loop

int main() {
    sf::Vector2u window_size({window_width, window_height});
    sf::RenderWindow window(sf::VideoMode(window_size), window_title);

    window.setFramerateLimit(max_frame_rate);
    window.setMinimumSize(window.getSize());
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::Vector2i centerPosition(
        (int)((desktop.size.x - window_size.x) / 2),
        (int)((desktop.size.y - window_size.y) / 2)
    );
    window.setPosition(centerPosition);

    sf::View camera = sf::View(sf::FloatRect({0.0, 0.0}, {384.f, 288.f}));
    window.setView(camera);

    sf::Shader shader;
    if (!shader.loadFromFile("Tappa11/flash.frag", sf::Shader::Type::Fragment)) {
        fprintf(stderr, "Error: could not load shader.\n");
        return 1;
    }

    sf::Shader redShader;
    if (!redShader.loadFromFile("Tappa11/redflash.frag", sf::Shader::Type::Fragment)) {
        fprintf(stderr, "Error: could not load shader.\n");
        return 1;
    }

    sf::Font font;
    if (!font.openFromFile("Risorse/fonts/PublicPixel.ttf")) {
        fprintf(stderr, "Error: could not open font.\n");
        return -1; 
    }
    const_cast<sf::Texture&>(font.getTexture(32)).setSmooth(false);
    const_cast<sf::Texture&>(font.getTexture(8)).setSmooth(false);

    State state = State(shader, redShader, font);
    sf::Clock clock;

    while (window.isOpen()) {
        // eventi
        window.handleEvents(
            [&window](const sf::Event::Closed&) { handle_close(window); },
            [&window](const sf::Event::Resized& event) { handle_resize(event, window); },
            [&state, &window](const auto& event) { handle(event, state, window); } 
        );

        // update
        state.update(clock.restart().asSeconds(), camera);

        // display
        window.clear(sf::Color::Black);
        window.setView(camera);
        state.draw(window, shader, redShader);
        window.display();
    }
}