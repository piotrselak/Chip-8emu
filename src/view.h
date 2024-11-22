#pragma once
#include <array>

class IView {
public:
    virtual ~IView() = 0;

    virtual void draw(std::array<std::array<bool, 64>, 32> display) = 0;

    virtual bool is_key_pressed(int key) = 0;

    virtual int get_key() = 0;

    virtual bool should_end() = 0;
};

inline IView::~IView() = default;
