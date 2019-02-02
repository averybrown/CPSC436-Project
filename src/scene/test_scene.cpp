//
// Created by alex on 29/01/19.
//

#include <components/panda.h>
#include <components/platform.h>
#include <components/collidable.h>
#include <components/obeysGravity.h>
#include <components/health.h>
#include <components/velocity.h>
#include <components/walkable.h>
#include <components/causesDamage.h>
#include "test_scene.h"

#include "components/transform.h"


TestScene::TestScene(Blackboard& blackboard, SceneManager& scene_manager) :
    Scene(scene_manager),
    sprite_render_system(),
    sprite_transform_system(),
    physics_system(),
    player_movement_system(),
    collision_system()
{
    create_panda(blackboard);
    create_platforms(blackboard);
    gl_has_errors();
}

void TestScene::update(Blackboard& blackboard) {
    // some sample input handling


    // update the systems here
    player_movement_system.update(blackboard, registry_);
    collision_system.update(blackboard, registry_);
    physics_system.update(blackboard, registry_);
    sprite_transform_system.update(blackboard, registry_);
}

void TestScene::render(Blackboard& blackboard) {
    // update the rendering systems
    sprite_render_system.update(blackboard, registry_);

}

void TestScene::create_panda(Blackboard& blackboard) {
    panda_entity = registry_.create();

    auto texture = blackboard.textureManager.get_texture("panda");
    auto shader = blackboard.shader_manager.get_shader("sprite");

    float scale = 0.5;
    registry_.assign<Transform>(panda_entity, 0., -100., 0., scale, scale);
    registry_.assign<Sprite>(panda_entity, texture, shader);
    registry_.assign<Panda>(panda_entity, texture.width() * scale, texture.height() * scale);
    registry_.assign<ObeysGravity>(panda_entity);
    registry_.assign<Health>(panda_entity,1);
    registry_.assign<Walkable>(panda_entity);
    registry_.assign<CausesDamage>(panda_entity, false, true, 1);
    registry_.assign<Velocity>(panda_entity,0.f,0.f);
    registry_.assign<Collidable>(panda_entity, texture.width() * scale, texture.height() * scale);

}

void TestScene::create_platforms(Blackboard& blackboard) {
    auto texture = blackboard.textureManager.get_texture("platform");
    auto texture2 = blackboard.textureManager.get_texture("platform2");
    auto shader = blackboard.shader_manager.get_shader("sprite");
    float scale = 100.f / texture.width();

    platform = registry_.create();
    platform2 = registry_.create();
    platform3 = registry_.create();
    platform4 = registry_.create();
    platform5 = registry_.create();
    platform6 = registry_.create();

    registry_.assign<Transform>(platform, -200., 0., 0., scale, scale);
    registry_.assign<Sprite>(platform, texture, shader);
    registry_.assign<Collidable>(platform, texture.width() * scale, texture.height() * scale);

    registry_.assign<Transform>(platform2, -100., 100., 0., scale, scale);
    registry_.assign<Sprite>(platform2, texture2, shader);
    registry_.assign<Collidable>(platform2, texture.width() * scale, texture.height() * scale);

    registry_.assign<Transform>(platform3, 0., 200., 0., scale, scale);
    registry_.assign<Sprite>(platform3, texture2, shader);
    registry_.assign<Collidable>(platform3, texture.width() * scale, texture.height() * scale);

    registry_.assign<Transform>(platform4, 100., 300., 0., scale, scale);
    registry_.assign<Sprite>(platform4, texture2, shader);
    registry_.assign<Collidable>(platform4, texture.width() * scale, texture.height() * scale);

    platform5 = registry_.create();
    registry_.assign<Transform>(platform5, 200., 400., 0., scale, scale);
    registry_.assign<Sprite>(platform5, texture2, shader);
    registry_.assign<Collidable>(platform5, texture.width() * scale, texture.height() * scale);

}

