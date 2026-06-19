#include "enemies.hpp"
#include "player.hpp"

Enemy::Enemy(sf::Vector2f pos, const sf::Texture& texture, std::string name, dir enemyDir, int id) : sprite(texture)
{
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(sf::IntRect({0, 32}, {32, 32}));
    float sx = (float)sprite.getTextureRect().size.x;
    float sy = (float)sprite.getTextureRect().size.y;
    sprite.setOrigin({sx / 2.f, sy / 2.f});
    this->pos = pos;
    this->name = name;
    this->id = id;
    healthPoints = 3;
    animation_clock.start();
    aggro_range = {pos, 100.f};
    isJumping = false;
    jumptimer = 0.f;
    cooldownTimer = 0.f;
    jumpStart = {0.f, 0.f};
    jumpTarget = {0.f, 0.f};
    this->enemyDir = enemyDir;
    hitbox = sf::FloatRect({pos.x - 7.f, pos.y + 4.f}, {13.f, 2.f});
    hurtbox = sf::FloatRect({pos.x - 7.f, pos.y - 4.f}, {13.f, 11.f});
    hurtbox_offset = {0.f, 0.f};
    hurt = false;
    flashClock.reset();
    isDead = false;
    deathFinished = false;
}

BlueSlime::BlueSlime(sf::Vector2f pos, const sf::Texture& texture, dir enemyDir, int id)
        : Enemy(pos, texture, "blueSlime", enemyDir, id) {
}

RedSlime::RedSlime(sf::Vector2f pos, const sf::Texture& texture, dir enemyDir, int id)
        : Enemy(pos, texture, "redSlime", enemyDir, id) {
    fireballTexture = sf::Texture(fireballSprites);
    hasShot = false;
}

void Enemy::draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash)
{
    sprite.setPosition(pos);
    if (!hurt)
    {
        sprite.setColor(sf::Color::White);
        window.draw(sprite);
    }
    else
    {
        window.draw(sprite, &flash);
        if (flashClock.getElapsedTime().asSeconds() >= flash_duration)
        {
            hurt = false;
            flashClock.reset();
        }
    }

    if (hitboxes)
    {
        sf::CircleShape aggro = sf::CircleShape(aggro_range.radius);
        aggro.setOrigin({aggro_range.radius, aggro_range.radius});
        aggro.setPosition(aggro_range.center);
        aggro.setOutlineColor(sf::Color::White);
        aggro.setOutlineThickness(1.f);
        aggro.setFillColor(sf::Color::Transparent);
        window.draw(aggro);

        sf::RectangleShape hb = sf::RectangleShape(hitbox.size);
        hb.setPosition(hitbox.position);
        hb.setOutlineColor(sf::Color::White);
        hb.setOutlineThickness(1.f);
        hb.setFillColor(sf::Color::Transparent);
        window.draw(hb);

        sf::RectangleShape hurtb = sf::RectangleShape(hurtbox.size);
        hurtb.setPosition(hurtbox.position);
        hurtb.setOutlineColor(sf::Color::Red);
        hurtb.setOutlineThickness(1.f);
        hurtb.setFillColor(sf::Color::Transparent);
        window.draw(hurtb);
    }
}

void RedSlime::draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash)
{
    Enemy::draw(window, hitboxes, flash);
    for (auto& fireball : fireballs)
        fireball.draw(window, hitboxes);
}

void Enemy::animation(int row, float frameTime)
{
    if (isDead)
    {
        if (animation_clock.getElapsedTime().asSeconds() >= frameTime)
        {
            animation_clock.restart();
            sf::IntRect curFrame = sf::IntRect({death_animation_frame * 32, row * 32}, {32, 32});
            sprite.setTextureRect(curFrame);
            if (death_animation_frame == 4)
            {
                deathFinished = true;
            }
            death_animation_frame = (death_animation_frame + 1) % 5;
        }
    }
    else if (isJumping)
    {
        if (animation_clock.getElapsedTime().asSeconds() >= frameTime)
        {
            animation_clock.restart();
            sf::IntRect curFrame = sf::IntRect({jump_animation_frame * 32, row * 32}, {32, 32});
            sprite.setTextureRect(curFrame);
            if (enemyDir == LEFT)
                sprite.setScale({-1.f, 1.f});
            else
                sprite.setScale({1.f, 1.f});
            switch (jump_animation_frame)
            {
                case 1:
                case 2: hurtbox_offset.y -= 2.f; break;
                case 3:
                case 4: hurtbox_offset.y += 2.f; break;
                default: break;
            }
            jump_animation_frame = (jump_animation_frame + 1) % 6;
        }
    }
    else
    {
        if (animation_clock.getElapsedTime().asSeconds() >= frameTime)
        {
            animation_clock.restart();
            sf::IntRect curFrame = sf::IntRect({animation_frame * 32, row * 32}, {32, 32});
            sprite.setTextureRect(curFrame);
            if (enemyDir == LEFT)
                sprite.setScale({-1.f, 1.f});
            else
                sprite.setScale({1.f, 1.f});
            animation_frame = (animation_frame + 1) % 4;
        }
    }
}

void BlueSlime::jump_towards_player(sf::Vector2f playerPos, float elapsed)
{
    if (!isJumping)
    {
        sf::Vector2f toPlayer = playerPos - hitbox.getCenter();
        float distance = toPlayer.length();
        sf::Vector2f direction = toPlayer / distance;
        if (std::abs(direction.x) >= std::abs(direction.y)) {
            if (direction.x > 0) enemyDir = RIGHT;
            else enemyDir = LEFT;
        } else {
            if (direction.y > 0) enemyDir = DOWN;
            else enemyDir = UP;
        }
        jumpStart = pos;
        jumpTarget = jumpStart + direction * jumpdist;
        isJumping = true;
        jump_animation_frame = 0;
        jumptimer = 0.f;
    }
    else
    {
        jumptimer += elapsed;
        float progress = jumptimer / jumptime;
        if (progress >= 1.0f)
        {
            pos = jumpTarget;
            aggro_range.center = pos;
            isJumping = false;
            animation_frame = 0;
            cooldownTimer = 0.f;
        }
        else
        {
            sf::Vector2f currentPos = jumpStart + progress * (jumpTarget - jumpStart);
            pos = currentPos;
            aggro_range.center = pos;
        }
    }

}

void BlueSlime::enemy_logic(const Player& player, float elapsed)
{
    if (!isJumping)
        cooldownTimer += elapsed;

    if (isJumping) {
        jump_towards_player(player.hitbox.getCenter(), elapsed);
    }
    else if (intersects(aggro_range, player.pos) && cooldownTimer >= cooldown)
    {
        jump_towards_player(player.hitbox.getCenter(), elapsed);
    }

    int row = 0;
    if (isJumping)
    {
        switch (enemyDir) {
            case LEFT: row = 4; break;
            case RIGHT: row = 4; break;
            case UP: row = 5; break;
            case DOWN: row = 3; break;
        }
        animation(row, slimeMovFrameTime);
    }
    else
    {
        switch (enemyDir) {
            case LEFT: row = 1; break;
            case RIGHT: row = 1; break;
            case UP: row = 2; break;
            case DOWN: row = 0; break;
        }
        animation(row, idleFrameTime);
    }
}

void RedSlime::facesPlayer(const Player& player)
{
    sf::Vector2f toPlayer = player.pos - pos;
    float distance = toPlayer.length();
    sf::Vector2f direction = toPlayer / distance;
    if (std::abs(direction.x) >= std::abs(direction.y)) {
        if (direction.x > 0) enemyDir = RIGHT;
        else enemyDir = LEFT;
    } else {
            if (direction.y > 0) enemyDir = DOWN;
            else enemyDir = UP;
    }
}

void RedSlime::enemy_logic(const Player& player, float elapsed)
{
    facesPlayer(player);

    if (!isJumping) {
        cooldownTimer += elapsed;
        if (cooldownTimer >= cooldown)
        {
            isJumping = true;
            cooldownTimer = 0.f;
            jump_animation_frame = 0;
            hasShot = false;
        }
    }
    else
    {
        jumptimer += elapsed;
        if (jump_animation_frame == 3 && !hasShot)
        {
            spitFire(player);
            hasShot = true;
        }
        if (jumptimer >= jumptime)
        {
            isJumping = false;
            jumptimer = 0.f;
            animation_frame = 0;
        }
    }

    int row = 0;
    if (isJumping)
    {
        switch (enemyDir) {
            case LEFT: row = 4; break;
            case RIGHT: row = 4; break;
            case UP: row = 5; break;
            case DOWN: row = 3; break;
        }
        animation(row, slimeMovFrameTime);
    }
    else
    {
        switch (enemyDir) {
            case LEFT: row = 1; break;
            case RIGHT: row = 1; break;
            case UP: row = 2; break;
            case DOWN: row = 0; break;
        }
        animation(row, idleFrameTime);
    }
}

void RedSlime::spitFire(const Player& player)
{
    sf::Vector2f direction = player.hitbox.getCenter() - pos;
    float length = direction.length();
    if (length > 0.f)
        direction /= length;
    else
        return;
    fireballs.push_back(Fireball(fireballTexture, sf::IntRect({2, 2}, {8, 8}), pos, direction));
}

void RedSlime::deleteFire()
{
    for (int i = 0; i < fireballs.size(); )
    {
        if (fireballs[i].pos.x > 384.f || fireballs[i].pos.y > 288.f
            || fireballs[i].pos.x < 0.f || fireballs[i].pos.y < 0.f)
        {
            std::swap(fireballs[i], fireballs.back());
            fireballs.pop_back();
        }
        else
        {
            i++;
        }
    }
}