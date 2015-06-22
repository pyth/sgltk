AC_PREREQ(2.61)
AC_INIT([sgltk], [0.2], [[fjodor.schelichow@hotmail.com]])
AM_INIT_AUTOMAKE
LT_INIT

AC_CONFIG_MACRO_DIR([m4])

# name one of your source files here
AC_CONFIG_SRCDIR([src/app.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES(Makefile src/Makefile doc/Makefile)

# Checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([SDL2],		[SDL_Init])
AC_CHECK_LIB([SDL2_ttf],	[TTF_Init])
AC_CHECK_LIB([SDL2_mixer],	[Mix_Init])
AC_CHECK_LIB([SDL2_image],	[IMG_Init])
#AC_CHECK_LIB([GL],		[glShaderSource])
#AC_CHECK_LIB([GLU],		[gluBuild2DMipmaps])
#AC_CHECK_LIB([GLEW],		[glewInit])
#AC_CHECK_LIB([assimp],		[main])

PKG_CHECK_MODULES(GLEW, glew)
AC_SUBST([GLEW_CFLAGS])
AC_SUBST([GLEW_LIBS])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([inttypes.h stdlib.h string.h unistd.h])

AC_OUTPUT
