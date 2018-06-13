#pragma once
#include "VirtualBox.h"
#include "com_wrapper.hpp"

namespace vb
{
  class display
  {
  public:
    display() = default;
  /*~display() = default;
    display(const display& other) = default;
    display& operator=(const display& other) = default;
    display(display&& other) = default;
    display& operator=(display&& other) = default;*/

    explicit display(vb::wrapper::com<IConsole> console);

    void update();

  private:
    unsigned long width = 0;
    unsigned long height = 0; 
    unsigned long bits_per_pixel = 0;
    long x_origin = 0;
    long y_origin = 0;
    GuestMonitorStatus status = GuestMonitorStatus::GuestMonitorStatus_Disabled;
    vb::wrapper::com<IConsole> vb_console = nullptr;
    vb::wrapper::com<IDisplay> vb_display = nullptr;
  };
};

