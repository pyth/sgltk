lib_LTLIBRARIES = libsgltk.la

SGLTK_HEADER_FILES =	core.h \
			app.h \
			mesh.h \
			audio.h \
			timer.h \
			camera.h \
			shader.h \
			particle.h \
			texture.h

libsgltk_la_SOURCES =	core.h core.cpp\
			app.h app.cpp \
			mesh.h mesh.cpp \
			audio.h audio.cpp \
			timer.h timer.cpp \
			camera.h camera.cpp \
			shader.h shader.cpp \
			particle.h particle.cpp \
			texture.h texture.cpp image.cpp

libsgltk_la_newdir = $(includedir)/$(PACKAGE)-$(VERSION)
libsgltk_la_new_HEADERS = $(SGLTK_HEADER_FILES)

libsgltk_la_olddir = $(oldincludedir)/$(PACKAGE)-$(VERSION)
libsgltk_la_old_HEADERS = $(SGLTK_HEADER_FILES)

libsgltk_la_CPPFLAGS = -D PACKAGE=\"$(PACKAGE)\" -D VERSION=\"$(VERSION)\" $(GLEW_FLAGS)
libsgltk_la_LIBADD = $(GLEW_LIBS)
libsgltk_la_LDFLAGS = -module -shared

libsgltk_nv_new_includedir = $(includedir)/$(PACKAGE)
libsgltk_nv_old_includedir = $(oldincludedir)/$(PACKAGE)
install-data-hook:
	@test -L $(libsgltk_nv_new_includedir) && echo "!!! The nonversioned library include dir <$(libsgltk_nv_new_includedir)> is already covered by a link. You'll have to update yourself". || true
	test -L $(libsgltk_nv_new_includedir) || ln -s $(libsgltk_la_newdir) $(libsgltk_nv_new_includedir)

	@test -L $(libsgltk_nv_old_includedir) && echo "!!! The nonversioned library include dir <$(libsgltk_nv_old_includedir)> is already covered by a link. You'll have to update yourself". || true
	test -L $(libsgltk_nv_old_includedir) || ln -s $(libsgltk_la_olddir) $(libsgltk_nv_old_includedir)

	@echo "$(libdir)" > "/etc/ld.so.conf.d/$(PACKAGE).conf"
	@ldconfig
