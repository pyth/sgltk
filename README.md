# sgltk
A collection of easy to use OpenGL tools.
![library_test screenshot](https://public.boxcloud.com/api/2.0/internal_files/229185037677/versions/241935226221/representations/png_paged_2048x2048/content/1.png?access_token=1!a3PcJtLKtAKyHsTz7tbj57ksOopb6VRfLaPbR5NFlJe_5l-HyfqUflQvFjUWc7Z0L0U645ejrvLRGZ4ULEJ7ZyZ-aIw7mB50MVs48mmmpa41xe-dQXsAmEyFIZfIqsk0xUpKXCQ_12gzupCWKEEr3t8lx1VVj_WAwlFJzM7kVf6b_QhgEyxMiYkYnSkOstbUChSLC0Y6ekePBIUzL9zKJeQv7NQDmeomWYyagQb0ZcDvBN5EILu9Q8SWsQ6HKpzdjgxWZACf2kz8L7DSAJLCsd7Csb30qpffagZv7Ytx5P7cXZziQTPA6bB7IH-FEthZVbnQa3-Acnblv2g1RomDZ7NZ5YegNJ5Tw9otw7_BVvJmf0aOIeGp8QnnscsrUUfxo4xREVHAcmcWULER)
![island screenshot](https://public.boxcloud.com/api/2.0/internal_files/229184965357/versions/241935148013/representations/png_paged_2048x2048/content/1.png?access_token=1!AqFJ55wCO3zgJhDeVsX7-lZIzgrw6Cg7qA-9U_B0E-BqIN6QABS-FMJR7XIdhienrCVbKK3gFsSC0sLcmFpIggYISYP_71tt00ehQMEDnGzbtF4aOOjjfbCRNImEiM2rnLL68peDhmxhalywAi8yYvpJdSmF_WSK3cs1s2y42UT5bOnEREU7RXOuiJ0RPZo826bLYg-EyEuady_Pykv__McrgdSjW4x-SE1eaeyxDR_75ucCuAg-8jvtMG9Iv6CpmquyMJKJXX9tAC69VWkahpaO6e_npw5z6OSPYQiWBGwllB_oMeopo-ApOs9Y2cQfd9z8UTZv6fvBKzKXkyNWt9fqmPyZIFCSMRzuRT2UIPI8m5ikXeLo8PbbRuXG7Vz0H2u5NMArp407rE5W)

## About
This project's goal is to speed up and make the development of graphical
applications easier by providing a layer of abstraction from OpenGL.

## Dependencies

**Required**:

* [OpenGL](http://www.opengl.org)
* [GLEW](http://glew.sourceforge.net)
* [SDL2](http://www.libsdl.org/download-2.0.php)
* [SDL2_image](http://www.libsdl.org/projects/SDL_image/)

**Optional**:

* [SDL2_ttf](http://www.libsdl.org/projects/SDL_ttf/)
* [Assimp](http://www.assimp.org/main_downloads.html)

## Building the library
* **Windows**
	1. Generate the makefiles or project files using `cmake`:

	`cmake -G` to get the list of available generators

	`cmake -G "generator_name" -DINCLUDE_PATH="path/to/include" -DLIBRARY_PATH="path/to/lib" .`

	Alternatively you can use a graphical tool like **CMake GUI**

	2. Use `make` or open the project file in the appropriate IDE to build the project


* **Linux**
	1. Generate the makefiles using `cmake`:

	`cmake .`

	Alternatively you can use a graphical tool like **CMake GUI**

	2. Build the project using `make`:

	`make` or `make -j number-of-threads`

	This will build both a shared and a static library. To build only one of these you can use the build targets `sgltk` or `sgltk_static`.

	3. Optionally you can install the library using `make` with appropriate privileges:

	`make install`

## Linking against sgltk
If you are linking against the **static** version of sgltk on a **Windows** system, you will have to define **SGLTK_STATIC**. Otherwise just link against sgltk like you would do it with any other 3rd party library.


## Documentation
The documentation is available [here](http://pyth.github.io/sgltk/doc/html/annotated.html).
