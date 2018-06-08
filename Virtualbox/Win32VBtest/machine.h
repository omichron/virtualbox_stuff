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
    using i_unknown = IMachine;

    explicit machine(IMachine* vb_machine);
    ~machine();
  private:
    vb::wrapper::unknown<IMachine> vb_machine = nullptr;
    vb::wrapper::unknown<ISession> vb_session = nullptr;
    vb::wrapper::unknown<IConsole> vb_console = nullptr;

  public:
    std::string get_name();
    void start();
    void power_down();
    inline IMachine* get_machine_instance() { return vb_machine.ptr(); }
    inline ISession* get_session_instance() { return vb_session.ptr(); }
  };
}


