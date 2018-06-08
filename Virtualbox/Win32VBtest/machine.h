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
    explicit machine(vb::wrapper::unknown<IMachine>& vb_machine);
    ~machine();
  private:
    vb::wrapper::unknown<IMachine> vb_machine;
    vb::wrapper::unknown<ISession> vb_session;
    vb::wrapper::unknown<IConsole> vb_console;

  public:
    std::string get_name();
    void start();
    void power_down();
  };
}


