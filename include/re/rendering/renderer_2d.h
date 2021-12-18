#pragma once

#include "./renderer_batcher.h"
#include "./sprite_renderer.h"
#include "./font_renderer.h"

class Renderer2D {
  public:
    ~Renderer2D();
    static Renderer2D* GetInstance();
    void Initialize();
    void SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, int zIndex, float rotation = 0.0f, Color color = Color(1.0f, 1.0f, 1.0f), bool flipX = false, bool flipY = false);
    void SubmitFontBatchItem(Font *font, const std::string &text, float x, float y, int zIndex, float scale, Color color);
    void FlushBatches();

  private:
    RendererBatcher rendererBatcher;
    SpriteRenderer *spriteRenderer = nullptr;
    FontRenderer *fontRenderer = nullptr;

    Renderer2D() = default;
};
