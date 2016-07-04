# sgltk
A collection of easy to use OpenGL tools.

## About
This project's goal is to speed up and make the development of graphical
applications easier by providing a layer of abstraction from OpenGL.

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