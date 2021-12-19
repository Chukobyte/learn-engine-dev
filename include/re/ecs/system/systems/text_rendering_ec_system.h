#pragma once

#include "../ec_system.h"

#include "../../../scene/scene_node_utils.h"
#include "../../component/components/transform2d_component.h"
#include "../../component/components/text_label_component.h"
#include "../../../rendering/renderer_2d.h"

class TextRenderingECSystem : public ECSystem {
  private:
    Renderer2D *renderer2D = nullptr;
    ComponentManager *componentManager = nullptr;

  public:
    TextRenderingECSystem() : renderer2D(Renderer2D::GetInstance()), componentManager(ComponentManager::GetInstance()) {}

    void Render() override {
        if (IsEnabled()) {
            for (Entity entity : entities) {
                Transform2DComponent translatedTransform = SceneNodeUtils::TranslateEntityTransformIntoWorld(entity);
                TextLabelComponent textLabelComponent = componentManager->GetComponent<TextLabelComponent>(entity);
                renderer2D->SubmitFontBatchItem(
                    textLabelComponent.font,
                    textLabelComponent.text,
                    translatedTransform.position.x,
                    translatedTransform.position.y,
                    translatedTransform.zIndex,
                    translatedTransform.scale.x,
                    textLabelComponent.color
                );
            }
        }
    }
};
