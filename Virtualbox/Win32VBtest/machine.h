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
    explicit machine(vb::wrapper::com<IMachine>& vb_machine);
    ~machine();
  private:
    vb::wrapper::com<IMachine> vb_machine;
    vb::wrapper::com<ISession> vb_session;
    vb::wrapper::com<IConsole> vb_console;

  public:
    std::string get_name();
    void start();
    void power_down();
  };
}


