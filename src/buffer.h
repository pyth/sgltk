#ifndef __BUFFER_H_
#define __BUFFER_H_

#include "app.h"

namespace sgltk {

/**
 * @class Buffer
 * @brief Manages buffer objects
 */
class Buffer {
	GLenum usage;
	GLenum target;
	GLuint buffer;
public:
	/**
	 * @brief Size of the buffer in bytes
	 */
	unsigned int size;

	/**
	 * @brief Number of elements contained in the buffer
	 */
	unsigned int num_elements;

	/**
	 * @param target The target to bind the buffer object to
	 */
	Buffer(GLenum target = GL_ARRAY_BUFFER) {
		glGenBuffers(1, &buffer);
		set_target(target);
	}

	~Buffer() {
		glDeleteBuffers(1, &buffer);
	}

	Buffer& operator=(const Buffer& other) {
		if(this != &other) {
			this->usage = other.usage;
			this->target = other.target;
			this->buffer = other.buffer;
			this->size = other.size;
			this->num_elements = other.num_elements;
		}
		return *this;
	}

	/**
	 * @brief Sets the binding target for the buffer
	 * @param target The target to bind the buffer object to
	 */
	void set_target(GLenum target) {
		this->target = target;
	}

	/**
	 * @brief Binds the buffer object to its target
	 * @see set_target
	 */
	void bind() {
		glBindBuffer(target, buffer);
	}

	/**
	 * @brief Binds the buffer object to an indexed buffer target.
	 * @param index The index of the binding point within the array
	 * 	specified by target.
	 * @return Returns false if the target of the buffer is not
	 * 	GL_ATOMIC_COUNTER_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER,
	 * 	GL_UNIFORM_BUFFER or GL_SHADER_STORAGE_BUFFER.
	 * @see set_target
	 */
	bool bind_buffer_base(unsigned int index = 0) {
		if(GL_ATOMIC_COUNTER_BUFFER || GL_TRANSFORM_FEEDBACK_BUFFER ||
				GL_UNIFORM_BUFFER || GL_SHADER_STORAGE_BUFFER) {
			glBindBufferBase(target, index, buffer);
			return true;
		} else {
			return false;
		}
	}

	/**
	 * @brief Unbinds the buffer object
	 */
	void unbind() {
		glBindBuffer(target, 0);
	}

	/**
	 * @brief Loads data into the buffer
	 * @param data The data to load into the buffer
	 * @param usage A hint as to how the buffer will be accessed.
	 *      Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 */
	template <typename T>
	void load(const std::vector<T> &data, GLenum usage) {
		this->usage = usage;
		bind();
		size = data.size() * sizeof(T);
		num_elements = data.size();

		glBufferData(target, size, data.data(), usage);
		unbind();
	}

	/**
	 * @brief Loads data into the buffer
	 * @param number_elements Number of elements
	 * @param data The data to load into the buffer
	 * @param usage A hint as to how the buffer will be accessed.
	 *      Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 */
	template <typename T>
	void load(unsigned int num_elements, const T *data, GLenum usage) {
		this->usage = usage;
		bind();
		unsigned int buffer_size = num_elements * sizeof(T);

		glBufferData(target, buffer_size, data, usage);
		unbind();
	}

	/**
	 * @brief Overwrites all data in a vertex buffer
	 * @param data The data to be loaded into the buffer
	 */
	template <typename T>
	void replace_data(const std::vector<T> &data) {
		bind();

		unsigned int new_size = data.size() * sizeof(T);

		if(size == new_size) {
			//replace the buffer data without reallocation
			glBufferSubData(target, 0, size, data.data());
		} else {
			//replace the buffer data with reallocation
			glBufferData(target, new_size, data.data(), usage);
		}
		unbind();
	}

	/**
	 * @brief Overwrites all data in the buffer
	 * @param data The data to be loaded into the buffer
	 * @param number_elements Number of elements
	 */
	template <typename T>
	void replace_data(const T *data, unsigned int number_elements) {
		bind();

		unsigned int new_size = number_elements * sizeof(T);

		if(size == new_size) {
			//replace the buffer data without reallocation
			glBufferSubData(target, 0, size, data);
		} else {
			//replace the buffer data with reallocation
			glBufferData(target, new_size, data, usage);
		}
		unbind();
	}

	/**
	 * @brief Overwrites data in the buffer starting at the specified offset
	 * @param offset The offset from the start of the buffer in bytes
	 * @param data The data to be loaded into the buffer
	 */
	template <typename T>
	bool replace_partial_data(unsigned int offset, const std::vector<T>& data) {

		unsigned int data_size = data.size() * sizeof(T);

		if(offset + data_size > size) {
			return false;
		}

		bind();
		glBufferSubData(target, offset, size, data.data());
		unbind();
		return true;
	}

	/**
	 * @brief Overwrites data in the buffer starting at the specified offset
	 * @param offset The offset from the start of the buffer in bytes
	 * @param data The data to be loaded into the buffer
	 * @param number_elements Number of elements
	 */
	template <typename T>
	bool replace_partial_data(unsigned int offset,
				  const T *data,
				  unsigned int number_elements) {

		unsigned int data_size = data.size() * sizeof(T);

		if(offset + data_size > size) {
			return false;
		}

		bind();
		glBufferSubData(target, offset, size, data.data());
		unbind();
		return true;
	}

};

};

#endif
