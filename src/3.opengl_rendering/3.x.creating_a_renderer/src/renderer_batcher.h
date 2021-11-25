#ifndef RENDERER_BATCHER_H
#define RENDERER_BATCHER_H

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "./game_lib/rendering/color.h"
#include "./game_lib/rendering/texture.h"
#include "./game_lib/math/math.h"

struct SpriteBatchItem {
    Texture *texture2D = nullptr;
    Rect2 sourceRectangle;
    Rect2 destinationRectangle;
    float rotation = 0.0f;
    Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    bool flipX = false;
    bool flipY = false;
};

struct ZIndexDrawBatch {
    std::vector<SpriteBatchItem> spriteDrawBatches;
};

using RenderFlushFunction = std::function<void(const int zIndex, const ZIndexDrawBatch &zIndexDrawBatch)>;

class RendererBatcher {
  private:
    std::map<int, ZIndexDrawBatch> drawBatches;

  public:
    void BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex);

    void Flush(const RenderFlushFunction &renderFlushFunction);
};

#endif //RENDERER_BATCHER_H
