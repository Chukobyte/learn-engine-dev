#ifndef RENDERER_BATCHER_H
#define RENDERER_BATCHER_H

#include <vector>
#include <map>
#include <string>

#include "./texture.h"
#include "./font.h"
#include "./color.h"

struct SpriteBatchItem {
    Texture *texture2D = nullptr;
    Rect2 sourceRectangle;
    Rect2 destinationRectangle;
    float rotation = 0.0f;
    Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
};

struct FontBatchItem {
    Font *font = nullptr;
    std::string text;
    float x = 0.0f;
    float y = 0.0f;
    float scale = 1.0f;
    Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
};

struct ZIndexDrawBatch {
    std::vector<SpriteDrawBatch> spriteDrawBatches;
    std::vector<FontDrawBatch> fontDrawBatches;
};

class RendererBatcher {
  private:
    std::map<int, ZIndexDrawBatch> drawBatches;

    void BatchDrawSprite(SpriteDrawBatch spriteBatchItem, int zIndex);

    void BatchDrawFont(FontBatchItem fontBatchItem, int zIndex);

    void Flush();
};

#endif //RENDERER_BATCHER_H
