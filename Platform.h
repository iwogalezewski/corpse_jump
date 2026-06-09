#pragma once
#include "GameObject.h"

class Platform : public GameObject {
public:
    Platform(float startX, float startY, float width, float height);
    void update(float deltaTime) override;
};