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
    IVirtualBox *get_virtual_box_instance();
    std::list<machine> get_machines();

  private:
    vb::wrapper::unknown<IVirtualBoxClient> vb_client = nullptr;
    vb::wrapper::unknown<IVirtualBox> vb_virtual_box = nullptr;

  public:
    machine find_machine(const std::string& name_or_id);
  };
}


