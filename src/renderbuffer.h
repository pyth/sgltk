#ifndef __RENDERBUFFER_H__
#define __RENDERBUFFER_H__

#include "app.h"

namespace sgltk {

class Renderbuffer {
	GLenum format;
	public:
		unsigned int width;
		unsigned int height;
		GLuint buffer;
		EXPORT Renderbuffer(unsigned int res_x,
				    unsigned int res_y,
				    GLenum format);
		EXPORT ~Renderbuffer();
		EXPORT void bind();
		EXPORT void unbind();
};

}

#endif
