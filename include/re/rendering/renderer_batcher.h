#ifndef RENDERER_BATCHER_H
#define RENDERER_BATCHER_H

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "./texture.h"
#include "./font.h"
#include "./color.h"
#include "./re/math/math.h"

struct SpriteBatchItem {
    Texture *texture2D = nullptr;
    Rect2 sourceRectangle;
    Rect2 destinationRectangle;
    float rotation = 0.0f;
    Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    bool flipX = false;
    bool flipY = false;
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

using RenderFlushFunction = std::function<void(const int zIndex, const ZIndexDrawBatch &zIndexDrawBatch)>;

class RendererBatcher {
  private:
    std::map<int, ZIndexDrawBatch> drawBatches;

  public:
    void BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex);

    void BatchDrawFont(FontBatchItem fontBatchItem, int zIndex);

    void Flush(const RenderFlushFunction &renderFlushFunction);
};

#endif //RENDERER_BATCHER_H
