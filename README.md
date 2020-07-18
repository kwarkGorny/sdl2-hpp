# sdl2-hpp
header-only C++17 bindings to [SDL2]

[SDL2]: https://wiki.libsdl.org/FrontPage

## Documentation

Todo

## Usage

This library is using some features from C++17 so for it to work you need to set flags ([gcc][gcc-c++17], [clang][clang-c++17], [msvc][msvc-c++17]). 
This library was created as header-only, meaning that there is no build step. You just need to make the `sources` directory visible
to your compiler, and replace your `SDL.h` include by `#include <sdl2/sdl.hpp`. You still need to have SDL2 properly visible and linked.

## Additional SDL2 libraries

### SDL2_image
If you want to use SDL2_image you will need to add SDL2_ENABLE_IMG. Those flags must be defined before any include to the library. Additionaly SDL2_image need to have be properly visible and linked.
### SDL_ttf
Everything you need is in sdl2/ttf sub-folder you just need to make SDL2_ttf properly visible and linked.
### SDL_mixer
Everything you need is in sdl2/mixer sub-folder you just need to make SDL2_mixer properly visible and linked.

## Dependencies

### Mandatory

- [SDL2]
- A C++17 capable compiler

### Optional

- SDL2_image (remember to add SDL2_ENABLE_IMG define)
- SDL2_ttf
- SDL2_mixer
- OpenGL(todo)
- Vulkan(todo)
