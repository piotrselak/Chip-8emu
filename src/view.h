#pragma once
#include <array>
#include <cstdint>

class IView {
  public:
    virtual ~IView() = 0;

    virtual void draw(std::array<std::array<bool, 64>, 32> display) = 0;

    virtual uint8_t get_key() = 0;

    virtual float get_delta_time() = 0;

    virtual bool should_end() = 0;
};

inline IView::~IView() = default;
