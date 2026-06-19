#include "room.hpp"

Room::Room(std::string& filename, Flags& flags) : conditionEvaluator(flags)
{
    start_animations = false;
    clock_active = false;
    spikesDisappearing = false;
    animationEnded = false;
    animation_clock.reset();
    chestFalls = false;
    roomTexture = sf::Texture(all_tiles);
    assetsTexture = sf::Texture(all_assets);
    blueSlimeTexture = sf::Texture(blueSlime);
    redSlimeTexture = sf::Texture(redSlime);
    spikesTexture = sf::Texture(spikes);
    grassTexture = sf::Texture(grass_tiles);
    this->flags = &flags;
    conditionEvaluator = ConditionEvaluator(flags);
    door_hitboxes.push_back(sf::FloatRect({5 * 32.f, 3 * 32.f}, {13.f, -1 * 32.f}));
    door_hitboxes.push_back(sf::FloatRect({7 * 32.f, 3 * 32.f}, {-13.f, -1 * 32.f}));
    load(filename);
}

void Room::draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash)
{
    for (auto& tile : tiles)
        tile.draw(window);
    for (auto& asset : assets)
        asset.draw(window);
    for (auto& enemy : enemies)
        enemy->draw(window, hitboxes, flash);

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
    else if (tileID == "GRASS")
        return sf::IntRect({32, 0}, {32, 32});
    else if (tileID == "GRASS1")
        return sf::IntRect({144, 32}, {32, 32});
    else if (tileID == "GRASSUP")
        return sf::IntRect({0, 0}, {32, 32});
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

    start_animations = false;
    clock_active = false;
    animation_clock.reset();
    for (int i = 0; i < 2; i++)
        animation_frames[i] = 0;
    spikesDisappearing = false;
    animationEnded = false;
    chestFalls = false;
}

void Room::load(std::string& new_room)
{
    name = new_room;
    std::ifstream file(new_room);

    if (!file.is_open())
    {
        fprintf(stderr, "Failed to open map file");
    }

    room_left.clear();
    room_right.clear();
    room_up.clear();
    room_down.clear();

    nlohmann::json mapData;
    file >> mapData;
    
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
            else if (tileID == "GRASS1")
            {
                sf::IntRect floorRect = sf::IntRect({32, 0}, {32, 32});
                tiles.push_back(Tile(tile_pos, grassTexture, floorRect, "GRASS"));
            }
            else if (tileID == "GRASSUP")
            {
                sf::IntRect floorRect = stringToIntRect("FLOOR");
                tiles.push_back(Tile(tile_pos, roomTexture, floorRect, "BACKGROUND"));   
            }
            if (tileID == "GRASS" || tileID == "GRASS1" || tileID == "GRASSUP") tiles.push_back(Tile(tile_pos, grassTexture, textureRect, tileID));
            else tiles.push_back(Tile(tile_pos, roomTexture, textureRect, tileID));
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
        for (auto& [asset_name, asset_data] : mapData["assets"].items())
        {
            std::string name = asset_name;
            if (asset_data.contains("states"))
            {
                for (auto& state : asset_data["states"]) {
                    if (conditionEvaluator.evaluate(state["condition"]))
                    {
                        sf::Vector2f pos;
                        sf::Vector2i size;
                        sf::Vector2i texturePos;
                        if (state.contains("pos")) {
                            pos.x = state["pos"][0].get<float>();
                            pos.y = state["pos"][1].get<float>();
                        }
                        if (state.contains("size")) {
                            size.x = state["size"][0].get<int>();
                            size.y = state["size"][1].get<int>();
                        }
                        if (state.contains("spritepos")) {
                            texturePos.x = state["spritepos"][0].get<int>();
                            texturePos.y = state["spritepos"][1].get<int>();
                        }
                        sf::Vector2f fsize = sf::Vector2f({(float)size.x, (float)size.y});
                        assets.push_back(Asset(pos, assetsTexture, sf::IntRect(texturePos, size), name, fsize));

                        if (state["condition"] == "room_3_gauntlet_cleared_but_chest_not_fallen"
                            || state["condition"] == "room_5_gauntlet_cleared_but_chest_not_fallen"
                            || state["condition"] == "room_6_gauntlet_cleared_but_chest_not_fallen") {
                            chestFalls = true;
                        }
                    }
                }
            }
            else
            {
                for (auto& [asset_id, asset_d] : asset_data.items())
                {
                    if (asset_d.contains("condition"))
                    {
                        if (conditionEvaluator.evaluate(asset_d["condition"]))
                        {
                            sf::Vector2f pos;
                            sf::Vector2i size;
                            sf::Vector2i texturePos;
                            if (asset_d.contains("pos")) {
                                pos.x = asset_d["pos"][0].get<float>();
                                pos.y = asset_d["pos"][1].get<float>();
                            }
                            if (asset_d.contains("size")) {
                                size.x = asset_d["size"][0].get<int>();
                                size.y = asset_d["size"][1].get<int>();
                            }
                            if (asset_d.contains("spritepos")) {
                                texturePos.x = asset_d["spritepos"][0].get<int>();
                                texturePos.y = asset_d["spritepos"][1].get<int>();
                            }
                            sf::Vector2f fsize = sf::Vector2f({(float)size.x, (float)size.y});
                            assets.push_back(Asset(pos, spikesTexture, sf::IntRect(texturePos, size), name, fsize));
                        }
                    }
                }
            }
        }
    }

    if (mapData.contains("enemies"))
    {
        int id = 0;
        for (auto& [enemy_type, enemy_list] : mapData["enemies"].items())
        {
            std::string name = enemy_type;
            for (auto& [enemy_id, enemy_data] : enemy_list.items())
            {
                if (enemy_data.contains("condition"))
                {
                    if (conditionEvaluator.evaluate(enemy_data["condition"]))
                    {
                        sf::Vector2f pos;
                        dir enemyDirection;
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
                            enemies.push_back(std::make_unique<BlueSlime>(pos, blueSlimeTexture, enemyDirection, id));
                        else
                            enemies.push_back(std::make_unique<RedSlime>(pos, redSlimeTexture, enemyDirection, id));
                        id++;
                    }
                }
                else
                {
                    sf::Vector2f pos;
                    dir enemyDirection;
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
                        enemies.push_back(std::make_unique<BlueSlime>(pos, blueSlimeTexture, enemyDirection, id));
                    else
                        enemies.push_back(std::make_unique<RedSlime>(pos, redSlimeTexture, enemyDirection, id));
                    id++;
                }
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
                enemy->hurtbox.position = {enemy->pos.x - 7.f + enemy->hurtbox_offset.x, enemy->pos.y - 4.f + enemy->hurtbox_offset.y};
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
                enemy->hurtbox.position = {enemy->pos.x - 7.f + enemy->hurtbox_offset.x, enemy->pos.y - 4.f + enemy->hurtbox_offset.y};
            }
        }
        if (name == "Risorse/maps-09/room0.json"
            || name == "Risorse/maps-09/room1.json"
            || name == "Risorse/maps-09/room2.json"
            || name == "Risorse/maps-09/room4.json")
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
                    enemy->hurtbox.position = {enemy->pos.x - 7.f + enemy->hurtbox_offset.x, enemy->pos.y - 4.f + enemy->hurtbox_offset.y};
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
                enemy->hurtbox.position = {enemy->pos.x - 7.f + enemy->hurtbox_offset.x, enemy->pos.y - 4.f + enemy->hurtbox_offset.y};
            }
        }
    }
}

void Room::enemyDeathCleanUp()
{
    for (int i = 0; i < enemies.size(); )
    {
        if (enemies[i]->isDead && enemies[i]->deathFinished)
        {  
            std::swap(enemies[i], enemies.back());
            enemies.pop_back();
        }
        else
            i++;
    }
}

void Room::spikesAnimation()
{
    if (start_animations)
    {
        std::vector<Asset*> spikesVec;
        for (auto& asset : assets)
        {
            if (asset.name == "spikes")
            {
                spikesVec.push_back(&asset);
            }
        }
        if (!clock_active)
        {
            animation_clock.restart();
            clock_active = true;
        }
        if (animation_clock.getElapsedTime().asSeconds() >= spawnFrameTime)
        {
            bool animationEnds = false;
            for (int i = 0; i < spikesVec.size(); i++)
            {
                spikesVec[i]->sprite.setTextureRect(sf::IntRect({animation_frames[i]*32, 0}, {32, 32}));
                animation_frames[i]++;
                if (animation_frames[i] >= 3)
                {
                    if (i == 1)
                        animationEnds = true;
                    animation_frames[i] = 0;
                }
            }

            if (animationEnds)
            {
                start_animations = false;
                clock_active = false;
            }
            else
            {
                animation_clock.restart();
            }
        }
    }
}

void Room::spikesDisappearingAnimation()
{
    if (spikesDisappearing)
    {
        std::vector<Asset*> spikesVec;
        for (auto& asset : assets)
        {
            if (asset.name == "spikes")
            {
                spikesVec.push_back(&asset);
            }
        }
        if (!clock_active)
        {
            animation_clock.restart();
            clock_active = true;
        }
        if (animation_clock.getElapsedTime().asSeconds() >= spawnFrameTime)
        {
            for (int i = 0; i < spikesVec.size(); i++)
            {
                spikesVec[i]->sprite.setTextureRect(sf::IntRect({animation_frames[i]*32, 0}, {32, 32}));
                animation_frames[i]--;
                if (animation_frames[i] < 0 && i == 1)
                        animationEnded = true;
            }

            if (animationEnded)
            {
                spikesDisappearing = false;
                clock_active = false;
            }
            else
            {
                animation_clock.restart();
            }
        }
    }
}

int Room::chestFalling(float elapsed)
{
    if (chestFalls)
    {
        for (auto& asset : assets)
        {
            if (asset.name == "chest")
            {
                if (asset.pos.y >= 80.f)
                {
                    chestFalls = false;
                    if (name == "Risorse/maps-09/room3.json") return 0;
                    else if (name == "Risorse/maps-09/room5.json") return 1;
                    else if (name == "Risorse/maps-09/room6.json") return 2;
                }
                asset.pos.y += chest_falling_speed * elapsed;
            }
        }
    }
    return -1;
}