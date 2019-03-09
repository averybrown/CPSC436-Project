//
// Created by Prayansh Srivastava on 2019-02-07.
//

#include "background_transform_system.h"
#include <util/constants.h>

BackgroundTransformSystem::BackgroundTransformSystem(SceneID scene_id) : scene_id(scene_id) {

}

void BackgroundTransformSystem::update(Blackboard &blackboard, entt::DefaultRegistry &registry) {
    auto viewBg = registry.view<Background>();
    for (auto entity: viewBg) {
        //get the background
        auto &background = viewBg.get(entity);
        // move the background
        switch (scene_id) {
            case HORIZONTAL_SCENE_ID: {
                horizontal_background_transform(blackboard, background);
            }
                break;
            case VERTICAL_SCENE_ID: {
                vertical_background_transform(blackboard, background);
            }
                break;
            case BOSS_SCENE_ID: {
                boss_background_transform(blackboard, background);
            }
                break;
        }
    }
}

void BackgroundTransformSystem::horizontal_background_transform(Blackboard &blackboard,
                                                                Background &background) {
    float displacement = background.z_pos() * HORIZONTAL_LAYER_SPEED * blackboard.delta_time;
    Camera camera = blackboard.camera;
    background.set_pos1(background.pos1().x + displacement, background.pos1().y);
    background.set_pos2(background.pos2().x + displacement, background.pos2().y);
    if (background.pos1().x < (camera.position().x - camera.size().x)) {
        background.set_pos1(background.pos1().x + camera.size().x * 2, background.pos1().y);
    }
    if (background.pos2().x < (camera.position().x - camera.size().x)) {
        background.set_pos2(background.pos2().x + camera.size().x * 2, background.pos2().y);
    }
}

void BackgroundTransformSystem::vertical_background_transform(Blackboard &blackboard,
                                                              Background &background) {
    if (background.z_pos() != 0) {
        Camera camera = blackboard.camera;
        float displacement = background.z_pos() * VERTICAL_LAYER_SPEED * blackboard.delta_time;
        background.set_pos1(background.pos1().x + displacement, background.pos1().y);
        background.set_pos2(background.pos2().x + displacement, background.pos2().y);
        if (background.pos1().y > (camera.position().y + camera.size().y)) {
            background.set_pos1(background.pos1().x, background.pos1().y - camera.size().y * 2);
            background.set_z_pos(background.z_pos() * -1);
        }
        if (background.pos2().y > (camera.position().y + camera.size().y)) {
            background.set_pos2(background.pos2().x, background.pos2().y - camera.size().y * 2);
            background.set_z_pos(background.z_pos() * -1);
        }
    }
}

float BackgroundTransformSystem::clamp(float val, float from, float to) {
    if (val > from) {
        return std::min(val, to);
    } else if (val < to) {
        return std::max(val, from);
    } else {
        return val;
    }
}

void BackgroundTransformSystem::boss_background_transform(Blackboard &blackboard,
                                                          Background &background) {
    if (background.z_pos() == 0) { // follow the camera
        // this is the moon
        Camera camera = blackboard.camera;
        float displacement = blackboard.delta_time * 1;
        background.set_pos1(camera.position().x + displacement, camera.position().y);
    } else if (background.z_pos() == 3) { // follow the camera only in x-direction
        // This is the grass
        Camera camera = blackboard.camera;
        float displacement = blackboard.delta_time * 1;
        background.set_pos1(camera.position().x + displacement, background.pos1().y);
    } else {
        // everything else
        float og_pos1 = 0.0;
        float clampValue = blackboard.camera.size().x / 0.5f;
        float displacement = clamp(
                (og_pos1 - blackboard.camera.position().x) * (background.z_pos() / 10.0f),
                -clampValue,
                clampValue
        );
        vec2 pos1 = background.pos1();
        background.set_pos1(blackboard.camera.position().x + displacement, pos1.y);
    }
}
