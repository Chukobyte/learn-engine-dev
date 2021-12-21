#pragma once

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "./re/rendering/color.h"
#include "./re/rendering/texture.h"
#include "./re/math/redmath.h"

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
  public:
    void BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex);
    void Flush(const RenderFlushFunction &renderFlushFunction);

  private:
    std::map<int, ZIndexDrawBatch> drawBatches;
};
