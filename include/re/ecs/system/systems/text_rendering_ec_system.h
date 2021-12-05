#ifndef TEXT_RENDERING_EC_SYSTEM_H
#define TEXT_RENDERING_EC_SYSTEM_H

#include "../ec_system.h"

#include "../../component/components/transform2d_component.h"
#include "../../component/components/text_label_component.h"
#include "../../../rendering/renderer_2d.h"

class TextRenderingECSystem : public ECSystem {
  private:
    Renderer2D *renderer2D = nullptr;
    ComponentManager *componentManager = nullptr;

  public:
    TextRenderingECSystem() : renderer2D(Renderer2D::GetInstance()), componentManager(ComponentManager::GetInstance()) {
        enabled = true;
    }

    void Render() {
        if (IsEnabled()) {
            for (Entity entity : entities) {
                Transform2DComponent transform2DComponent = componentManager->GetComponent<Transform2DComponent>(entity);
                TextLabelComponent textLabelComponent = componentManager->GetComponent<TextLabelComponent>(entity);
                renderer2D->SubmitFontBatchItem(
                    textLabelComponent.font,
                    textLabelComponent.text,
                    transform2DComponent.position.x,
                    transform2DComponent.position.y,
                    transform2DComponent.zIndex,
                    transform2DComponent.scale.x,
                    textLabelComponent.color
                );
            }
        }
    }
};

#endif //TEXT_RENDERING_EC_SYSTEM_H
