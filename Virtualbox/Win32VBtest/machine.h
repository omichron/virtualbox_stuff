#pragma once
#include <string>
#include "VirtualBox.h"
#include "utilities.h"
#include "wrappers.hpp"
#include "display.h"

namespace vb
{
  class machine
  {
  public:
    machine(vb::wrapper::com<IMachine> vb_machine);
    ~machine();
  private:
    vb::wrapper::com<IMachine> vb_machine;
    vb::wrapper::com<ISession> vb_session;
    vb::wrapper::com<IConsole> vb_console;
    vb::display display;

  public:
    std::string get_name();
    void start();
    void power_down();
  };
}


