#pragma once
#include "GameObject.h"

class Platform : public GameObject {
public:
    Platform(float startX, float startY, float width, float height);

    // Platforma siê nie porusza, ale musimy nadpisaæ funkcjê wirtualn¹
    void update(float deltaTime) override;
};