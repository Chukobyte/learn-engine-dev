#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "./game_lib/rendering/renderer_batcher.h"
#include "./game_lib/rendering/sprite_renderer.h"
#include "./game_lib/rendering/font_renderer.h"

class Renderer2D {
  private:
    RendererBatcher rendererBatcher;
    SpriteRenderer *spriteRenderer = nullptr;
    FontRenderer *fontRenderer = nullptr;

  public:
    Renderer2D() = default;

    ~Renderer2D();

    void Initialize();

    void SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, int zIndex, float rotation = 0.0f, Color color = Color(1.0f, 1.0f, 1.0f), bool flipX = false, bool flipY = false);

    void SubmitFontBatchItem(Font *font, const std::string &text, float x, float y, int zIndex, float scale, Color color);

    void FlushBatches();
};


#endif //RENDERER_2D_H
