#include "state.hpp"

State::State(sf::Shader& flash, sf::Shader& redflash, sf::Font& font)
    : room(room1), gameOverText(font, "GAME OVER", 32), restartText(font, "Press ENTER to restart.", 8)
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
    hitboxes = false;
    this->flash = &flash;
    this->redflash = &redflash;

    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition({((float)window_width / 3.f - gameOverText.getGlobalBounds().size.x) / 2.f, 50.f});

    restartText.setFillColor(sf::Color::White);
    restartText.setPosition({((float)window_width / 3.f - restartText.getGlobalBounds().size.x) / 2.f, 200.f});
}

void State::draw(sf::RenderWindow& window, sf::Shader& flash, sf::Shader& redflash)
{
    room.draw(window, hitboxes, flash);
    player.draw(window, hitboxes, redflash);

    if (hitboxes && 
        (roomname == "Risorse/maps/room1.json"
        || roomname == "Risorse/maps/room2.json"
        || roomname == "Risorse/maps/room4.json"))
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

void State::update(float elapsed)
{
    if (player.dead)
    {
        player.animation(9, idleFrameTime);
        if (player.deathAnimationEnded)
        {
            gameMode = GAME_OVER;
        }
    }
    else
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
    }
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
    {
        player.invincibilityTime();
    }
    room.enemyDeathCleanUp();
    room.enemyCollisions();
    room.enemyWallCollisions();
    room_transition();
}

void State::reset()
{
    gameMode = PLAYING;
    roomname = room1;

    room.unload();
    room.load(roomname);

    move_player_up = false;
    move_player_down = false;
    move_player_left = false;
    move_player_right = false;
    playerAttacks = false;
    playerMoving = false;
    hitboxes = false;
    lastPressed = UP;

    player.reset();
}