
#pragma once

#include <GL/glew.h>
#include <gl/GL.h>

#include <memory>
#include <vector>

#include "../Types.h"
#include "../load/resource.h"

namespace CNGE
{
	class Attribute {
	public:
		u32 perVertex;
		u32 size;
		f32* data;
		GLuint buffer;

		Attribute(u32, u32, f32*);

		/// the vertices in a vao are an attribute
		/// use this function to shortcut the creation
		static Attribute createVertexAttribute(u32, f32*);

		auto genBuffers() -> void;
		auto deleteBuffers() -> void;
	};

	class VAO : public Resource {
	public:
		VAO();

		VAO(
			i32, Attribute&&,
			u32, u32[],
			u32, Attribute[]
		);

		~VAO();

		/// actually draws this vao
		void render();

		void render(i32);

	protected:
		void customGather() {} // no gather
		void customDiscard() {};

		void customProcess();
		void customUnload();

	private:
		GLuint vao;

		Attribute vertexAttrib;

		u32 attribCount;
		Attribute* attribs;

		u32 indexCount;
		u32* indices;

		GLuint ibo;

		i32 drawMode;

		/// internally adds a vertex attribute to this vao
		/// creates and returns the opengl buffer
		void add_attribute(Attribute& attrib, u32 location);
	};

}
