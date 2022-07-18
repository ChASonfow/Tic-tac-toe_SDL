# Tic-tac-toe
A simple game, made within 4 days on SDL (which I have never got a deal with before). It's a "one-shot" project, which will never be improved or remade from that point on.

## Features
'3x3' and '5x5' fields. "Squares" always start first.
'PVP' ("Player VS Player"), 'Solo' ("Player VS AI") and 'AUTO' ("AI VS AI") game mods are implemented.

## How to build
`git clone --recursive [link]` to recursively download main project with 2 submodules.
You have to build [SDL2](https://github.com/libsdl-org/SDL) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) libraries manually - this is pretty simple by using MS Visual Studio.

As I prefer [premake](https://premake.github.io) more, than cmake, go [here](https://premake.github.io/download) to download pre-build binary file (no need to install anything). Then just follow this little [instruction](https://premake.github.io/docs/Using-Premake) to launch it and build your solution and project files. After that no problems should occur.

I hope you'll like it.