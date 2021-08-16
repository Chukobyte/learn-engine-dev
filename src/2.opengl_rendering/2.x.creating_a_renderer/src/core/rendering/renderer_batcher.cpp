#include "./renderer_batcher.h"

void RendererBatcher::BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex) {
    if (drawBatches.find(zIndex) == drawBatched.end()) {
        drawBatches.emplace(zIndex, ZIndexDrawBatch{});
    }
    drawBatches[zIndex].spriteDrawBatches.emplace_back(spriteBatchItem);
}

void RendererBatcher::BatchDrawFont(FontBatchItem fontBatchItem, int zIndex) {
    if (drawBatches.find(zIndex) == drawBatched.end()) {
        drawBatches.emplace(zIndex, ZIndexDrawBatch{});
    }
    drawBatches[zIndex].fontDrawBatches.emplace_back(fontBatchItem);
}

void RendererBatcher::Flush() {
    for (const auto &pair : drawBatches2D) {
        ZIndexDrawBatch zIndexDrawBatch = pair.second;
        for (const SpriteDrawBatch &spriteDrawBatch : zIndexDrawBatch.spriteDrawBatches) {}
        for (const FontDrawBatch &fontDrawBatch : zIndexDrawBatch.fontDrawBatches) {}
    }
    drawBatches2D.clear();
}
