#include "state.hpp"

State::State(sf::Shader& flash, sf::Shader& redflash, sf::Font& font)
    : room(room1, flags), gameOverText(font, "GAME OVER", 32), restartText(font, "Press ENTER to restart.", 8), victoryText(font, "YOU WON!", 32), textbox(font)
{
    gameMode = PLAYING;
    roomname = room1;
    move_player_up = false;
    move_player_down = false;
    move_player_left = false;
    move_player_right = false;
    lastPressed = UP;
    playerAttacks = false;
    playerMoving = false;
    playerInteracting = false;
    hitboxes = false;
    this->flash = &flash;
    this->redflash = &redflash;
    interactionIsHappening = false;
    continueDialogue = false;
    checkingChest3 = false;
    checkingChest5 = false;
    checkingChest6 = false;
    spawningSmoke = sf::Texture(spawnSprites);

    for (int i = 0; i < 4; i++)
    {
        sf::Sprite sprite = sf::Sprite(spawningSmoke, sf::IntRect({0, 0}, {32, 32}));
        sprite.setPosition(spritePositions[i]);
        spawningSmokeSprites.push_back(sprite);
    }

    start_animations = false;
    animation_clock.reset();
    clockActive = false;

    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition({((float)window_width / 3.f - gameOverText.getGlobalBounds().size.x) / 2.f, 50.f});

    restartText.setFillColor(sf::Color::White);
    restartText.setPosition({((float)window_width / 3.f - restartText.getGlobalBounds().size.x) / 2.f, 200.f});

    victoryText.setFillColor(sf::Color::White);
    victoryText.setPosition({((float)window_width / 3.f - victoryText.getGlobalBounds().size.x) / 2.f, 50.f});
}

void State::draw(sf::RenderWindow& window, sf::Shader& flash, sf::Shader& redflash)
{
    room.draw(window, hitboxes, flash);
    player.draw(window, hitboxes, redflash);
    textbox.draw(window);

    if (start_animations)
    {
        for (auto& sprite : spawningSmokeSprites)
        {
            window.draw(sprite);
        }
    }

    if (hitboxes && 
        (roomname == "Risorse/maps-09/room1.json"
        || roomname == "Risorse/maps-09/room2.json"
        || roomname == "Risorse/maps-09/room4.json"))
    {
        for (auto& hb : room.door_hitboxes)
        {
            sf::RectangleShape hitbox = sf::RectangleShape(hb.size);
            hitbox.setPosition(hb.position);
            hitbox.setOutlineColor(sf::Color::White);
            hitbox.setOutlineThickness(1.f);
            hitbox.setFillColor(sf::Color::Transparent);
            window.draw(hitbox);
        }
    }
    if (gameMode == GAME_OVER)
    {
        window.draw(gameOverText);
        window.draw(restartText);
    }
    if (gameMode == VICTORY)
    {
        window.draw(victoryText);
        window.draw(restartText);
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
    if (roomname == "Risorse/maps-09/room1.json"
        || roomname == "Risorse/maps-09/room2.json"
        || roomname == "Risorse/maps-09/room4.json")
    {
        for (auto& door_hitbox : room.door_hitboxes)
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

void State::hit()
{
    for (auto& enemy : room.enemies)
    {
        if (enemy->isDead) continue;

        if (!player.isInvincible && !player.dead)
        {
            if (player.hitbox.findIntersection(enemy->hitbox))
            {
                player.healthPoints--;
                player.isHurt = true;
                player.redFlashClock.restart();
                if (player.healthPoints <= 0)
                {
                    player.dead = true;
                }
                player.isInvincible = true;
                player.invincibilityClock.restart();
                player.invincibilityFlashClock.restart();
            }
            if (enemy->name == "redSlime")
            {
                RedSlime* redSlime = static_cast<RedSlime*>(enemy.get());
                for (auto& fireball : redSlime->fireballs)
                {
                    if (player.hitbox.findIntersection(fireball.hitbox))
                    {
                        player.healthPoints--;
                        player.isHurt = true;
                        player.redFlashClock.restart();
                        if (player.healthPoints <= 0)
                        {
                            player.dead = true;
                        }
                        player.isInvincible = true;
                        player.invincibilityClock.restart();
                        player.invincibilityFlashClock.restart();
                    }
                }
            }
        }
        if (player.slashHit && player.slashHitbox.findIntersection(enemy->hurtbox))
        {
            enemy->healthPoints--;
            player.slashHit = false;
            if (enemy->healthPoints <= 0)
            {
                enemy->isDead = true;
                if (enemy->name == "redSlime")
                {
                    RedSlime* redSlime = static_cast<RedSlime*>(enemy.get());
                    for (auto& fireball : redSlime->fireballs)
                    {
                        fireball.isDestroyed = true;
                    }
                }
            }
            enemy->hurt = true;
            enemy->flashClock.restart();

            break;
        }
    }
}

void State::update(float elapsed, sf::View& camera)
{
    chestCheck();
    if (gameMode == VICTORY)
    {
        player.animation(2, idleFrameTime);
    }
    else if (player.dead)
    {
        player.animation(9, idleFrameTime);
        if (player.deathAnimationEnded)
        {
            gameMode = GAME_OVER;
        }
    }
    else if (!playerInteracting)
    {
        if (playerAttacks)
        {
            player.attack(elapsed);
            if (!player.isAttacking)
            {
                playerAttacks = false;
            }
        }
        else
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
                    case UP: row = 2; break;
                    case DOWN: row = 0; break;
                    case LEFT: row = 1; break;
                    case RIGHT: row = 1; break;
                }
                player.animation(row, idleFrameTime);
            }
        }
    }
    else if (continueDialogue)
    {
        textbox.showNextLine();
        // gestione screenshake
        if (textbox.text.getString() == "..." && !shakeDone) 
        {
            screenShakeDuration = 0.3f;
            shakeDone = true;
        }
        else if (textbox.text.getString() != "...") 
        {
            shakeDone = false; 
        }
        continueDialogue = false;
        if (!textbox.isActive)
        {
            playerInteracting = false;
            interactionIsHappening = false;
            if (checkingChest3 && !flags.chest_3_opened) flags.chest_3_opened = true;
            if (checkingChest5 && !flags.chest_5_opened) flags.chest_5_opened = true;
            if (checkingChest6 && !flags.chest_6_opened) flags.chest_6_opened = true;
            if (flags.chest_6_opened) gameMode = VICTORY;
        }
    }
    if (screenShakeDuration > 0.f) 
    {
        screenShakeDuration -= elapsed;

        if (screenShakeDuration <= 0.f) 
        {
            camera.setCenter(center);
            screenShakeDuration = 0.f; 
        } 
        else 
        {
            float offsetX = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * intensity;
            float offsetY = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * intensity;
            camera.setCenter(center + sf::Vector2f(offsetX, offsetY));
        }
    }
    else
        camera.setCenter(center);
    for (auto& enemy : room.enemies)
    {
        if (enemy->isDead)
        {
            enemy->animation(12, idleFrameTime);
            if (enemy->name == "redSlime")
            {
                RedSlime* redSlime = static_cast<RedSlime*>(enemy.get());
                for (auto& fireball : redSlime->fireballs)
                {
                    fireball.animation(idleFrameTime);
                }
            }
            continue;
        }
        enemy->enemy_logic(player, elapsed);
        if (enemy->name == "redSlime")
        {
            RedSlime* redSlime = static_cast<RedSlime*>(enemy.get());
            for (auto& fireball : redSlime->fireballs)
            {
                fireball.goTowardsPlayer(elapsed);
            }
            redSlime->deleteFire();
        }
    }

    hit();
    if (player.isInvincible)
        player.invincibilityTime();
    room.enemyDeathCleanUp();
    room.enemyCollisions();
    room.enemyWallCollisions();
    room_transition();

    trigger_gauntlet3();
    clear_gauntlet3();
    trigger_gauntlet5();
    clear_gauntlet5();
    trigger_gauntlet6();
    clear_gauntlet6();

    spawnAnimation();
    room.spikesAnimation();
    room.spikesDisappearingAnimation();
    if (room.animationEnded)
    {
        room.unload();
        room.load(room.name);
    }
}

void State::reset()
{
    gameMode = PLAYING;
    roomname = room1;

    flags = {false, false, false,
             false, false, false,
             false, false, false};

    room.unload();
    room.load(roomname);

    move_player_up = false;
    move_player_down = false;
    move_player_left = false;
    move_player_right = false;
    playerAttacks = false;
    playerMoving = false;
    playerInteracting = false;
    hitboxes = false;
    lastPressed = UP;
    interactionIsHappening = false;
    continueDialogue = false;
    checkingChest3 = false;
    checkingChest5 = false;
    checkingChest6 = false;
    start_animations = false;
    animation_clock.reset();
    clockActive = false;
    for (int i = 0; i < 4; i++)
    {
        animation_frames[i] = 0;
    }

    player.reset();
}

void State::trigger_gauntlet3()
{
    if (room.name == "Risorse/maps-09/room3.json" && !flags.room_3_gauntlet_triggered)
    {
        if (player.hitbox.position.y + player.hitbox.size.y < 256.f)
        {
            flags.room_3_gauntlet_triggered = true;
            start_animations = true;
            room.unload();
            room.load(room.name);
            room.start_animations = true;
        }
    }
}

void State::trigger_gauntlet5()
{
    if (room.name == "Risorse/maps-09/room5.json" && !flags.room_5_gauntlet_triggered)
    {
        if (player.hitbox.position.y + player.hitbox.size.y < 256.f)
        {
            flags.room_5_gauntlet_triggered = true;
            start_animations = true;
            room.unload();
            room.load(room.name);
            room.start_animations = true;
        }
    }
}

void State::trigger_gauntlet6()
{
    if (room.name == "Risorse/maps-09/room6.json" && !flags.room_6_gauntlet_triggered)
    {
        if (player.hitbox.position.y + player.hitbox.size.y < 256.f)
        {
            flags.room_6_gauntlet_triggered = true;
            start_animations = true;
            room.unload();
            room.load(room.name);
            room.start_animations = true;
        }
    }
}

void State::clear_gauntlet3()
{
    if (room.name == "Risorse/maps-09/room3.json" && flags.room_3_gauntlet_triggered && !flags.room_3_gauntlet_cleared)
    {
        if (room.enemies.empty()){
            flags.room_3_gauntlet_cleared = true;
            room.spikesDisappearing = true;
            for (int i = 0; i < 2; i++)
                room.animation_frames[i] = 2;
        }
    }
}

void State::clear_gauntlet5()
{
    if (room.name == "Risorse/maps-09/room5.json" && flags.room_5_gauntlet_triggered && !flags.room_5_gauntlet_cleared)
    {
        if (room.enemies.empty()){
            flags.room_5_gauntlet_cleared = true;
            room.spikesDisappearing = true;
            for (int i = 0; i < 2; i++)
                room.animation_frames[i] = 2;
        }
    }
}

void State::clear_gauntlet6()
{
    if (room.name == "Risorse/maps-09/room6.json" && flags.room_6_gauntlet_triggered && !flags.room_6_gauntlet_cleared)
    {
        if (room.enemies.empty()){
            flags.room_6_gauntlet_cleared = true;
            room.spikesDisappearing = true;
            for (int i = 0; i < 2; i++)
                room.animation_frames[i] = 2;
        }
    }
}

void State::chestCheck()
{
    if (playerInteracting && !interactionIsHappening)
    {
        for (auto& asset : room.assets)
        {
            if (asset.name == "chest")
            {
                if (player.hitbox.position.x >= asset.pos.x
                    && player.hitbox.position.x + player.hitbox.size.x <= asset.pos.x + asset.size.x
                    && player.hitbox.position.y - (asset.pos.y + asset.size.y) <= 1.f)
                {
                    interactionIsHappening = true;
                    if (room.name == "Risorse/maps-09/room3.json")
                    {
                        if (flags.chest_3_opened) interaction("chest_opened");
                        else interaction("chest_opening3");
                        checkingChest3 = true;
                    }
                    else if (room.name == "Risorse/maps-09/room5.json")
                    {
                        if (flags.chest_5_opened) interaction("chest_opened");
                        else interaction("chest_opening5");
                        checkingChest5 = true;
                    }
                    else if (room.name == "Risorse/maps-09/room6.json")
                    {
                        interaction("chest_opening6");
                        checkingChest6 = true;
                    }
                }
            }
        }
    }
    if (!interactionIsHappening)
        playerInteracting = false;
}

void State::interaction(std::string dialogue)
{
    textbox.isActive = true;
    while (!textbox.dialogue_queue.empty())
        textbox.dialogue_queue.pop();

    for (const auto& line : dialogueManager.dialogueMap[dialogue])
        textbox.dialogue_queue.push(line);
    
    textbox.showNextLine();
}

void State::spawnAnimation()
{
    if (start_animations)
    {
        if (!clockActive)
        {
            animation_clock.restart();
            clockActive = true;
        }
        if (animation_clock.getElapsedTime().asSeconds() >= spawnFrameTime)
        {
            bool animationEnded  = false;
            for (int i = 0; i < spawningSmokeSprites.size(); i++)
            {
                animation_frames[i]++;
                if (animation_frames[i] >= 5)
                {
                    if (i == 3)
                        animationEnded = true;
                    animation_frames[i] = 0;
                }
                spawningSmokeSprites[i].setTextureRect(sf::IntRect({animation_frames[i]*32, 0}, {32, 32}));
            }

            if (animationEnded)
            {
                start_animations = false;
                clockActive = false;
            }
            else
            {
                animation_clock.restart();
            }
        }
    }
}