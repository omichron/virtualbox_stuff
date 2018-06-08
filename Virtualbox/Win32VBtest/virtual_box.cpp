#include "virtual_box.h"

#include <iostream>
#include <windows.h>
#include <exception>
#include "utilities.h"

using namespace vb;

virtual_box::virtual_box()
{
  HRESULT rc = CoCreateInstance(CLSID_VirtualBoxClient, /* the VirtualBoxClient object */
    NULL,                   /* no aggregation */
    CLSCTX_INPROC_SERVER,   /* the object lives in the current process */
    IID_IVirtualBoxClient,  /* IID of the interface */
    (void**)static_cast<IVirtualBoxClient**>(vb_client));
  vb::util::throw_if_failed(rc, "Could not create virtual box client instance");

  rc = vb_client->get_VirtualBox(vb_virtual_box);
  vb::util::throw_if_failed(rc, "Could not create virtual box instance");
}

virtual_box::~virtual_box()
{
}

std::list<machine> virtual_box::get_machines()
{
  vb::wrapper::safe_array<vb::machine> machine_array;

  int rc = vb_virtual_box->get_Machines(machine_array);
  vb::util::throw_if_failed(rc, "Could not enumerate machines");

  return machine_array.as_std_list();
}

machine virtual_box::find_machine(const std::string& name_or_id)
{
  IMachine* vb_machine;
  
  auto rc = vb_virtual_box->FindMachine(wrapper::bstr(name_or_id), &vb_machine);
  util::throw_if_failed(rc, "Failed to find machine");

  return machine(vb_machine);
}
