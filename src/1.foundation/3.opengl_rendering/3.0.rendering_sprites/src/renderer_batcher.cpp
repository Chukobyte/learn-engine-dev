#include "./renderer_batcher.h"

void RendererBatcher::BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex) {
    if (drawBatches.find(zIndex) == drawBatches.end()) {
        drawBatches.emplace(zIndex, ZIndexDrawBatch{});
    }
    drawBatches[zIndex].spriteDrawBatches.emplace_back(spriteBatchItem);
}

void RendererBatcher::Flush(const RenderFlushFunction &renderFlushFunction) {
    for (const auto &pair : drawBatches) {
        const int zIndex = pair.first;
        const ZIndexDrawBatch &zIndexDrawBatch = pair.second;
        renderFlushFunction(zIndex, zIndexDrawBatch);
    }
    drawBatches.clear();
}
