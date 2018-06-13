#include "virtual_box.h"

#include <iostream>
#include <windows.h>
#include <exception>
#include "utilities.h"
#include "safearray_wrapper.hpp"

vb::virtual_box::virtual_box()
  : vb_client(vb::wrapper::create_invoke_CoCreateInstance<IVirtualBoxClient>())
  , vb_virtual_box(
      vb_client.create_invoke(&IVirtualBoxClient::get_VirtualBox))
{
}

std::vector<vb::wrapper::com<IMachine>> vb::virtual_box::get_machines()
{
  vb::wrapper::safe_array<vb::wrapper::com<IMachine>> machine_array;

  int rc = vb_virtual_box->get_Machines(machine_array);
  vb::util::throw_if_failed(rc, "Could not enumerate machines");

  return machine_array.as_std_vector();
}

vb::machine vb::virtual_box::find_machine(const std::string& name_or_id)
{
  auto vb_machine =
    vb_virtual_box.create_invoke(
      &IVirtualBox::FindMachine, wrapper::bstr(name_or_id));

  return machine(vb_machine);
}
