#pragma once

typedef unsigned int uint;

class VertexBuffer {
private:
    uint m_renderer_id_;

public:
    VertexBuffer(const void *data, uint size);

    ~VertexBuffer();

    void bind() const;

    void unbind() const;
};
