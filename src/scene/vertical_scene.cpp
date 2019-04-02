//
// Created by cowan on 14/02/19.
//

#include <components/obeys_gravity.h>
#include <components/health.h>
#include <components/interactable.h>
#include <components/causes_damage.h>
#include <components/velocity.h>
#include <components/platform.h>
#include <graphics/background.h>
#include <components/timer.h>
#include <graphics/health_bar.h>
#include <graphics/font.h>
#include <graphics/text.h>
#include <graphics/fade_overlay.h>
#include <components/score.h>
#include <components/layer.h>
#include <components/pause_menu.h>
#include <components/hud_element.h>
#include "vertical_scene.h"
#include "util/constants.h"

VerticalScene::VerticalScene(Blackboard &blackboard, SceneManager &scene_manager) :
        Scene(scene_manager),
        level_system(),
        sprite_transform_system(),
        sprite_render_system(),
        physics_system(),
        player_movement_system(SKY_TYPE),
        player_animation_system(SKY_TYPE),
        panda_dmg_system(),
        background_render_system(),
        falling_platform_system(),
        background_transform_system(SKY_TYPE),
        enemy_system(),
        enemy_animation_system(),
        text_transform_system(),
        text_render_system(),
        score_system(SKY_TYPE),
        pause_menu_transform_system(),
        pause_menu_render_system(),
        cave_render_system(),
        transition_system(SKY_TYPE),
        hud_transform_system(),
        label_system()
{
    init_scene(blackboard);
    gl_has_errors("vertical_scene");
}


void VerticalScene::init_scene(Blackboard &blackboard) {
    blackboard.randNumGenerator.init(0);
    blackboard.camera.set_position(CAMERA_START_X, CAMERA_START_Y);
    blackboard.camera.compose();
    create_background(blackboard);
    create_panda(blackboard);
    create_score_text(blackboard);
    create_fade_overlay(blackboard);
    level_system.init();
}

void VerticalScene::create_panda(Blackboard &blackboard) {
    panda_entity = registry_.create();

    auto texture = blackboard.texture_manager.get_texture("panda_sprites");
    auto shader = blackboard.shader_manager.get_shader("sprite");
    auto mesh = blackboard.mesh_manager.get_mesh("sprite");

    float scaleY = 75.0f / texture.height();
    float scaleX = 75.0f / texture.width();
    registry_.assign<Transform>(panda_entity, PANDA_START_X, PANDA_START_Y, 0., scaleX, scaleY);
    registry_.assign<Sprite>(panda_entity, texture, shader, mesh);
    registry_.assign<Panda>(panda_entity);
    registry_.assign<ObeysGravity>(panda_entity);
    registry_.assign<Health>(panda_entity, 3);
    registry_.assign<Interactable>(panda_entity);
    registry_.assign<CausesDamage>(panda_entity, PANDA_DMG_MASK, 1);
    registry_.assign<Velocity>(panda_entity, 0.f, 0.f);
    registry_.assign<Timer>(panda_entity);
    registry_.assign<Collidable>(panda_entity, texture.width() * scaleX, texture.height() * scaleY);
    registry_.assign<Layer>(panda_entity, PANDA_LAYER);

    auto shaderHealth = blackboard.shader_manager.get_shader("health");
    auto meshHealth = blackboard.mesh_manager.get_mesh("health");
    vec2 size = {HEALTH_BAR_X_SIZE, HEALTH_BAR_Y_SIZE};
    vec2 scale = {0.5, 0.5};
    auto &healthbar = registry_.assign<HealthBar>(panda_entity,
                                                  meshHealth, shaderHealth, size, scale);
    registry_.assign<HudElement>(panda_entity,
                                 vec2{size.x / 2.f * scale.x + HUD_HEALTH_X_OFFSET,
                                      blackboard.camera.size().y - HUD_Y_OFFSET});
}

void VerticalScene::update(Blackboard &blackboard) {
    auto &panda = registry_.get<Panda>(panda_entity);
    auto &fadeOverlay = registry_.get<FadeOverlay>(fade_overlay_entity);
    auto &interactable = registry_.get<Interactable>(panda_entity);
    auto &transform = registry_.get<Transform>(panda_entity);
    auto &panda_collidable = registry_.get<Collidable>(panda_entity);

    if (blackboard.input_manager.key_just_pressed(SDL_SCANCODE_ESCAPE)) {
        if (pause) {
            pause = false;
            registry_.destroy(pause_menu_entity);
        } else {
            pause = true;
            create_pause_menu(blackboard);
        }
    } else if (blackboard.input_manager.key_just_pressed(SDL_SCANCODE_RETURN) && pause) {
        blackboard.camera.set_position(0, 0);
        reset_scene(blackboard);
        registry_.destroy(pause_menu_entity);
        change_scene(MAIN_MENU_SCENE_ID);
        pause = false;
        return;
    }
    vec2 cam_size = blackboard.camera.size();
    vec2 cam_position = blackboard.camera.position();

    if (!pause) {
        if (panda.alive && !panda.dead) {
            if (!blackboard.camera.in_transition){
                blackboard.camera.set_position(cam_position.x,
                                               cam_position.y - CAMERA_SPEED * blackboard.delta_time);
            }
            blackboard.camera.compose();
            player_movement_system.update(blackboard, registry_);
        } else if (!panda.alive && interactable.grounded) {
            fade_overlay_system.update(blackboard, registry_);
        }

        if (transform.y - panda_collidable.height / 2 > cam_position.y + cam_size.y / 2 ||
            panda.dead) {
            reset_scene(blackboard);
        } else if (transform.x + panda_collidable.width / 2 > cam_position.x + cam_size.x / 2) {
            transform.x = cam_position.x + cam_size.x / 2 - panda_collidable.width / 2;
        } else if (transform.x - panda_collidable.width / 2 < cam_position.x - cam_size.x / 2) {
            transform.x = cam_position.x - cam_size.x / 2 + panda_collidable.width / 2;
        }

        background_transform_system.update(blackboard, registry_);
        level_system.update(blackboard, registry_);
        physics_system.update(blackboard, registry_);
        panda_dmg_system.update(blackboard, registry_);
        sprite_transform_system.update(blackboard, registry_);
        health_bar_transform_system.update(blackboard, registry_);
        score_system.update(blackboard, registry_);
        label_system.update(blackboard, registry_);
        text_transform_system.update(blackboard, registry_);
        player_animation_system.update(blackboard, registry_);
        enemy_system.update(blackboard, registry_, SKY_TYPE);
        enemy_animation_system.update(blackboard, registry_);
        timer_system.update(blackboard, registry_);
        falling_platform_system.update(blackboard, registry_);
        transition_system.update(blackboard, registry_);
        hud_transform_system.update(blackboard, registry_); // should run last
    } else {
        pause_menu_transform_system.update(blackboard, registry_);
    }
}

void VerticalScene::render(Blackboard &blackboard) {
    // update the rendering systems
    glClearColor(74.f / 256.f, 105.f / 256.f, 189.f / 256.f,
                 1); // same colour as the top of the background
    glClear(GL_COLOR_BUFFER_BIT);
    background_render_system.update(blackboard, registry_);
    cave_render_system.update(blackboard, registry_);
    sprite_render_system.update(blackboard, registry_);
    health_bar_render_system.update(blackboard, registry_);

    // This if is for hiding the score in STORY mode
    // TODO: Remove this hackiness, especially if we need some other text in story mode
    if (mode_ == ENDLESS)
        text_render_system.update(blackboard, registry_);

    auto &panda = registry_.get<Panda>(panda_entity);
    auto &interactable = registry_.get<Interactable>(panda_entity);
    if (!panda.alive && interactable.grounded) {
        fade_overlay_render_system.update(blackboard, registry_);
    }
    if (pause) {
        pause_menu_render_system.update(blackboard, registry_);
    }
    if (blackboard.camera.transition_ready) {
        fade_overlay_render_system.update(blackboard, registry_);
        go_to_next_scene(blackboard);
    }
}

void VerticalScene::reset_scene(Blackboard &blackboard) {
    registry_.destroy(panda_entity);
    level_system.destroy_entities(registry_);
    for (uint32_t e: bg_entities) {
        registry_.destroy(e);
    }
    bg_entities.clear();
    registry_.destroy(score_entity);
    blackboard.score = 0;
    blackboard.camera.in_transition = false;
    blackboard.camera.transition_ready = false;
    init_scene(blackboard);
}

void VerticalScene::go_to_next_scene(Blackboard &blackboard) {
    level_system.destroy_entities(registry_);
    registry_.destroy(panda_entity);
    for (uint32_t e: bg_entities) {
        registry_.destroy(e);
    }
    bg_entities.clear();
    registry_.destroy(fade_overlay_entity);
    blackboard.camera.in_transition = false;
    blackboard.camera.transition_ready = false;
    change_scene(BOSS_SCENE_ID);
    init_scene(blackboard);
}

void VerticalScene::create_background(Blackboard &blackboard) {
    // This order matters for rendering
    auto tex1 = blackboard.texture_manager.get_texture("clouds1");
    auto tex2 = blackboard.texture_manager.get_texture("clouds2");
    auto tex3 = blackboard.texture_manager.get_texture("horizon");
    // end order
    auto shader = blackboard.shader_manager.get_shader("sprite");
    auto mesh = blackboard.mesh_manager.get_mesh("sprite");

    auto bg_entity1 = registry_.create();
    auto &bg = registry_.assign<Background>(bg_entity1, tex1, shader, mesh, -1);
    bg.set_pos1(200.0f, 0.0f);
    bg.set_pos2(200.0f, -blackboard.camera.size().y);
    bg.set_rotation_rad(0.0f);
    bg.set_scale(blackboard.camera.size().x / tex1.width(),
                 blackboard.camera.size().y / tex1.height());
    bg_entities.push_back(bg_entity1);

    auto bg_entity2 = registry_.create();
    auto &bg2 = registry_.assign<Background>(bg_entity2, tex2, shader, mesh, 1);
    bg2.set_pos1(-200.0f, 0.0f);
    bg2.set_pos2(-200.f, -blackboard.camera.size().y);
    bg2.set_rotation_rad(0.0f);
    bg2.set_scale(blackboard.camera.size().x / tex2.width(),
                  blackboard.camera.size().y / tex2.height());
    bg_entities.push_back(bg_entity2);

    auto bg_entity0 = registry_.create();
    auto &bg0 = registry_.assign<Background>(bg_entity0, tex3, shader, mesh, 0, false);
    bg0.set_pos1(0.0f, 0.0f);
    bg0.set_rotation_rad(0.0f);
    bg0.set_scale(blackboard.camera.size().x / tex3.width(),
                  blackboard.camera.size().y / tex3.height());
    bg_entities.push_back(bg_entity0);
}

void VerticalScene::create_score_text(Blackboard &blackboard) {
    auto shader = blackboard.shader_manager.get_shader("text");
    auto mesh = blackboard.mesh_manager.get_mesh("sprite");

    FontType font = blackboard.fontManager.get_font("titillium_72");

    score_entity = registry_.create();
    std::string textVal = "0";
    auto &text = registry_.assign<Text>(score_entity, shader, mesh, font, textVal);
    text.set_scale(0.8f);
    registry_.assign<Score>(score_entity);
    registry_.assign<HudElement>(score_entity,
                                 vec2{blackboard.camera.size().x - HUD_SCORE_X_OFFSET,
                                      blackboard.camera.size().y - HUD_Y_OFFSET});
}

void VerticalScene::set_mode(SceneMode mode) {
    Scene::set_mode(mode);
    level_system.set_mode(mode);
}

void VerticalScene::create_fade_overlay(Blackboard &blackboard) {
    fade_overlay_entity = registry_.create();
    auto shaderFade = blackboard.shader_manager.get_shader("fade");
    auto meshFade = blackboard.mesh_manager.get_mesh("health");
    float height = blackboard.camera.size().y;
    float width = blackboard.camera.size().x;
    vec2 size = {width, height};
    auto &fade = registry_.assign<FadeOverlay>(fade_overlay_entity, meshFade, shaderFade, size);
}

void VerticalScene::create_pause_menu(Blackboard &blackboard) {
    pause_menu_entity = registry_.create();

    auto texture = blackboard.texture_manager.get_texture("pause_menu");
    auto shader = blackboard.shader_manager.get_shader("sprite");
    auto mesh = blackboard.mesh_manager.get_mesh("sprite");

    registry_.assign<Sprite>(pause_menu_entity, texture, shader, mesh);
    registry_.assign<PauseMenu>(pause_menu_entity);
}

