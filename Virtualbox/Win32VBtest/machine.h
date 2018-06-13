#pragma once
#include <string>
#include "VirtualBox.h"
#include "utilities.h"
#include "wrappers.hpp"

namespace vb
{
  class machine
  {
  public:
    machine(vb::wrapper::com<IMachine> vb_machine);
    ~machine() = default;
  private:
    vb::wrapper::com<IMachine> vb_machine;
    vb::wrapper::com<ISession> vb_session;
    vb::wrapper::com<IConsole> vb_console;
    vb::wrapper::com<IDisplay> vb_display;

  public:
    std::string get_name();
    void start();
    void power_down();
  };
}


