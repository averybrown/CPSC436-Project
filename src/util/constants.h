//
// Created by Prayansh Srivastava on 2019-01-30.
//

#ifndef PANDAEXPRESS_CONSTANTS_H
#define PANDAEXPRESS_CONSTANTS_H


#include "project_path.hpp"
#define data_path PROJECT_SOURCE_DIR "data"
#define shaders_path(name) data_path "/shaders/" name
#define textures_path(name) data_path "/textures/" name
#define audio_path(name) data_path "/audio/" name
#define levels_path(name) data_path "/levels/" name
#define mesh_path(name) data_path "/meshes/" name
#define fonts_path(name) data_path "/fonts/" name

typedef int SceneID;
typedef int SFXID;
typedef int SceneType;

static const SFXID SFX_JUMP = 0;
static const SFXID SFX_JACKO_LAUGH = 1;
static const SFXID SFX_PANDA_HURT = 2;

static const SceneID MAIN_MENU_SCENE_ID   = 0;
static const SceneID STORY_JUNGLE_SCENE_ID  = 1;
static const SceneID STORY_SKY_SCENE_ID    = 2;
static const SceneID BOSS_SCENE_ID    = 3;
static const SceneID ENDLESS_JUNGLE_SCENE_ID = 4;
static const SceneID ENDLESS_SKY_SCENE_ID = 5;
static const SceneID STORY_INTRO_SCENE_ID  = 6;

static const SceneType JUNGLE_TYPE = 0;
static const SceneType SKY_TYPE = 1;
static const SceneType BOSS_TYPE = 2;

#endif //PANDAEXPRESS_CONSTANTS_H
