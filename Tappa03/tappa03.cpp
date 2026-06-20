#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

// finestra
const char* window_title = "Tappa 03";
const unsigned window_width = 1152;
const unsigned window_height = 864;
const float max_frame_rate = 60;

// giocatore
const char* player_texture = "Risorse/sprites/player.png";
const float player_speed = 120.0;

// animazione
const float movFrameTime = 0.1;
const float idleFrameTime = 0.2;

// stanza
const char* all_tiles = "Risorse/sprites/tiles-all-32x32.png";
const sf::Vector2f displacement = {0.0, 0.0};
const sf::Vector2i floor_tile_num = {12, 9};

enum dir { UP, DOWN, LEFT, RIGHT };


// struct

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

struct Tile
{
    sf::Sprite sprite;
    sf::Vector2f pos;

    Tile(sf::Vector2f pos, const sf::Texture& texture, sf::IntRect textureRect);
    void draw(sf::RenderWindow& window);
};

struct Room
{
    sf::Texture roomTexture;
    std::vector<Tile> tiles;

    Room(const std::string& filename);
    void draw(sf::RenderWindow& window);
};

struct State
{
    std::string filename;
    Player player;
    Room room;

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


// costruttori

Player::Player() : sprite(texture)
{
    texture = sf::Texture(player_texture);
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(sf::IntRect({0, 96}, {48, 48}));
    float sx = (float)sprite.getTextureRect().size.x;
    float sy = (float)sprite.getTextureRect().size.y;
    sprite.setOrigin({sx / 2.f, sy / 2.f});
    float px = (float)window_width / (3.f * 2.f);
    float py = (float)window_height / 3.f - sy / 2.0 - 32;
    pos = {px, py};
    speed = player_speed;
    isLeft = false;
    animation_clock.start();
}

Tile::Tile(sf::Vector2f pos, const sf::Texture& texture, sf::IntRect textureRect) : sprite(texture)
{
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(textureRect);
    this->pos = pos;
}

sf::IntRect stringToIntRect(std::string tileID)
{
    if (tileID == "UPLWALLTRANS")
        return sf::IntRect({4*32, 0*32}, {32, 32});
    else if (tileID == "UPCWALLTRANS")
        return sf::IntRect({5*32, 0*32}, {32, 32});
    else if (tileID == "UPRWALLTRANS")
        return sf::IntRect({6*32, 0*32}, {32, 32});
    else if (tileID == "LWALLTRANS")
        return sf::IntRect({4*32, 1*32}, {32, 32});
    else if (tileID == "RWALLTRANS")
        return sf::IntRect({6*32, 1*32}, {32, 32});
    else if (tileID == "DLWALLTRANS")
        return sf::IntRect({4*32, 2*32}, {32, 32});
    else if (tileID == "DCWALLTRANS")
        return sf::IntRect({5*32, 2*32}, {32, 32});
    else if (tileID == "DRWALLTRANS")
        return sf::IntRect({6*32, 2*32}, {32, 32});
    else if (tileID == "LEFTDOORTRANS1")
        return sf::IntRect({5*32, 4*32}, {32, 32});
    else if (tileID == "RIGHTDOORTRANS1")
        return sf::IntRect({4*32, 4*32}, {32, 32});
    else if (tileID == "LEFTDOORTRANS2")
        return sf::IntRect({5*32, 3*32}, {32, 32});
    else if (tileID == "RIGHTDOORTRANS2")
        return sf::IntRect({4*32, 3*32}, {32, 32});
    else if (tileID == "WALL1")
        return sf::IntRect({1*32, 2*32}, {32, 32});
    else if (tileID == "WALL2")
        return sf::IntRect({1*32, 4*32}, {32, 32});
    else if (tileID == "DOOR1")
        return sf::IntRect({2*32, 5*32}, {32, 32});
    else if (tileID == "DOOR2")
        return sf::IntRect({3*32, 5*32}, {32, 32});
    else if (tileID == "DOOR3")
        return sf::IntRect({2*32, 6*32}, {32, 32});
    else if (tileID == "DOOR4")
        return sf::IntRect({3*32, 6*32}, {32, 32});
    else if (tileID == "LEFTDOOR1")
        return sf::IntRect({2*32, 2*32}, {32, 32});
    else if (tileID == "LEFTDOOR2")
        return sf::IntRect({2*32, 4*32}, {32, 32});
    else if (tileID == "RIGHTDOOR1")
        return sf::IntRect({0*32, 2*32}, {32, 32});
    else if (tileID == "RIGHTDOOR2")
        return sf::IntRect({0*32, 4*32}, {32, 32});
    else if (tileID == "FLOORDECOR1")
        return sf::IntRect({11*32, 8*32}, {32, 32});
    else if (tileID == "FLOORDECOR2")
        return sf::IntRect({12*32, 8*32}, {32, 32});
    else if (tileID == "FLOORDECOR3")
        return sf::IntRect({13*32, 8*32}, {32, 32});
    else if (tileID == "FLOORDECOR4")
        return sf::IntRect({11*32, 9*32}, {32, 32});
    else// if (tileID == "FLOOR")
        return sf::IntRect({8*32, 1*32}, {32, 32});
}

Room::Room(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open map file: " + filename);
    }

    nlohmann::json mapData;
    file >> mapData;

    roomTexture = sf::Texture(all_tiles);
    for (int ty = 0; ty < floor_tile_num.y; ty++)
    {
        for (int tx = 0; tx < floor_tile_num.x; tx++)
        {
            std::string tileID = mapData["grid"][ty * floor_tile_num.x + tx];
            sf::IntRect textureRect = stringToIntRect(tileID);
            sf::Vector2f tile_pos = {
                tx * 32 + displacement.x,
                ty * 32 + displacement.y
            };
            if (tileID == "UPLWALLTRANS" || tileID == "RIGHTDOORTRANS2")
            {
                sf::IntRect floorRect = sf::IntRect({0*32, 0*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));
            }
            else if (tileID == "UPCWALLTRANS")
            {
                sf::IntRect floorRect = sf::IntRect({1*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));
            }
            else if (tileID == "UPRWALLTRANS" || tileID == "LEFTDOORTRANS2")
            {
                sf::IntRect floorRect = sf::IntRect({2*32, 0*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));
            }
            else if (tileID == "LWALLTRANS" || tileID == "LEFTDOORTRANS1")
            {
                sf::IntRect floorRect = sf::IntRect({2*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));
            }
            else if (tileID == "RWALLTRANS" || tileID == "RIGHTDOORTRANS1")
            {
                sf::IntRect floorRect = sf::IntRect({0*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));
            }
            else if (tileID == "DLWALLTRANS")
            {
                sf::IntRect floorRect = sf::IntRect({0*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));
            }
            else if (tileID == "DCWALLTRANS")
            {
                sf::IntRect floorRect = sf::IntRect({1*32, 0*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));    
            }
            else if (tileID == "DRWALLTRANS")
            {
                sf::IntRect floorRect = sf::IntRect({2*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));
            }
            else if (tileID == "WALL2" || tileID == "DOOR3" || tileID == "DOOR4" || tileID == "LEFTDOOR2" || tileID == "RIGHTDOOR2")
            {
                sf::IntRect floorRect = stringToIntRect("FLOOR");
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect));
            }

            tiles.push_back(Tile(tile_pos, roomTexture, textureRect));
        }
    }
}

State::State() : room("Risorse/maps/room1.json")
{
    filename = "Risorse/maps/room1.json";
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

void Tile::draw(sf::RenderWindow& window)
{
    sprite.setPosition(pos);
    window.draw(sprite);
}

void Room::draw(sf::RenderWindow& window)
{
    for (auto& tile : tiles)
        tile.draw(window);
}

void State::draw(sf::RenderWindow& window)
{
    room.draw(window);
    player.draw(window);
}


// metodi

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