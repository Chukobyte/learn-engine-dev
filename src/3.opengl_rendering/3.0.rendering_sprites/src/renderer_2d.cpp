#include "renderer_2d.h"

void Renderer2D::Initialize() {}


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

void Renderer2D::FlushBatches() {
    const RenderFlushFunction &renderFlushFunction = [this] (const int zIndex, const ZIndexDrawBatch &zIndexDrawBatch) {
        // Render Sprites
        for (const SpriteBatchItem &spriteBatchItem : zIndexDrawBatch.spriteDrawBatches) {
            DrawSprite(spriteBatchItem.texture2D,
                       spriteBatchItem.sourceRectangle,
                       spriteBatchItem.destinationRectangle,
                       spriteBatchItem.rotation,
                       spriteBatchItem.color,
                       spriteBatchItem.flipX,
                       spriteBatchItem.flipY);
        }
    };
    rendererBatcher.Flush(renderFlushFunction);
}

void Renderer2D::DrawSprite(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, float rotation,
                            Color color, bool flipX, bool flipY) {

}
