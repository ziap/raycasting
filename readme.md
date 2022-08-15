# Raycasting engine

Old-school raycasting pseudo-3D game engine made with C++. My first ambitious project.

## Requirements

- A C++ compiler that supports C++20. *
- GNU make.
- [SDL2](https://www.libsdl.org/download-2.0.php).

*: The default compiler is `clang` so you need to edit the [makefile](makefile)
if you want to use another compiler.

## Build and run

To compile go to the project directory and run `make build`.

Run the copiled binary `build/main`.

## Feature goals

 - Working raycasting engine.
 - Texture mapping.
 - Load textures and audios during build step.
 - Entites and projectiles.
 - Vertical camera rotation.

## Implementation goals

 - Render everything into an image buffer and display it every frame. No 3D
   renderer or shader.
 - Native, cross-platform, portable.
 - Decouple SDL2 code and other C++ codes so I can easily port it to WASM
   later.
 - Playable framerates (60fps) on low-end hardware including iGPU.
 - WASM support without using SDL2 and Emscripten. (Use canvas image data for
   rendering and DOM events for user input).

## License

This template is licensed under the [AGPL-v3 License](LICENSE).
