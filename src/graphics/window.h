//
// Created by alex on 20/01/19.
//

#pragma once

#include "SDL.h"

#include "render.h"
#include "sprite.h"

// Wrap SDL calls with a window creation/management class
class Window : public RenderTarget {
private:
    SDL_Window* sdl_window_;
    SDL_GLContext gl_context_;
    uint64_t last_time_, recent_time_;
    int width_, height_;
    float delta_time_ = 0;
    int WINDOWED_WIDTH = 800;
    int WINDOWED_HEIGHT = 450;

public:
    Window(): sdl_window_(nullptr), gl_context_() {}

    ~Window();

    // Initialize the window
    // returns false if some part of initialization fails
    // otherwise returns true
    bool initialize(const char* title);

    // destroy the window when finished
    void destroy();

    // clears the window
    void clear();

    // swaps the buffers and displays what's been drawn
    void display();

    // returns the time elapsed between the last 2 display() calls, in seconds
    float delta_time();

    // returns the size of the window
    vec2 size();

    void draw(Renderable* renderable, const mat3& projection) override;

};