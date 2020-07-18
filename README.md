# sdl2-hpp
header-only C++17 bindings to [SDL2]

[SDL2]: https://wiki.libsdl.org/FrontPage

## Documentation

Todo

## Usage

This library was created as header-only, meaning that there is no build step. You need to include the `src` directory into
your project, and you are rady to go but remember that you still need to have SDL2 properly visible and linked.

## Additional SDL2 libraries

### SDL2_image
If you want to use SDL2_image you will need to add SDL2_ENABLE_IMG. Those flags must be defined before any include to the library (note: you need to have SDL2_image included and linked).
### SDL_ttf
Everything you need is in sdl2/ttf sub-folder(note: you need to have SDL2_ttf properly included and linked).
### SDL_mixer
Everything you need is in sdl2/mixer sub-folder(note: you need to have SDL2_mixer properly included and linked).

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
