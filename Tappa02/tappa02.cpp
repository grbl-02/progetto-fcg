#include <SFML/Graphics.hpp>

// finestra
const char* window_title = "Tappa 02";
const unsigned window_width = 800;
const unsigned window_height = 600;
const float max_frame_rate = 60;

// giocatore
const char* player_texture = "Risorse/sprites/player.png";
const float player_speed = 80.0;

// camera
const float zoom_factor = 0.3f;

// animazione
const float movFrameTime = 0.125;
const float idleFrameTime = 0.2;

enum dir { UP, DOWN, LEFT, RIGHT };

// stato

struct Player
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;
    float speed;
    int animation_frame = 0;
    bool isLeft;
    sf::Clock animation_clock;

    Player();
    void draw(sf::RenderWindow& window);
    void animation(int row, float frameTime);
    void move_up(float elapsed);
    void move_down(float elapsed);
    void move_left(float elapsed);
    void move_right(float elapsed);
};

struct State
{
    Player player;
    bool move_player_up;
    bool move_player_down;
    bool move_player_left;
    bool move_player_right;
    bool playerMoving;
    dir lastPressed;

    State();
    void draw(sf::RenderWindow& window);
    void update(float elapsed);
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
    speed = player_speed;
    isLeft = false;
    animation_clock.start();
}

State::State()
{
    move_player_up = false;
    move_player_down = false;
    move_player_left = false;
    move_player_right = false;
    lastPressed = UP;
    playerMoving = false;
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

// update

void Player::animation(int row, float frameTime)
{
    if (animation_clock.getElapsedTime().asSeconds() >= frameTime)
    {
        animation_clock.restart();
        sf::IntRect curFrame = sf::IntRect({animation_frame * 48, row * 48}, {48, 48});
        sprite.setTextureRect(curFrame);
        if (isLeft)
            sprite.setScale({-1.f, 1.f});
        else
            sprite.setScale({1.f, 1.f});
        animation_frame = (animation_frame + 1) % 6;
    }
}

void Player::move_up(float elapsed)
{
    pos.y -= player_speed * elapsed;
    isLeft = false;
    animation(5, movFrameTime);
}

void Player::move_down(float elapsed)
{
    pos.y += player_speed * elapsed;
    isLeft = false;
    animation(3, movFrameTime);
}

void Player::move_left(float elapsed)
{
    pos.x -= player_speed * elapsed;
    isLeft = true;
    animation(4, movFrameTime);
}

void Player::move_right(float elapsed)
{
    pos.x += player_speed * elapsed;
    isLeft = false;
    animation(4, movFrameTime);
}

void State::update(float elapsed)
{
    if (move_player_up)
        player.move_up(elapsed);
    if (move_player_down)
        player.move_down(elapsed);
    if (move_player_left)
        player.move_left(elapsed);
    if (move_player_right)
        player.move_right(elapsed);
    if (!playerMoving)
    {
        int row = 0;
        switch (lastPressed)
        {
            case UP:
                row = 2;
                break;
            case DOWN:
                row = 0;
                break;
            case LEFT:
                row = 1;
                break;
            case RIGHT:
                row = 1;
                break;
        }
        player.animation(row, idleFrameTime);
    }
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

template <typename T>
void handle(T& event, State& state) {}

void handle(const sf::Event::KeyPressed& key, State& state)
{
    switch (key.scancode)
    {
        case sf::Keyboard::Scancode::Up:
            state.move_player_up = true;
            state.playerMoving = true;
            return;
        case sf::Keyboard::Scancode::Down:
            state.move_player_down = true;
            state.playerMoving = true;
            return;
        case sf::Keyboard::Scancode::Left:
            state.move_player_left = true;
            state.playerMoving = true;
            return;
        case sf::Keyboard::Scancode::Right:
            state.move_player_right = true;
            state.playerMoving = true;
            return;
        default:
            return;
    }
}

void handle(const sf::Event::KeyReleased& key, State& state)
{
    switch (key.scancode)
    {
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
    window.setView(camera);

    State state;
    sf::Clock clock;

    while (window.isOpen())
    {
        // eventi
        window.handleEvents(
            [&window](const sf::Event::Closed&) { handle_close(window); },
            [&window](const sf::Event::Resized& event) { handle_resize(event, window); },
            [&state](const auto& event) { handle(event, state); } 
        );

        // update
        state.update(clock.restart().asSeconds());

        // display
        window.clear(sf::Color::Black);
        state.draw(window);
        window.display();
    }
}