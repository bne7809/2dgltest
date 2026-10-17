# 2dgltest

A 2D action game written from scratch in modern C++ (C++20) with OpenGL.

A controllable, animated knight moves, attacks and fires projectiles, driven by a data-defined animation state machine. The project was built to explore real-time rendering and game logic, from the render pipeline down to the animation and input systems, without a game engine.

## Highlights

- **Animation state machine.** Character animations (idle, run, attack, and more) are defined as data tables of frames, durations and transitions, with per-frame triggers, for example, a sword swing fires a projectile partway through the attack animation.
- **Game logic.** Delta-time movement with acceleration and gravity, an input-command buffer that maps key presses to actions, entity state tracked with bit flags, and a projectile pool.
- **Rendering.** OpenGL 3.3 core profile via GLFW and glad, GLSL vertex and fragment shaders, sprite-sheet rendering with direction flipping and alpha blending.
- **Build discipline.** Compiled with `clang++ -std=c++20`, exceptions and RTTI disabled, with separate debug and release (`-O2 -flto`, dead-strip) configurations.

## Tech

C++20, OpenGL 3.3 (GLFW, glad), GLSL shaders. Built with clang and `make`.

## Build

Developed on macOS. Requires GLFW (e.g. via Homebrew) and the OpenGL framework.

```sh
make            # debug build
make BUILD=release
make run
```

## Controls

Keyboard controls for movement and attacks (see `src/controllers.cpp`).

## Project structure

```
include/   # headers (game, anim_entity, sprite_sheet, shader, clock, controllers, proj_pool, ...)
src/        # game loop, animation, input, rendering, and the glad GL loader
shaders/    # GLSL vertex and fragment shaders
res/        # sprite-sheet assets
Makefile
```

## Note on assets

The sprite art in `res/` is free/third-party artwork used only to demonstrate the rendering and animation code. All code is my own.
