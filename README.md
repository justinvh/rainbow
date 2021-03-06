# Rainbow #

## About ##

Rainbow is an [open-source][mit] game engine being developed for a currently
unknown video game.

## Dependencies ##

* [CMake 2.8][cmake]
* [SDL2 with OpenGL 3.x Support][sdl2]
* [SDL_ttf][sdl_ttf]
* [GLEW][glew]
* [GLM][glm]
* [Lua 5.1][lua]
* C++11 compiler

## Building ##

    $ git clone git://github.com/justinvh/rainbow.git
    $ cd rainbow
    $ mkdir build
    $ cd build
    $ ln -s $PWD/../game $PWD/game
    $ cmake ..
    $ make -j4

### Shaders ###
GLSL 1.50 support is required. SDL2 provides a 3.x core profile for Win32
and Linux, but it does not provide a profile under Mac OSX 10.7+.
This is due to the deprecation and changes between 10.5-10.7; SDL2 is
adamant to maintaining compatibility and therefor a truly cross-platform
solution does not exist. However a [patch][sdl2patch] is available if needed
to run under Mac OSX 10.7+.

### C++11 ###

C++11 support is required to build Rainbow. This includes support for
`lambdas`, `unique_ptr`, `enum class`, and `threads`. Currently tested under:

    Apple clang version 4.0 (tags/Apple/clang-421.0.60) (based on LLVM 3.1svn)
    Target: x86_64-apple-darwin11.4.0
    Thread model: posix


  [mit]: http://opensource.org/licenses/mit-license.php "The MIT License"
  [sdl2]: http://hg.libsdl.org/SDL "SDL2 HEAD"
  [sdl_ttf]: http://hg.libsdl.org/SDL_ttf/summary
  [sdl2patch]: http://pastebin.com/3MTjjGPe
  [glew]: http://glew.sourceforge.net/ "The OpenGL Extension Wrangler"
  [glm]: http://glm.g-truc.net/ "OpenGL Mathematics"
  [cmake]: http://www.cmake.org/
  [lua]: http://www.lua.org/
