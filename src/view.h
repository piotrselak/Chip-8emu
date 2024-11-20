#pragma once
#include <array>

class IView {
public:
    virtual ~IView() = 0;

    virtual void draw(const std::array<std::array<bool, 64>, 32> &display) = 0;

    virtual bool should_end() = 0;
};

IView::~IView() {
}
