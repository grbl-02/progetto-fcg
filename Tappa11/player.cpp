#include "player.hpp"

Player::Player() : sprite(texture) {
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
    direction = UP;
    isAttacking = false;
    slashHitbox = sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
    slashHit = false;
    healthPoints = 10;
    isInvincible = false;
    isHurt = false;
    redFlashClock.reset();
    invincibilityClock.reset();
    dead = false;
    deathAnimationEnded = false;
    invincibilityFlashClock.reset();
    spriteVisible = false;
    isFlashing = false;
    isDying = false;
}

void Player::animation(int row, float frameTime) {
    if (dead) {
        if (animation_clock.getElapsedTime().asSeconds() >= frameTime) {
            animation_clock.restart();
            sf::IntRect curFrame = sf::IntRect({death_animation_frame * 48, row * 48}, {48, 48});
            sprite.setTextureRect(curFrame);
            if (death_animation_frame < 2)
                death_animation_frame ++;
            else
                deathAnimationEnded = true;
        }
    } else if (isAttacking) {
        if (animation_clock.getElapsedTime().asSeconds() >= frameTime) {
            animation_clock.restart();
            sf::IntRect curFrame = sf::IntRect({attack_animation_frame * 48, row * 48}, {48, 48});
            sprite.setTextureRect(curFrame);
            if (direction == LEFT)
                sprite.setScale({-1.f, 1.f});
            else
                sprite.setScale({1.f, 1.f});
            if (attack_animation_frame == 1) slash();
            else if (attack_animation_frame == 2) unslash();
            aaf_no_mod = aaf_no_mod + 1;
            attack_animation_frame = (attack_animation_frame + 1) % 4;
            if (aaf_no_mod > 3) {
                isAttacking = false;
                aaf_no_mod = 0;
            }
        }
    } else if (animation_clock.getElapsedTime().asSeconds() >= frameTime) {
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

void Player::attack(float elapsed) {
    if (!isAttacking) {
        isAttacking = true;
        attack_animation_frame = 0;
        aaf_no_mod = 0;
        animation_clock.restart();
    }
    int row = 0;
    switch (direction) {
        case LEFT: row = 7; break;
        case RIGHT: row = 7; break;
        case UP: row = 8; break;
        case DOWN: row = 6; break;
        default: break;
    }
    animation(row, slash_frame_time);
}

void Player::slash() {
    slashHit = true;
    switch (direction) {
        case LEFT: slashHitbox = sf::FloatRect({pos.x - 18.f, pos.y + 8.f}, {11.f, 13.f}); break;
        case RIGHT: slashHitbox = sf::FloatRect({pos.x + 7.f, pos.y + 8.f}, {11.f, 13.f}); break;
        case UP: slashHitbox = sf::FloatRect({pos.x - 10.f, pos.y -2.f}, {19.f, 11.f}); break;
        case DOWN: slashHitbox = sf::FloatRect({pos.x - 7.f, pos.y + 13.f}, {19.f, 11.f}); break;
        default: break;
    }
}

void Player::unslash() {
    slashHitbox = sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
}

void Player::enterLeftPos() {
    pos.x = 12 * 32 - 5.f;
    hitbox.position = {pos.x - 5.f, pos.y + 15.f};
}

void Player::enterRightPos() {
    pos.x = 5.f;
    hitbox.position = {pos.x - 5.f, pos.y + 15.f};
}

void Player::enterUpPos() {
    pos.y = (float)window_height / 3.f - (float)sprite.getTextureRect().size.y / 2.f + 6.f;
    hitbox.position = {pos.x - 5.f, pos.y + 15.f};
}

void Player::enterDownPos() {
    pos.y = 3 * 32 - 15.f;
    hitbox.position = {pos.x - 5.f, pos.y + 15.f};
    if (hitbox.position.x < 173.f) {
        hitbox.position.x = 173.f;
        pos.x = hitbox.position.x + 5.f;
    }
    if (hitbox.position.x + hitbox.size.x > 211.f) {
        hitbox.position.x = 211.f - hitbox.size.x;
        pos.x = hitbox.position.x + 5.f;
    }
}

void Player::invincibilityTime() {
    if (invincibilityClock.getElapsedTime().asSeconds() >= invincibility) {
        isInvincible = false;
        invincibilityClock.reset();
        invincibilityFlashClock.reset();
        isFlashing = false;
    }
}

void Player::reset(Difficulty difficulty) {
    texture = sf::Texture(player_texture);
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(sf::IntRect({0, 96}, {48, 48}));
    float sx = (float)sprite.getTextureRect().size.x;
    float sy = (float)sprite.getTextureRect().size.y;
    sprite.setOrigin({sx / 2.f, sy / 2.f});
    float px = (float)window_width / (3.f * 2.f);
    float py = (float)window_height / 3.f - sy / 2.f + 6.f;
    pos = {px, py};
    isLeft = false;
    animation_clock.start();
    hitbox = sf::FloatRect({pos.x - 5.f, pos.y + 15.f}, {10.f, 3.f});
    direction = UP;
    isAttacking = false;
    slashHitbox = sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
    slashHit = false;
    if (difficulty == NORMAL)
        healthPoints = 10;
    else
        healthPoints = 1;
    isInvincible = false;
    isHurt = false;
    redFlashClock.reset();
    invincibilityClock.reset();
    dead = false;
    deathAnimationEnded = false;
    invincibilityFlashClock.reset();
    spriteVisible = false;
    isFlashing = false;
    isDying = false;
    animation_frame = 0;
    attack_animation_frame = 0;
    aaf_no_mod = 0;
    death_animation_frame = 0;
}

void Player::draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& redflash) {
    sprite.setPosition(pos);
    if ((!isHurt && !isFlashing) || deathAnimationEnded || isDying) {
        sprite.setColor(sf::Color::White);
        window.draw(sprite);
    } else if (isHurt) {
        window.draw(sprite, &redflash);
        if (redFlashClock.getElapsedTime().asSeconds() >= flash_duration) {
            isHurt = false;
            isFlashing = true;
            redFlashClock.reset();
            if (dead) isDying = true;
        }
    } else if (isFlashing && !dead) {
        if (invincibilityFlashClock.getElapsedTime().asSeconds() >= invincibility_flash_interval) {
            spriteVisible = !spriteVisible;
            invincibilityFlashClock.restart();
        }
        if (spriteVisible) sprite.setColor(sf::Color::White);
        else sprite.setColor(sf::Color(255, 255, 255, 50));
        window.draw(sprite);
    }

    if (hitboxes) {
        sf::RectangleShape hb = sf::RectangleShape(hitbox.size);
        hb.setPosition(hitbox.position);
        hb.setOutlineColor(sf::Color::White);
        hb.setOutlineThickness(1.f);
        hb.setFillColor(sf::Color::Transparent);
        window.draw(hb);

        sf::RectangleShape slashHB = sf::RectangleShape(slashHitbox.size);
        slashHB.setPosition(slashHitbox.position);
        slashHB.setOutlineColor(sf::Color::Red);
        slashHB.setOutlineThickness(1.f);
        slashHB.setFillColor(sf::Color::Transparent);
        window.draw(slashHB);
    }
}