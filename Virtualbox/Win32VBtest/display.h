#pragma once
#include "virtual_box.h"
#include "com_wrapper.hpp"

namespace vb
{
  class display
  {
  public:
    display();
    ~display() = default;

  private:
    unsigned long width = 0;
    unsigned long height = 0; 
    unsigned long bits_per_pixel = 0;
    long xOrigin = 0;
    long yOrigin = 0;
    GuestMonitorStatus status = GuestMonitorStatus::GuestMonitorStatus_Disabled;
    vb::wrapper::com<IDisplay> vb_display = nullptr;
  };
};

