#include "enemies.hpp"
#include "player.hpp"

Enemy::Enemy(sf::Vector2f pos, const sf::Texture& texture, std::string name, Dir enemy_dir, int id) : sprite(texture) {
    sprite = sf::Sprite(texture);
    sprite.setTextureRect(sf::IntRect({0, 32}, {32, 32}));
    float sx = (float)sprite.getTextureRect().size.x;
    float sy = (float)sprite.getTextureRect().size.y;
    sprite.setOrigin({sx / 2.f, sy / 2.f});
    this->pos = pos;
    this->name = name;
    this->id = id;
    health_points = 3;
    animation_clock.start();
    aggro_range = {pos, 100.f};
    is_jumping = false;
    jump_timer = 0.f;
    cooldown_timer = 0.f;
    jump_start = {0.f, 0.f};
    jump_target = {0.f, 0.f};
    this->enemy_dir = enemy_dir;
    hitbox = sf::FloatRect({pos.x - 7.f, pos.y + 4.f}, {13.f, 2.f});
    hurtbox = sf::FloatRect({pos.x - 7.f, pos.y - 4.f}, {13.f, 11.f});
    hurtbox_offset = {0.f, 0.f};
    hurt = false;
    flash_clock.reset();
    is_dead = false;
    death_finished = false;
}

void Enemy::animation(int row, float frame_time) {
    if (is_dead) {
        if (animation_clock.getElapsedTime().asSeconds() >= frame_time) {
            animation_clock.restart();
            sf::IntRect cur_frame = sf::IntRect({death_animation_frame * 32, row * 32}, {32, 32});
            sprite.setTextureRect(cur_frame);
            if (death_animation_frame == 4)
                death_finished = true;
            death_animation_frame = (death_animation_frame + 1) % 5;
        }
    } else if (is_jumping) {
        if (animation_clock.getElapsedTime().asSeconds() >= frame_time) {
            animation_clock.restart();
            sf::IntRect cur_frame = sf::IntRect({jump_animation_frame * 32, row * 32}, {32, 32});
            sprite.setTextureRect(cur_frame);
            if (enemy_dir == LEFT)
                sprite.setScale({-1.f, 1.f});
            else
                sprite.setScale({1.f, 1.f});
            switch (jump_animation_frame) {
                case 1:
                case 2: hurtbox_offset.y -= 2.f; break;
                case 3:
                case 4: hurtbox_offset.y += 2.f; break;
                default: break;
            }
            jump_animation_frame = (jump_animation_frame + 1) % 6;
        }
    } else {
        if (animation_clock.getElapsedTime().asSeconds() >= frame_time) {
            animation_clock.restart();
            sf::IntRect cur_frame = sf::IntRect({animation_frame * 32, row * 32}, {32, 32});
            sprite.setTextureRect(cur_frame);
            if (enemy_dir == LEFT)
                sprite.setScale({-1.f, 1.f});
            else
                sprite.setScale({1.f, 1.f});
            animation_frame = (animation_frame + 1) % 4;
        }
    }
}

void Enemy::draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash) {
    sprite.setPosition(pos);
    if (!hurt) {
        sprite.setColor(sf::Color::White);
        window.draw(sprite);
    } else {
        window.draw(sprite, &flash);
        if (flash_clock.getElapsedTime().asSeconds() >= flash_duration) {
            hurt = false;
            flash_clock.reset();
        }
    }

    if (hitboxes) {
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

BlueSlime::BlueSlime(sf::Vector2f pos, const sf::Texture& texture, Dir enemy_dir, int id)
        : Enemy(pos, texture, "blueSlime", enemy_dir, id) {
}

void BlueSlime::jumpTowardsPlayer(sf::Vector2f player_pos, float elapsed) {
    if (!is_jumping) {
        sf::Vector2f to_player = player_pos - hitbox.getCenter();
        float distance = to_player.length();
        sf::Vector2f direction = to_player / distance;
        if (std::abs(direction.x) >= std::abs(direction.y)) {
            if (direction.x > 0) enemy_dir = RIGHT;
            else enemy_dir = LEFT;
        } else {
            if (direction.y > 0) enemy_dir = DOWN;
            else enemy_dir = UP;
        }
        jump_start = pos;
        jump_target = jump_start + direction * jump_distance;
        is_jumping = true;
        is_starting_to_jump = true;
        jump_animation_frame = 0;
        jump_timer = 0.f;
    } else {
        jump_timer += elapsed;
        float progress = jump_timer / jump_time;
        if (progress >= 1.0f) {
            pos = jump_target;
            aggro_range.center = pos;
            is_jumping = false;
            animation_frame = 0;
            cooldown_timer = 0.f;
        } else {
            sf::Vector2f currentPos = jump_start + progress * (jump_target - jump_start);
            pos = currentPos;
            aggro_range.center = pos;
        }
    }
}

void BlueSlime::enemyLogic(const Player& player, float elapsed) {
    if (!is_jumping)
        cooldown_timer += elapsed;

    if (is_jumping) {
        jumpTowardsPlayer(player.hitbox.getCenter(), elapsed);
    } else if (intersects(aggro_range, player.pos) && cooldown_timer >= cooldown) {
        jumpTowardsPlayer(player.hitbox.getCenter(), elapsed);
    }

    int row = 0;
    if (is_jumping) {
        switch (enemy_dir) {
            case LEFT: row = 4; break;
            case RIGHT: row = 4; break;
            case UP: row = 5; break;
            case DOWN: row = 3; break;
        }
        animation(row, slime_mov_frame_time);
    } else {
        switch (enemy_dir) {
            case LEFT: row = 1; break;
            case RIGHT: row = 1; break;
            case UP: row = 2; break;
            case DOWN: row = 0; break;
        }
        animation(row, idle_frame_time);
    }
}

RedSlime::RedSlime(sf::Vector2f pos, const sf::Texture& texture, Dir enemy_dir, int id)
        : Enemy(pos, texture, "redSlime", enemy_dir, id) {
    fireballTexture = sf::Texture(fireball_path);
    has_shot = false;
}

void RedSlime::enemyLogic(const Player& player, float elapsed) {
    facesPlayer(player);

    if (!is_jumping) {
        cooldown_timer += elapsed;
        if (cooldown_timer >= cooldown) {
            is_jumping = true;
            cooldown_timer = 0.f;
            jump_animation_frame = 0;
            has_shot = false;
        }
    } else {
        jump_timer += elapsed;
        if (jump_animation_frame == 3 && !has_shot) {
            spitFire(player);
            has_shot = true;
        }
        if (jump_timer >= jump_time) {
            is_jumping = false;
            jump_timer = 0.f;
            animation_frame = 0;
        }
    }

    int row = 0;
    if (is_jumping) {
        switch (enemy_dir) {
            case LEFT: row = 4; break;
            case RIGHT: row = 4; break;
            case UP: row = 5; break;
            case DOWN: row = 3; break;
        }
        animation(row, slime_mov_frame_time);
    } else {
        switch (enemy_dir) {
            case LEFT: row = 1; break;
            case RIGHT: row = 1; break;
            case UP: row = 2; break;
            case DOWN: row = 0; break;
        }
        animation(row, idle_frame_time);
    }
}

void RedSlime::facesPlayer(const Player& player) {
    sf::Vector2f to_player = player.pos - pos;
    float distance = to_player.length();
    sf::Vector2f direction = to_player / distance;
    if (std::abs(direction.x) >= std::abs(direction.y)) {
        if (direction.x > 0) enemy_dir = RIGHT;
        else enemy_dir = LEFT;
    } else {
        if (direction.y > 0) enemy_dir = DOWN;
        else enemy_dir = UP;
    }
}

void RedSlime::spitFire(const Player& player) {
    sf::Vector2f direction = player.hitbox.getCenter() - pos;
    float length = direction.length();
    if (length > 0.f)
        direction /= length;
    else
        return;
    fireballs.push_back(Fireball(fireballTexture, sf::IntRect({2, 2}, {8, 8}), pos, direction));
    is_starting_to_spit = true;
}

void RedSlime::deleteFire() {
    for (int i = 0; i < fireballs.size(); ) {
        if (fireballs[i].pos.x > 384.f || fireballs[i].pos.y > 288.f
            || fireballs[i].pos.x < 0.f || fireballs[i].pos.y < 0.f) {
            std::swap(fireballs[i], fireballs.back());
            fireballs.pop_back();
        } else
            i++;
    }
}

void RedSlime::draw(sf::RenderWindow& window, bool hitboxes, sf::Shader& flash) {
    Enemy::draw(window, hitboxes, flash);
    for (auto& fireball : fireballs)
        fireball.draw(window, hitboxes);
}