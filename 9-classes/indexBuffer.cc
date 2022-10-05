#include "indexBuffer.hh"
#include "renderer.hh"

IndexBuffer::IndexBuffer(const uint *data, uint count) {
    GLCall(glGenBuffers(1, &m_renderer_id_));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id_));
    GLCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_renderer_id_); }

void IndexBuffer::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id_)); }

void IndexBuffer::unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
