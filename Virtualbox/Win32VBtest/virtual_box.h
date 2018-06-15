#pragma once
#include "VirtualBox.h"
#include <vector>
#include "machine.h"
#include "wrappers.hpp"

namespace vb
{
  class virtual_box
  {
  public:
    virtual_box();
    //~virtual_box() = default;

  public:
    std::vector<vb::wrapper::com<IMachine>> get_machines();
    vb::machine find_machine(const std::string& name_or_id);

  private:
    vb::wrapper::com<IVirtualBoxClient> vb_client;
    vb::wrapper::com<IVirtualBox> vb_virtual_box;
  };
}


