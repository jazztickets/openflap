#pragma once

// Includes
#include <types.h>

//     Config
const  int          DEFAULT_SCREEN_WIDTH           = 800;
const  int          DEFAULT_SCREEN_HEIGHT          = 600;
const  int          DEFAULT_VSYNC                  = 1;
const  bool         DEFAULT_FULLSCREEN             = false;
const  bool         DEFAULT_AUDIOENABLED           = true;
const  float        DEFAULT_SOUNDVOLUME            = 1.0f;
const  float        DEFAULT_MUSICVOLUME            = 0.8f;

//     Game
const  std::string  GAME_WINDOWTITLE               = "openflap";
const  float        GAME_FPS                       = 100.0f;
const  u32          GAME_SLEEP_RATE                = (u32)(1000/(GAME_FPS));
const  float        GAME_TIMESTEP                  = 1.0f/GAME_FPS;

const  float        JUMP_POWER                     = -670.0f;
const  float        GRAVITY                        = 1600.0f;
const  float        DIED_WAIT_TIME                 = 0.3f;
const  float        WALL_VELOCITY                  = -210.0f;
const  float        WALL_WIDTH                     = 100.0f;
const  float        SPACING                        = 105.0f;
const  float        SPAWNTIME                      = 1.6f;
const  float        SPAWN_RANGE                    = 145.0f;
