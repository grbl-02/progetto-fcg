#include <SFML/Graphics.hpp>

// finestra
const char* window_title = "Tappa 01";
const unsigned window_width = 800;
const unsigned window_height = 600;
const float max_frame_rate = 60;

// giocatore
const char* player_texture = "Risorse/sprites/characters/player.png";

// camera
const float zoom_factor = 0.3f;

// stato

struct Player
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;

    Player();
    void draw(sf::RenderWindow& window);
};

struct State
{
    Player player;

    State();
    void draw(sf::RenderWindow& window);
};

Player::Player() : sprite(texture)
{
    texture = sf::Texture(player_texture);
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(sf::IntRect({0, 96}, {48, 48}));
    float sx = (float)sprite.getTextureRect().size.x;
    float sy = (float)sprite.getTextureRect().size.y;
    sprite.setOrigin({sx / 2.f, sy / 2.f});
    float px = (float)window_width / 2.0;
    float py = (float)window_height - sy / 2.0;
    pos = {px, py};
}

State::State()
{
    
}

// draw

void Player::draw(sf::RenderWindow& window)
{
    sprite.setPosition(pos);
    window.draw(sprite);
}

void State::draw(sf::RenderWindow& window)
{
    player.draw(window);
}

// eventi

void handle_close(sf::RenderWindow& window)
{
    window.close();
}

// mantiene lo stesso aspect ratio quando si ridimensiona la finestra
void handle_resize(const sf::Event::Resized& resized, sf::RenderWindow& window)
{
    float aspect = static_cast<float>(window_width)/static_cast<float>(window_height);
    sf::Vector2u ws = resized.size;
    float new_aspect = static_cast<float>(ws.x)/static_cast<float>(ws.y);
    if (new_aspect < aspect)
    {
        ws = {ws.x, static_cast<unsigned>(ws.x/aspect)};
    }
    else
    {
        ws = {static_cast<unsigned>(ws.y*aspect), ws.y};
    }
    window.setSize(ws);
}

// loop

int main()
{
    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), window_title);
    window.setFramerateLimit(max_frame_rate);
    window.setMinimumSize(window.getSize());

    sf::View camera = sf::View(sf::FloatRect({0.0, 0.0}, {window_width, window_height}));
    camera.zoom(zoom_factor);
    float newHeight = (float)window_height * zoom_factor;
    float newX = (float)window_width / 2.f;
    float newY = (float)window_height - (newHeight / 2.f);
    camera.setCenter({newX, newY});

    State state;

    while (window.isOpen())
    {
        // eventi
        window.handleEvents(
            [&window](const sf::Event::Closed&) { handle_close(window); },
            [&window](const sf::Event::Resized& event) { handle_resize(event, window); }
        );

        // display
        window.clear(sf::Color::Black);

        window.setView(camera);
        state.draw(window);

        window.display();
    }
}