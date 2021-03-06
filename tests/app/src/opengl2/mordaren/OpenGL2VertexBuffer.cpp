#include "OpenGL2VertexBuffer.hpp"

#include "OpenGL2_util.hpp"

using namespace mordaren;

void OpenGL2VertexBuffer::init(GLsizeiptr size, const GLvoid* data) {
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
	assertOpenGLNoError();
	
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	assertOpenGLNoError();
	
	//TODO: remove this
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	assertOpenGLNoError();
}


OpenGL2VertexBuffer::OpenGL2VertexBuffer(const utki::Buf<kolme::Vec4f> vertices) :
		morda::VertexBuffer(vertices.size()),
		numComponents(4),
		type(GL_FLOAT)
{
	this->init(vertices.sizeInBytes(), &*vertices.begin());
}

OpenGL2VertexBuffer::OpenGL2VertexBuffer(const utki::Buf<kolme::Vec3f> vertices) :
		morda::VertexBuffer(vertices.size()),
		numComponents(3),
		type(GL_FLOAT)
{
	this->init(vertices.sizeInBytes(), &*vertices.begin());
}

OpenGL2VertexBuffer::OpenGL2VertexBuffer(const utki::Buf<kolme::Vec2f> vertices) :
		morda::VertexBuffer(vertices.size()),
		numComponents(2),
		type(GL_FLOAT)
{
	this->init(vertices.sizeInBytes(), &*vertices.begin());
}

OpenGL2VertexBuffer::OpenGL2VertexBuffer(const utki::Buf<float> vertices) :
		morda::VertexBuffer(vertices.size()),
		numComponents(1),
		type(GL_FLOAT)
{
	this->init(vertices.sizeInBytes(), &*vertices.begin());
}
