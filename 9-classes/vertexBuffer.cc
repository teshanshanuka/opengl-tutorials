#include "vertexBuffer.hh"
#include "renderer.hh"

VertexBuffer::VertexBuffer(const void *data, uint size) {
    GLCall(glGenBuffers(1, &m_renderer_id_));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id_));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_renderer_id_); }

void VertexBuffer::bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id_)); }

void VertexBuffer::unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
