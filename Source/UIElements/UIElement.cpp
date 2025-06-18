//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIElement.h"

UIElement::UIElement(const Vector2 &pos, const Vector2 &size,
                     const Vector3 &color)
    : mPosition(pos), mSize(size), mColor(color) {}