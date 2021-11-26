#include "render_context.h"
#include "../utils/logger.h"

RenderContext* RenderContext::GetInstance() {
    static RenderContext *instance = new RenderContext();
    return instance;
}

void RenderContext::InitializeFont() {
    if(FT_Init_FreeType(&freeTypeLibrary)) {
        Logger::GetInstance()->Error("Could not initialize FreeType Library!");
    }
}
