#pragma once

#include "System.hpp"

class EulerTranslationSystem final : public System {
public:
    void run(float dt) final;
    EulerTranslationSystem(void);
};
