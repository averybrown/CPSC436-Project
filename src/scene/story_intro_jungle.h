//
// Created by Avery Brown on 2019-04-13.
//

#ifndef PANDAEXPRESS_STORY_INTRO_JUNGLE_H
#define PANDAEXPRESS_STORY_INTRO_JUNGLE_H


#include "game_scene.h"
#include <systems/timer_system.h>
#include <systems/fade_overlay_system.h>
#include <systems/physics_system.h>
#include <systems/pause_menu_transform_system.h>
#include <systems/render_system.h>
#include <systems/story_jungle_animation_system.h>
#include "scene.h"
#include "scene_manager.h"
#include "../systems/sprite_transform_system.h"
#include "../util/blackboard.h"
#include "../systems/timer_system.h"

class StoryIntroJungleScene: public GameScene {
private:
    const float PANDA_POS_X = -420.f;
    const float PANDA_POS_Y = 150.f;
    const float KELLY_POS_X = 550.f;
    const float KELLY_POS_Y = -250.f;
    const std::string BEACH_SCENE_END_LABEL = "end_scene";
    const float BEACH_SCENE_END = 27.f;

    bool pause = false;
    bool endScene = false;

    uint32_t background_entity;
    uint32_t panda_entity;
    uint32_t kelly_entity;
    SpriteTransformSystem sprite_transform_system;
    StoryJungleAnimationSystem story_animation_system;
    TimerSystem timer_system;
    FadeOverlaySystem fade_overlay_system;
    PauseMenuTransformSystem pause_menu_transform_system;
    RenderSystem render_system;
    PhysicsSystem physics_system;
    Timer scene_timer;


    void create_background(Blackboard &blackboard);
    void create_panda(Blackboard& blackboard);
    void create_kelly(Blackboard& blackboard);
    void init_scene(Blackboard &blackboard);


public:
    StoryIntroJungleScene(Blackboard &blackboard,
                         SceneManager &scene_manager);

    virtual void update(Blackboard& blackboard) override;
    virtual void render(Blackboard& blackboard) override;
    void reset_scene(Blackboard& blackboard) override;

};

#endif //PANDAEXPRESS_STORY_INTRO_JUNGLE_H