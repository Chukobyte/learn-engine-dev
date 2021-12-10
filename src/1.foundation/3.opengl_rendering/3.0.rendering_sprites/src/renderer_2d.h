#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "renderer_batcher.h"
#include "sprite_renderer.h"

class Renderer2D {
  public:
    Renderer2D() = default;
    ~Renderer2D();
    void Initialize();
    void SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, int zIndex, float rotation = 0.0f, Color color = Color(1.0f, 1.0f, 1.0f), bool flipX = false, bool flipY = false);
    void FlushBatches();

  private:
    RendererBatcher rendererBatcher;
    SpriteRenderer *spriteRenderer = nullptr;
};


#endif //RENDERER_2D_H
