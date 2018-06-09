#include "virtual_box.h"

#include <iostream>
#include <windows.h>
#include <exception>
#include "utilities.h"

using namespace vb;

virtual_box::virtual_box()
{
  vb_client.create_voidpp(CoCreateInstance, CLSID_VirtualBoxClient,
    nullptr,
    CLSCTX_INPROC_SERVER,
    IID_IVirtualBoxClient);

  vb_virtual_box.create_mem(
    &IVirtualBoxClient::get_VirtualBox, vb_client);
}

virtual_box::~virtual_box()
{
}

std::list<vb::wrapper::unknown<IMachine>> virtual_box::get_machines()
{
  vb::wrapper::safe_array<vb::wrapper::unknown<IMachine>> machine_array;

  int rc = vb_virtual_box->get_Machines(machine_array);
  vb::util::throw_if_failed(rc, "Could not enumerate machines");

  return machine_array.as_std_list();
}

machine virtual_box::find_machine(const std::string& name_or_id)
{
  wrapper::unknown<IMachine> vb_machine;
  vb_machine.create_mem(&IVirtualBox::FindMachine, vb_virtual_box, wrapper::bstr(name_or_id));

  return machine(vb_machine);
}
