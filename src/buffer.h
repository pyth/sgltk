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
	 * @param target The initial target to bind the buffer to
	 */
	Buffer(GLenum target = GL_ARRAY_BUFFER) {
		this->target = target;
		glGenBuffers(1, &buffer);
	}

	~Buffer() {
		glDeleteBuffers(1, &buffer);
	}

	/**
	 * @brief Binds the buffer object to the target it was previously
	 * 	  bound to or GL_ARRAY_BUFFER if the buffer has not been bound
	 * 	  to a target before
	 */
	void bind() {
		glBindBuffer(target, buffer);
	}

	/**
	 * @brief Binds the buffer object to a target
	 * @param target The target to bind the buffer to
	 */
	void bind(GLenum target) {
		this->target = target;
		glBindBuffer(target, buffer);
	}

	/**
	 * @brief Binds the buffer object to an indexed buffer target.
	 * @param target The target to bind the buffer to
	 * @param index The index of the binding point within the array
	 * 		specified by target.
	 * @note If the target of the buffer is not GL_ATOMIC_COUNTER_BUFFER,
	 * 	 GL_TRANSFORM_FEEDBACK_BUFFER, GL_UNIFORM_BUFFER or
	 * 	 GL_SHADER_STORAGE_BUFFER the index is ignored.
	 */
	void bind(GLenum target, unsigned int index) {
		this->target = target;
		switch(target) {
			case GL_ATOMIC_COUNTER_BUFFER:
			case GL_TRANSFORM_FEEDBACK_BUFFER:
			case GL_UNIFORM_BUFFER:
			case GL_SHADER_STORAGE_BUFFER:
				glBindBufferBase(target, index, buffer);
				break;
			default:
				glBindBuffer(target, buffer);
				break;
		}
	}

	/**
	 * @brief Unbinds the buffer object from the target it was bound to
	 * @note Exercise caution when unbinding a buffer! If you bind two buffers
	 * 	 to the same target and then call unbind on the first one, you
	 *
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
	 * @param num_elements Number of elements
	 * @param data The data to load into the buffer
	 * @param usage A hint as to how the buffer will be accessed.
	 *      Valid values are GL_{STREAM,STATIC,DYNAMIC}_{DRAW,READ,COPY}.
	 */
	template <typename T>
	void load(unsigned int num_elements, const T *data, GLenum usage) {
		this->usage = usage;
		bind();
		size = num_elements * sizeof(T);

		glBufferData(target, size, data, usage);
		unbind();
	}

	/**
	 * @brief Writes the contents of the buffer object into the storage
	 * @param offset The offset in bytes into the buffer object
	 * @param size The size of the storage
	 * @param storage The storage to write the data to
	 * @return Returns true on success, false otherwise
	 * @note To ensure that the shader program has finished its operations
	 * 	on the buffer you should call glMemoryBarrier before calling
	 * 	this function.
	 */
	bool store(unsigned int offset, unsigned int size, void *storage) {
		if(offset >= this->size)
			return false;

		if(size < this->size - offset)
			return false;

		if(!storage)
			return false;

		bind(GL_COPY_READ_BUFFER);
		glGetBufferSubData(GL_COPY_READ_BUFFER, offset, size, storage);
		unbind();

		return true;
	 }

	/**
	 * @brief Copies data from another buffer object
	 * @param source The source of the data to copy
	 * @param read_offset The offset into the source buffer
	 * @param write_offset The offset into the destination buffer
	 * @param size The size of the data to copy in bytes
	 * @return Returns true on success, false otherwise
	 */
	bool copy(Buffer& source, unsigned int read_offset,
		  unsigned int write_offset, unsigned int size) {

		if(read_offset >= source.size ||
			read_offset + size >= source.size)
				return false;

		if(write_offset >= this->size ||
			write_offset + size >= this->size)
				return false;

		source.bind(GL_COPY_READ_BUFFER);
		bind(GL_COPY_WRITE_BUFFER);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
						read_offset, write_offset, size);
		source.unbind();
		unbind();

		return true;
	}
	/**
	 * @brief Copies data from another buffer object
	 * @param source The source of the data to copy
	 * @param read_offset The offset into the source buffer
	 * @param write_offset The offset into the destination buffer
	 * @param size The size of the data to copy in bytes
	 * @return Returns true on success, false otherwise
	 */
	bool copy(Buffer *source, unsigned int read_offset,
		  unsigned int write_offset, unsigned int size) {

		if(!source)
			return false;

		if(read_offset >= source->size ||
			read_offset + size >= source->size)
				return false;

		if(write_offset >= this->size ||
			write_offset + size >= this->size)
				return false;

		source->bind(GL_COPY_READ_BUFFER);
		bind(GL_COPY_WRITE_BUFFER);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
						read_offset, write_offset, size);
		source->unbind();
		unbind();

		return true;
	}

	/**
	 * @brief Maps all of a buffer object's data into the client's address space
	 * @param access Indicates whether it will be possible to read from,
	 * 	write to, or both read from and write to the buffer object's
	 * 	mapped data store. Must be GL_READ_ONLY, GL_WRITE_ONLY, or GL_READ_WRITE
	 * @return Returns a pointer to the beginning of the mapped range once
	 * 	all pending operations on that buffer object have completed
	 */
	void *map(GLenum access) {
		bind(GL_ARRAY_BUFFER);
		void *ptr = glMapBuffer(target, access);
		unbind();
		return ptr;
	 }

	/**
	  * @brief Releases the mapping of a buffer object's data store into
	  * 	the client's address space
	  */
	void unmap() {
		bind(GL_ARRAY_BUFFER);
		glUnmapBuffer(target);
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
			size = new_size;
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
			size = new_size;
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
