# sgltk
A collection of easy to use OpenGL tools.

## About
This project's goal is to speed up and make the development of graphical
applications easier by providing a layer of abstraction from OpenGL.

## Dependencies

**Required**:

* [OpenGL](http://www.opengl.org)
* [GLEW](http://glew.sourceforge.net)
* [Assimp](http://www.assimp.org/main_downloads.html)
* [SDL2](http://www.libsdl.org/download-2.0.php)
* [SDL2_image](http://www.libsdl.org/projects/SDL_image/)

**Optional**:

* [SDL2_ttf](http://www.libsdl.org/projects/SDL_ttf/)

## Building the library
* **Windows**

    Coming soon.

* **Linux**
    1. Generate the makefiles using `cmake`:

        `cmake .` or `cmake path/to/sgltk/source`

    2. Build the project using `make`:

        `make` or `make -j number-of-threads`

        This will build both a shared and a static library. To build only one of these you can use the build targets `sgltk` or `sgltk_static`.

    3. Install the library using `make` with appropriate privileges:

        `make install`

## Documentation
The documentation is available at [here](http://pyth.github.io/sgltk/doc/html/annotated.html).