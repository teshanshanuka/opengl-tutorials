#pragma once

typedef unsigned int uint;

class IndexBuffer {
private:
    uint m_renderer_id_, m_count_;

public:
    IndexBuffer(const uint *data, uint count);

    ~IndexBuffer();

    void bind() const;

    void unbind() const;

    inline uint getCount() { return m_count_; }
};
