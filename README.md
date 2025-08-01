# sgltk
A collection of easy to use OpenGL tools.

![library_test screenshot](https://github.com/pyth/screenshots/blob/master/lib_test.png)

![island screenshot](https://github.com/pyth/screenshots/blob/master/island.png)

![island wireframe_screenshot](https://github.com/pyth/screenshots/blob/master/island_wireframe.png)

## About
This project's goal is to speed up and make the development of graphical
applications easier by providing a layer of abstraction from OpenGL.

## Dependencies

**Required**:

* [OpenGL](http://www.opengl.org)
* [glm](https://glm.g-truc.net)
* [GLEW](http://glew.sourceforge.net)
* [SDL2](http://www.libsdl.org/download-2.0.php)
* [SDL2_image](http://www.libsdl.org/projects/SDL_image/)

**Optional**:

* [SDL2_ttf](http://www.libsdl.org/projects/SDL_ttf/)
* [Assimp](http://www.assimp.org/index.php/downloads)

## Building the library
* **Windows**
	1. Generate the makefiles or project files using `cmake`:

	`cmake -G` to get the list of available generators

	`cmake -G "generator_name" -DINCLUDE_PATH="path/to/include" -DLIBRARY_PATH="path/to/lib" .`

	Alternatively you can use a graphical tool like **CMake GUI**

	2. Use `make` or open the project file in the appropriate IDE to build the project


* **Linux**

	1. `mkdir -p build`

	2. `cd build`

	3. `conan create . --build=missing`

## Linking against sgltk
If you are linking against the **static** version of sgltk on a **Windows** system, you will have to define **SGLTK_STATIC**. Otherwise just link against sgltk like you would do it with any other 3rd party library.


## Documentation
The documentation is available [here](http://pyth.github.io/sgltk/doc/html/annotated.html).

## Examples
The examples are available [here](https://github.com/pyth/sgltk_examples).
