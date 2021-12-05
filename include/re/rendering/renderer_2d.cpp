#include "renderer_2d.h"

#include <cassert>
#include <glad/glad.h>

Renderer2D::~Renderer2D() {
    if (spriteRenderer != nullptr) {
        delete spriteRenderer;
        spriteRenderer = nullptr;
    }
    if (fontRenderer != nullptr) {
        delete fontRenderer;
        fontRenderer = nullptr;
    }
}

Renderer2D* Renderer2D::GetInstance() {
    static Renderer2D *instance = new Renderer2D();
    return instance;
}

void Renderer2D::Initialize() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spriteRenderer = new SpriteRenderer();
    fontRenderer = new FontRenderer();
}


void Renderer2D::SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle,
                                       int zIndex, float rotation,
                                       Color color, bool flipX, bool flipY) {
    SpriteBatchItem spriteBatchItem = {
        texture2D,
        sourceRectangle,
        destinationRectangle,
        rotation,
        color,
        flipX,
        flipY
    };
    rendererBatcher.BatchDrawSprite(spriteBatchItem, zIndex);
}

void Renderer2D::SubmitFontBatchItem(Font *font, const std::string &text, float x, float y, int zIndex, float scale, Color color) {
    FontBatchItem fontBatchItem = {
        font,
        text,
        x,
        y,
        scale,
        color
    };
    rendererBatcher.BatchDrawFont(fontBatchItem, zIndex);
}

void Renderer2D::FlushBatches() {
    assert(spriteRenderer != nullptr && "SpriteRenderer is NULL, initialize the Renderer2D before using!");

    const RenderFlushFunction &renderFlushFunction = [this] (const int zIndex, const ZIndexDrawBatch &zIndexDrawBatch) {
        // Draw Sprites
        for (const SpriteBatchItem &spriteBatchItem : zIndexDrawBatch.spriteDrawBatches) {
            spriteRenderer->Draw(spriteBatchItem.texture2D,
                                 spriteBatchItem.sourceRectangle,
                                 spriteBatchItem.destinationRectangle,
                                 spriteBatchItem.rotation,
                                 spriteBatchItem.color,
                                 spriteBatchItem.flipX,
                                 spriteBatchItem.flipY);
        }
        // Draw Font
        for (const FontBatchItem &fontBatchItem : zIndexDrawBatch.fontDrawBatches) {
            fontRenderer->Draw(fontBatchItem.font,
                               fontBatchItem.text,
                               fontBatchItem.x,
                               fontBatchItem.y,
                               fontBatchItem.scale,
                               fontBatchItem.color);
        }
    };
    rendererBatcher.Flush(renderFlushFunction);
}
