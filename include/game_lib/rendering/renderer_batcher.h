#ifndef RENDERER_BATCHER_H
#define RENDERER_BATCHER_H

#include <vector>
#include <map>
#include <string>

#include <glm/glm.hpp>

#include "./texture.h"
#include "./font.h"
#include "./color.h"
#include "../math/rectangle.h"

struct SpriteBatchItem {
    Texture *texture2D = nullptr;
    Rectangle sourceRectangle;
    Rectangle destinationRectangle;
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
    std::vector<SpriteBatchItem> spriteDrawBatches;
    std::vector<FontBatchItem> fontDrawBatches;
};

class RendererBatcher {
  private:
    std::map<int, ZIndexDrawBatch> drawBatches;

    void BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex);

    void BatchDrawFont(FontBatchItem fontBatchItem, int zIndex);

    void Flush();
};

#endif //RENDERER_BATCHER_H
