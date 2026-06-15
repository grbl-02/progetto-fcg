#include "room.hpp"

Room::Room(std::string& filename)
{
    door_hitboxes.push_back(sf::FloatRect({5 * 32.f, 3 * 32.f}, {13.f, -1 * 32.f}));
    door_hitboxes.push_back(sf::FloatRect({7 * 32.f, 3 * 32.f}, {-13.f, -1 * 32.f}));
    load(filename);
}

void Room::draw(sf::RenderWindow& window, bool hitboxes)
{
    for (auto& tile : tiles)
        tile.draw(window);
    for (auto& asset : assets)
        asset.draw(window);
    for (auto& enemy : enemies)
        enemy->draw(window, hitboxes);

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
    name = new_room;
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
    blueSlimeTexture = sf::Texture(blueSlime);
    redSlimeTexture = sf::Texture(redSlime);
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
        dir enemyDirection;
        for (auto& [enemy_type, enemy_list] : mapData["enemies"].items())
        {
            name = enemy_type;
            for (auto& [enemy_id, enemy_data] : enemy_list.items())
            {
                pos = {enemy_data["pos"][0], enemy_data["pos"][1]};
                if (enemy_data["dir"] == "left") {
                    enemyDirection = LEFT;
                }
                else if (enemy_data["dir"] == "right") {
                    enemyDirection = RIGHT;
                }
                else if (enemy_data["dir"] == "up") {
                    enemyDirection = UP;
                }
                else if (enemy_data["dir"] == "down") {
                    enemyDirection = DOWN;
                }
                if (name == "blueslime")
                    enemies.push_back(std::make_unique<BlueSlime>(pos, blueSlimeTexture, enemyDirection));
                else
                    enemies.push_back(std::make_unique<RedSlime>(pos, redSlimeTexture, enemyDirection));
            }
        }
    }
}

void Room::enemyCollisions()
{
    for (auto& enemy : enemies)
    {
        for (auto& tile : tiles)
        {
            if (tile.name.find("WALL") != std::string::npos
                || tile.name.find("LEFTDOOR") != std::string::npos
                || tile.name.find("RIGHTDOOR") != std::string::npos)
            {
                sf::FloatRect tileBounds = tile.sprite.getGlobalBounds();
                if (auto intersecOp = enemy->hitbox.findIntersection(tileBounds))
                {
                    sf::FloatRect intersecRect = *intersecOp;
                    sf::Vector2f enemyCenter = enemy->hitbox.getCenter();
                    sf::Vector2f tileCenter = tileBounds.getCenter();
                    if (intersecRect.size.x < intersecRect.size.y)
                    {
                        // going to the right
                        if (enemyCenter.x < tileCenter.x) {
                            enemy->pos.x -= intersecRect.size.x;
                        }
                        // going to the left
                        else {
                            enemy->pos.x += intersecRect.size.x;
                        }
                    }
                    else
                    {
                        // going down
                        if (enemyCenter.y < tileCenter.y) {
                            enemy->pos.y -= intersecRect.size.y;
                        }
                        // going up
                        else {
                            enemy->pos.y += intersecRect.size.y;
                        }
                    }
                    enemy->isJumping = false;
                    enemy->cooldownTimer = 0.f; 
                    enemy->animation_frame = 0;
                }
                enemy->hitbox.position = {enemy->pos.x - 7.f, enemy->pos.y + 4.f};
            }
        }
        for (auto& asset : assets)
        {
            sf::FloatRect assetBounds = asset.sprite.getGlobalBounds();
            if (auto intersecOp = enemy->hitbox.findIntersection(assetBounds))
            {
                sf::FloatRect intersecRect = *intersecOp;
                sf::Vector2f enemyCenter = enemy->hitbox.getCenter();
                sf::Vector2f assetCenter = assetBounds.getCenter();
                if (intersecRect.size.x < intersecRect.size.y)
                {
                    // going to the right
                    if (enemyCenter.x < assetCenter.x) {
                        enemy->pos.x -= intersecRect.size.x;
                    }
                    // going to the left
                    else {
                        enemy->pos.x += intersecRect.size.x;
                    }
                }
                else
                {
                    // going down
                    if (enemyCenter.y < assetCenter.y) {
                        enemy->pos.y -= intersecRect.size.y;
                    }
                    // going up
                    else {
                        enemy->pos.y += intersecRect.size.y;
                    }
                }
                enemy->isJumping = false;
                enemy->cooldownTimer = 0.f; 
                enemy->animation_frame = 0;
                enemy->hitbox.position = {enemy->pos.x - 7.f, enemy->pos.y + 4.f};
            }
        }
        if (name == "Risorse/maps/room1.json"
            || name == "Risorse/maps/room2.json"
            || name == "Risorse/maps/room4.json")
        {
            for (auto& door_hitbox : door_hitboxes)
            {
                if (auto intersecOp = enemy->hitbox.findIntersection(door_hitbox))
                {
                    sf::FloatRect intersecRect = *intersecOp;
                    enemy->pos.y += intersecRect.size.y;
                    enemy->isJumping = false;
                    enemy->cooldownTimer = 0.f; 
                    enemy->animation_frame = 0;
                    enemy->hitbox.position = {enemy->pos.x - 7.f, enemy->pos.y + 4.f};
                }
            }
        }
    }
}

void Room::enemyWallCollisions()
{
    for (auto& enemy : enemies)
    {
        if (enemy->name == "blueSlime")
        {
            BlueSlime* blueSlime = static_cast<BlueSlime*>(enemy.get());
            sf::FloatRect left_exit_f = sf::FloatRect({(float)left_exit.position.x, (float)left_exit.position.y}, {(float)left_exit.size.x, (float)left_exit.size.y});
            sf::FloatRect right_exit_f = sf::FloatRect({(float)right_exit.position.x, (float)right_exit.position.y}, {(float)right_exit.size.x, (float)right_exit.size.y});
            sf::FloatRect up_exit_f = sf::FloatRect({(float)up_exit.position.x, (float)up_exit.position.y}, {(float)up_exit.size.x, (float)up_exit.size.y});
            sf::FloatRect down_exit_f = sf::FloatRect({(float)down_exit.position.x, (float)down_exit.position.y}, {(float)down_exit.size.x, (float)down_exit.size.y});
            auto exits = {left_exit_f, right_exit_f, up_exit_f, down_exit_f};
            std::optional<sf::FloatRect> intersecOp;
            sf::FloatRect collisionExit;
            for (const auto& exit : exits)
            {
                if (auto hit = blueSlime->hitbox.findIntersection(exit)) {
                    intersecOp = hit;
                    collisionExit = exit;
                    break;
                }
            }
            if (intersecOp)
            {
                sf::FloatRect intersecRect = *intersecOp;
                sf::Vector2f enemyCenter = enemy->hitbox.getCenter();
                sf::Vector2f exitCenter = collisionExit.getCenter();
                if (intersecRect.size.x < intersecRect.size.y)
                {
                    // going to the right
                    if (enemyCenter.x < exitCenter.x) {
                        enemy->pos.x -= intersecRect.size.x;
                    }
                    // going to the left
                    else {
                        enemy->pos.x += intersecRect.size.x;
                    }
                }
                else
                {
                    // going down
                    if (enemyCenter.y < exitCenter.y) {
                        enemy->pos.y -= intersecRect.size.y;
                    }
                    // going up
                    else {
                        enemy->pos.y += intersecRect.size.y;
                    }
                }
                blueSlime->isJumping = false;
                blueSlime->cooldownTimer = 0.f; 
                blueSlime->animation_frame = 0;
                enemy->hitbox.position = {enemy->pos.x - 7.f, enemy->pos.y + 4.f};
            }
        }
    }
}