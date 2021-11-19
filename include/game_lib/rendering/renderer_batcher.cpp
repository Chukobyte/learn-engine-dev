#include "./renderer_batcher.h"

void RendererBatcher::BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex) {
    if (drawBatches.find(zIndex) == drawBatches.end()) {
        drawBatches.emplace(zIndex, ZIndexDrawBatch{});
    }
    drawBatches[zIndex].spriteDrawBatches.emplace_back(spriteBatchItem);
}

void RendererBatcher::BatchDrawFont(FontBatchItem fontBatchItem, int zIndex) {
    if (drawBatches.find(zIndex) == drawBatches.end()) {
        drawBatches.emplace(zIndex, ZIndexDrawBatch{});
    }
    drawBatches[zIndex].fontDrawBatches.emplace_back(fontBatchItem);
}

void RendererBatcher::Flush() {
    for (const auto &pair : drawBatches) {
        ZIndexDrawBatch zIndexDrawBatch = pair.second;
        for (const SpriteBatchItem &spriteBatchItem : zIndexDrawBatch.spriteDrawBatches) {}
        for (const FontBatchItem &fontBatchItem : zIndexDrawBatch.fontDrawBatches) {}
    }
    drawBatches.clear();
}
