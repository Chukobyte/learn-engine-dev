#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "renderer_batcher.h"

class Renderer2D {
  private:
    RendererBatcher rendererBatcher;

    void DrawSprite(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, float rotation,
                    Color color, bool flipX, bool flipY);

    void DrawFont(Font *font, const std::string &text, float x, float y, float scale, Color color);

  public:
    void SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, int zIndex, float rotation = 0.0f, Color color = Color(1.0f, 1.0f, 1.0f), bool flipX = false, bool flipY = false);

    void SubmitFontBatchItem(Font *font, const std::string &text, float x, float y, int zIndex, float scale = 1.0f, Color color = Color(1.0f, 1.0f, 1.0f));

    void FlushBatches();
};


#endif //RENDERER_2D_H
