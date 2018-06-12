#pragma once
#include "VirtualBox.h"
#include <list>
#include "machine.h"
#include "wrappers.hpp"

namespace vb
{
  class virtual_box
  {
  public:
    virtual_box();
    ~virtual_box();

  public:
    std::list<vb::wrapper::com<IMachine>> get_machines();

  private:
    vb::wrapper::com<IVirtualBoxClient> vb_client;
    vb::wrapper::com<IVirtualBox> vb_virtual_box;

  public:
    machine find_machine(const std::string& name_or_id);
  };
}


