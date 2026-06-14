#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

// finestra
const char* window_title = "Tappa 06";
const unsigned window_width = 1152;
const unsigned window_height = 864;
const float max_frame_rate = 60;

// giocatore
const char* player_texture = "Risorse/sprites/characters/player.png";
const float player_speed = 120.0;

// animazione
const float movFrameTime = 0.1;
const float idleFrameTime = 0.2;

// stanza
const char* all_tiles = "Risorse/basic_caves_and_dungeons/tiles/tiles-all-32x32.png";
const sf::Vector2f displacement = {0.0, 0.0};
const sf::Vector2i floor_tile_num = {12, 9};
std::string room1 = "Risorse/maps/room1.json";

// oggetti
const char* all_assets = "Risorse/basic_caves_and_dungeons/assets/assets-all.png";

// nemici
const char* slime = "Risorse/sprites/characters/slime.png";

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
    sf::FloatRect hitbox;

    Player();
    void draw(sf::RenderWindow& window, bool hitboxes);
    void animation(int row, float frameTime);
    void move_up(float elapsed);
    void move_down(float elapsed);
    void move_left(float elapsed);
    void move_right(float elapsed);
    void enter_left_pos();
    void enter_right_pos();
    void enter_up_pos();
    void enter_down_pos();
};

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

struct Enemy
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f pos;
    std::string name;

    Enemy(sf::Vector2f pos, const sf::Texture& texture, std::string name);
    void draw(sf::RenderWindow& window);
};

struct Room
{
    sf::Texture roomTexture;
    sf::Texture assetsTexture;
    sf::Texture slimeTexture;
    std::vector<Tile> tiles;
    std::vector<Asset> assets;
    std::vector<Enemy> enemies;
    sf::IntRect left_exit;
    sf::IntRect right_exit;
    sf::IntRect up_exit;
    sf::IntRect down_exit;

    std::string room_left;
    std::string room_right;
    std::string room_up;
    std::string room_down;

    Room(std::string& filename);
    sf::IntRect stringToIntRect(std::string tileID);
    void unload();
    void load(std::string& new_room);
    void draw(sf::RenderWindow& window, bool hitboxes);
};

struct State
{
    std::string roomname;
    Player player;
    Room room;

    bool move_player_up;
    bool move_player_down;
    bool move_player_left;
    bool move_player_right;
    bool playerMoving;
    bool hitboxes;
    dir lastPressed;
    std::vector<sf::FloatRect> door_hitboxes;

    State();
    void draw(sf::RenderWindow& window);
    void room_transition();
    void collisions(bool isX);
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
    float py = (float)window_height / 3.f - sy / 2.f + 6.f;
    pos = {px, py};
    speed = player_speed;
    isLeft = false;
    animation_clock.start();
    hitbox = sf::FloatRect({pos.x - 5.f, pos.y + 15.f}, {10.f, 3.f});
}

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

Enemy::Enemy(sf::Vector2f pos, const sf::Texture& texture, std::string name) : sprite(texture)
{
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(sf::IntRect({0, 32}, {32, 32}));
    float sx = (float)sprite.getTextureRect().size.x;
    float sy = (float)sprite.getTextureRect().size.y;
    sprite.setOrigin({sx / 2.f, sy / 2.f});
    this->pos = pos;
    this->name = name;
}

Room::Room(std::string& filename)
{
    load(filename);
}

State::State() : room(room1)
{
    roomname = room1;
    move_player_up = false;
    move_player_down = false;
    move_player_left = false;
    move_player_right = false;
    lastPressed = UP;
    playerMoving = false;
    hitboxes = false;
    door_hitboxes.push_back(sf::FloatRect({5 * 32.f, 3 * 32.f}, {13.f, -1 * 32.f}));
    door_hitboxes.push_back(sf::FloatRect({7 * 32.f, 3 * 32.f}, {-13.f, -1 * 32.f}));
}


// draw

void Player::draw(sf::RenderWindow& window, bool hitboxes)
{
    sprite.setPosition(pos);
    window.draw(sprite);
    if (hitboxes)
    {
        sf::RectangleShape hb = sf::RectangleShape(hitbox.size);
        hb.setPosition(hitbox.position);
        hb.setOutlineColor(sf::Color::White);
        hb.setOutlineThickness(1.f);
        hb.setFillColor(sf::Color::Transparent);
        window.draw(hb);
    }
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

void Enemy::draw(sf::RenderWindow& window)
{
    sprite.setPosition(pos);
    window.draw(sprite);
}

void Room::draw(sf::RenderWindow& window, bool hitboxes)
{
    for (auto& tile : tiles)
        tile.draw(window);
    for (auto& asset : assets)
        asset.draw(window);
    for (auto& enemy : enemies)
        enemy.draw(window);

    if (hitboxes)
    {
        sf::Vector2f sizeL = {(float)left_exit.size.x, (float)left_exit.size.y};
        sf::Vector2f posL = {(float)left_exit.position.x, (float)left_exit.position.y};
        sf::RectangleShape hb = sf::RectangleShape(sizeL);
        hb.setPosition(posL);
        hb.setOutlineColor(sf::Color::White);
        hb.setOutlineThickness(1.f);
        hb.setFillColor(sf::Color::Transparent);
        window.draw(hb);

        sizeL = {(float)right_exit.size.x, (float)right_exit.size.y};
        posL = {(float)right_exit.position.x, (float)right_exit.position.y};
        hb = sf::RectangleShape(sizeL);
        hb.setPosition(posL);
        hb.setOutlineColor(sf::Color::White);
        hb.setOutlineThickness(1.f);
        hb.setFillColor(sf::Color::Transparent);
        window.draw(hb);

        sizeL = {(float)up_exit.size.x, (float)up_exit.size.y};
        posL = {(float)up_exit.position.x, (float)up_exit.position.y};
        hb = sf::RectangleShape(sizeL);
        hb.setPosition(posL);
        hb.setOutlineColor(sf::Color::White);
        hb.setOutlineThickness(1.f);
        hb.setFillColor(sf::Color::Transparent);
        window.draw(hb);

        sizeL = {(float)down_exit.size.x, (float)down_exit.size.y};
        posL = {(float)down_exit.position.x, (float)down_exit.position.y};
        hb = sf::RectangleShape(sizeL);
        hb.setPosition(posL);
        hb.setOutlineColor(sf::Color::White);
        hb.setOutlineThickness(1.f);
        hb.setFillColor(sf::Color::Transparent);
        window.draw(hb);
    }
}

void State::draw(sf::RenderWindow& window)
{
    room.draw(window, hitboxes);
    player.draw(window, hitboxes);

    if (hitboxes && 
        (roomname == "Risorse/maps/room1.json"
        || roomname == "Risorse/maps/room2.json"
        || roomname == "Risorse/maps/room4.json"))
    {
        for (auto& hb : door_hitboxes)
        {
            sf::RectangleShape hitbox = sf::RectangleShape(hb.size);
            hitbox.setPosition(hb.position);
            hitbox.setOutlineColor(sf::Color::White);
            hitbox.setOutlineThickness(1.f);
            hitbox.setFillColor(sf::Color::Transparent);
            window.draw(hitbox);
        }
    }
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
    hitbox.position.y -= player_speed * elapsed;
    isLeft = false;
    animation(5, movFrameTime);
}

void Player::move_down(float elapsed)
{
    pos.y += player_speed * elapsed;
    hitbox.position.y += player_speed * elapsed;
    isLeft = false;
    animation(3, movFrameTime);
}

void Player::move_left(float elapsed)
{
    pos.x -= player_speed * elapsed;
    hitbox.position.x -= player_speed * elapsed;
    isLeft = true;
    animation(4, movFrameTime);
}

void Player::move_right(float elapsed)
{
    pos.x += player_speed * elapsed;
    hitbox.position.x += player_speed * elapsed;
    isLeft = false;
    animation(4, movFrameTime);
}

void Player::enter_left_pos()
{
    pos.x = 12 * 32 - 5.f;
    hitbox.position = {pos.x - 5.f, pos.y + 15.f};
}

void Player::enter_right_pos()
{
    pos.x = 5.f;
    hitbox.position = {pos.x - 5.f, pos.y + 15.f};
}

void Player::enter_up_pos()
{
    pos.y = (float)window_height / 3.f - (float)sprite.getTextureRect().size.y / 2.f + 6.f;
    hitbox.position = {pos.x - 5.f, pos.y + 15.f};
}

void Player::enter_down_pos()
{
    pos.y = 3 * 32 - 15.f;
    hitbox.position = {pos.x - 5.f, pos.y + 15.f};
    if (hitbox.position.x < 173.f)
    {
        hitbox.position.x = 173.f;
        pos.x = hitbox.position.x + 5.f;
    }
    if (hitbox.position.x + hitbox.size.x > 211.f)
    {
        hitbox.position.x = 211.f - hitbox.size.x;
        pos.x = hitbox.position.x + 5.f;
    }
}

sf::IntRect Room::stringToIntRect(std::string tileID)
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

void Room::unload()
{
    tiles.clear();
    assets.clear();
    enemies.clear();

    left_exit = sf::IntRect();
    right_exit = sf::IntRect();
    up_exit = sf::IntRect();
    down_exit = sf::IntRect();
}

void Room::load(std::string& new_room)
{
    std::ifstream file(new_room);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open map file: " + new_room);
    }

    room_left.clear();
    room_right.clear();
    room_up.clear();
    room_down.clear();

    nlohmann::json mapData;
    file >> mapData;

    roomTexture = sf::Texture(all_tiles);
    assetsTexture = sf::Texture(all_assets);
    slimeTexture = sf::Texture(slime);
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
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));
            }
            else if (tileID == "UPCWALLTRANS")
            {
                sf::IntRect floorRect = sf::IntRect({1*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));
            }
            else if (tileID == "UPRWALLTRANS" || tileID == "LEFTDOORTRANS2")
            {
                sf::IntRect floorRect = sf::IntRect({2*32, 0*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));
            }
            else if (tileID == "LWALLTRANS" || tileID == "LEFTDOORTRANS1")
            {
                sf::IntRect floorRect = sf::IntRect({2*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));
            }
            else if (tileID == "RWALLTRANS" || tileID == "RIGHTDOORTRANS1")
            {
                sf::IntRect floorRect = sf::IntRect({0*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));
            }
            else if (tileID == "DLWALLTRANS")
            {
                sf::IntRect floorRect = sf::IntRect({0*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));
            }
            else if (tileID == "DCWALLTRANS")
            {
                sf::IntRect floorRect = sf::IntRect({1*32, 0*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));    
            }
            else if (tileID == "DRWALLTRANS")
            {
                sf::IntRect floorRect = sf::IntRect({2*32, 1*32}, {32, 32});
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));
            }
            else if (tileID == "WALL2" || tileID == "DOOR3" || tileID == "DOOR4" || tileID == "LEFTDOOR2" || tileID == "RIGHTDOOR2")
            {
                sf::IntRect floorRect = stringToIntRect("FLOOR");
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));
            }

            tiles.push_back(Tile(tile_pos, roomTexture, textureRect, tileID));
        }
    }

    if (mapData["exits"].contains("left")) {
        left_exit = sf::IntRect({mapData["exits"]["left"]["pos"][0].get<int>() * 32, mapData["exits"]["left"]["pos"][1].get<int>() * 32},
                                {mapData["exits"]["left"]["size"][0].get<int>(), mapData["exits"]["left"]["size"][1].get<int>() * 32});
        room_left = mapData["exits"]["left"]["room"];
    };
    if (mapData["exits"].contains("right")) {
        right_exit = sf::IntRect({mapData["exits"]["right"]["pos"][0].get<int>() * 32, mapData["exits"]["right"]["pos"][1].get<int>() * 32},
                                 {mapData["exits"]["right"]["size"][0].get<int>(), mapData["exits"]["right"]["size"][1].get<int>() * 32});
        room_right = mapData["exits"]["right"]["room"];
    };
    if (mapData["exits"].contains("up")) {
        up_exit = sf::IntRect({mapData["exits"]["up"]["pos"][0].get<int>() * 32, mapData["exits"]["up"]["pos"][1].get<int>() * 32},
                              {mapData["exits"]["up"]["size"][0].get<int>() * 32 , mapData["exits"]["up"]["size"][1].get<int>()});
        room_up = mapData["exits"]["up"]["room"];
    };
    if (mapData["exits"].contains("down")) {
        down_exit = sf::IntRect({mapData["exits"]["down"]["pos"][0].get<int>() * 32, mapData["exits"]["down"]["pos"][1].get<int>() * 32},
                                {mapData["exits"]["down"]["size"][0].get<int>() * 32, mapData["exits"]["down"]["size"][1].get<int>()});
        room_down = mapData["exits"]["down"]["room"];
    };

    if (mapData.contains("assets"))
    {
        for (auto& [asset, fields] : mapData["assets"].items())
        {
            sf::Vector2f pos;
            sf::Vector2i size;
            sf::Vector2i texturePos;
            if (fields.contains("pos")) {
                pos.x = fields["pos"][0].get<float>();
                pos.y = fields["pos"][1].get<float>();
            }
            if (fields.contains("size")) {
                size.x = fields["size"][0].get<int>();
                size.y = fields["size"][1].get<int>();
            }
            if (fields.contains("spritepos")) {
                texturePos.x = fields["spritepos"][0].get<int>();
                texturePos.y = fields["spritepos"][1].get<int>();
            }
            sf::Vector2f fsize = sf::Vector2f({(float)size.x, (float)size.y});
            assets.push_back(Asset(pos, assetsTexture, sf::IntRect(texturePos, size), asset, fsize));
        }
    }

    if (mapData.contains("enemies"))
    {
        sf::Vector2f pos;
        std::string name;
        for (auto& [enemy_type, enemy_list] : mapData["enemies"].items())
        {
            name = enemy_type;
            for (auto& [enemy_id, enemy_data] : enemy_list.items())
            {
                pos = {enemy_data["pos"][0], enemy_data["pos"][1]};
                enemies.push_back(Enemy(pos, slimeTexture, name));
            }
        }
    }
}

void State::room_transition()
{
    
    if (!std::empty(room.room_left))
    {
        float hb1x = player.hitbox.position.x;
        float hb1y = player.hitbox.position.y;
        float ex1x = room.left_exit.position.x;
        float ex1y = room.left_exit.position.y - 10.f;
        float hb2y = player.hitbox.position.y + player.hitbox.size.y;
        float ex2y = room.left_exit.position.y + room.left_exit.size.y + 10.f;
        if (hb1x < ex1x && hb1y > ex1y && hb2y < ex2y)
        {
            room.unload();
            roomname = room.room_left;
            room.load(room.room_left);
            player.enter_left_pos();
        }
    }
    if (!std::empty(room.room_right))
    {
        float hb1x = player.hitbox.position.x + player.hitbox.size.x;
        float hb1y = player.hitbox.position.y;
        float ex1x = room.right_exit.position.x;
        float ex1y = room.right_exit.position.y - 10.f;
        float hb2y = player.hitbox.position.y + player.hitbox.size.y;
        float ex2y = room.right_exit.position.y + room.right_exit.size.y + 10.f;
        if (hb1x > ex1x && hb1y > ex1y && hb2y < ex2y)
        {
            room.unload();
            roomname = room.room_right;
            room.load(room.room_right);
            player.enter_right_pos();
        }
    }
    if (!std::empty(room.room_up))
    {
        float hb1x = player.hitbox.position.x;
        float hb1y = player.hitbox.position.y;
        float ex1x = room.up_exit.position.x - 10.f;
        float ex1y = room.up_exit.position.y - 5.f;
        float hb2x = player.hitbox.position.x + player.hitbox.size.x;
        float ex2x = room.up_exit.position.x + room.up_exit.size.x + 10.f;
        if (hb1x > ex1x && hb1y < ex1y && hb2x < ex2x)
        {
            room.unload();
            roomname = room.room_up;
            room.load(room.room_up);
            player.enter_up_pos();
        }
    }
    if (!std::empty(room.room_down))
    {
        float hb1x = player.hitbox.position.x;
        float hb1y = player.hitbox.position.y + player.hitbox.size.y;
        float ex1x = room.down_exit.position.x - 10.f;
        float ex1y = room.down_exit.position.y;
        float hb2x = player.hitbox.position.x + player.hitbox.size.x;
        float ex2x = room.down_exit.position.x + room.down_exit.size.x + 10.f;
        if (hb1x > ex1x && hb1y > ex1y && hb2x < ex2x)
        {
            room.unload();
            roomname = room.room_down;
            room.load(room.room_down);
            player.enter_down_pos();
        }
    }
}

void State::collisions(bool isX)
{
    for (auto& tile : room.tiles)
    {
        if (tile.name.find("WALL") != std::string::npos
            || tile.name.find("LEFTDOOR") != std::string::npos
            || tile.name.find("RIGHTDOOR") != std::string::npos)
        {
            sf::FloatRect tileBounds = tile.sprite.getGlobalBounds();
            if (auto intersecOp = player.hitbox.findIntersection(tileBounds))
            {
                sf::FloatRect intersecRect = *intersecOp;
                sf::Vector2f playerCenter = player.hitbox.getCenter();
                sf::Vector2f tileCenter = tileBounds.getCenter();

                if (isX)
                {
                    // going to the right
                    if (playerCenter.x < tileCenter.x) {
                        player.pos.x -= intersecRect.size.x;
                    }
                    // going to the left
                    else {
                        player.pos.x += intersecRect.size.x;
                    }
                }
                else
                {
                    // going down
                    if (playerCenter.y < tileCenter.y) {
                        player.pos.y -= intersecRect.size.y;
                    }
                    // going up
                    else {
                        player.pos.y += intersecRect.size.y;
                    }
                }
                player.hitbox.position = {player.pos.x - 5.f, player.pos.y + 15.f};
            }
        }
    }
    for (auto& asset : room.assets)
    {
        sf::FloatRect assetBounds = asset.sprite.getGlobalBounds();
        if (auto intersecOp = player.hitbox.findIntersection(assetBounds))
        {
            sf::FloatRect intersecRect = *intersecOp;
            sf::Vector2f playerCenter = player.hitbox.getCenter();
            sf::Vector2f assetCenter = assetBounds.getCenter();

            if (isX)
            {
                // going to the right
                if (playerCenter.x < assetCenter.x) {
                    player.pos.x -= intersecRect.size.x;
                }
                // going to the left
                else {
                    player.pos.x += intersecRect.size.x;
                }
            }
            else
            {
                // going down
                if (playerCenter.y < assetCenter.y) {
                    player.pos.y -= intersecRect.size.y;
                }
                // going up
                else {
                    player.pos.y += intersecRect.size.y;
                }
            }
            player.hitbox.position = {player.pos.x - 5.f, player.pos.y + 15.f};
        }
    }
    if (roomname == "Risorse/maps/room1.json"
        || roomname == "Risorse/maps/room2.json"
        || roomname == "Risorse/maps/room4.json")
    {
        for (auto& door_hitbox : door_hitboxes)
        {
            if (auto intersecOp = player.hitbox.findIntersection(door_hitbox))
            {
                sf::FloatRect intersecRect = *intersecOp;
                player.pos.y += intersecRect.size.y;
                player.hitbox.position = {player.pos.x - 5.f, player.pos.y + 15.f};
            }
        }
    }
}

void State::update(float elapsed)
{
    if (move_player_left)
        player.move_left(elapsed);
    if (move_player_right)
        player.move_right(elapsed);
    collisions(true);

    if (move_player_up)
        player.move_up(elapsed);
    if (move_player_down)
        player.move_down(elapsed);
    collisions(false);
    
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
    room_transition();
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
        case sf::Keyboard::Scancode::H:
            state.hitboxes = !state.hitboxes;
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