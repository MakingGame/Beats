#ifndef RENDER_SPRITE_FRAME_BATCH_H__INCLUDE
#define RENDER_SPRITE_FRAME_BATCH_H__INCLUDE
#include "RenderObject.h"
#include "CommonTypes.h"

class CSpriteFrame;
class CSpriteFrameBatch : public CRenderObject
{
public:
    CSpriteFrameBatch();
    ~CSpriteFrameBatch();

    virtual void PreRender();

    virtual void Render();

    virtual void PostRender();

    void AddSpriteFrame(CSpriteFrame *frame, const kmMat4 &transform);

    void Clear();
    
private:
    void setupVAO();

private:
    static const size_t MAX_QUADS = 1024;
    GLuint m_texture;
    std::vector<CSpriteFrame *> m_frames;
    CQuadPT m_quads[MAX_QUADS];
    size_t m_count;
    GLuint m_VAO, m_VBO[2];
};

#endif