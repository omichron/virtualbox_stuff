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
    (void**)&vb_client.ptr());
  vb::util::throw_if_failed(rc, "Could not create virtual box client instance");

  rc = vb_client.ptr()->get_VirtualBox(&vb_virtual_box.ptr());
  vb::util::throw_if_failed(rc, "Could not create virtual box instance");
}

virtual_box::~virtual_box()
{
}

IVirtualBox *virtual_box::get_virtual_box_instance()
{
  return vb_virtual_box.ptr();
}

std::list<machine> virtual_box::get_machines()
{
  SAFEARRAY *machinesArray = nullptr;

  int rc = vb_virtual_box.ptr()->get_Machines(&machinesArray);
  vb::util::throw_if_failed(rc, "Could not enumerate machines");

  return vb::util::to_std_list<machine, IMachine>(machinesArray);
}

machine virtual_box::find_machine(const std::string& name_or_id)
{
  IMachine* vb_machine;
  
  auto rc = vb_virtual_box.ptr()->FindMachine(wrapper::bstr(name_or_id).get(), &vb_machine);
  util::throw_if_failed(rc, "Failed to find machine");

  return machine(vb_machine);
}
