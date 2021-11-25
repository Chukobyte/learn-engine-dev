#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "renderer_batcher.h"

class Renderer2D {
  private:
    RendererBatcher rendererBatcher;

    void DrawSprite(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, float rotation,
                    Color color, bool flipX, bool flipY);

  public:
    void Initialize();

    void SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, int zIndex, float rotation = 0.0f, Color color = Color(1.0f, 1.0f, 1.0f), bool flipX = false, bool flipY = false);

    void FlushBatches();
};


#endif //RENDERER_2D_H
