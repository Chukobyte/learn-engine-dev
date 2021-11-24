#include "render_context.h"

RenderContext* RenderContext::GetInstance() {
    static RenderContext *instance = new RenderContext();
    return instance;
}
