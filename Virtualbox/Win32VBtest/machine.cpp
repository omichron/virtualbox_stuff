#include <comutil.h>  
#include "machine.h"
#include "utilities.h"

using namespace vb;

machine::machine(IMachine* vb_machine)
  : vb_machine(vb_machine)
{
  vb_machine->AddRef();
}

machine::~machine()
{
}

std::string machine::get_name()
{
  BSTR bstr;
  HRESULT rc = vb_machine.ptr()->get_Name(&bstr);

  return SUCCEEDED(rc) ? wrapper::bstr(bstr).str() : std::string();
}

void machine::start()
{
  if (vb_session.ptr())
    return;

  HRESULT rc = CoCreateInstance(CLSID_Session,    /* the VirtualBox base object */
    NULL,                 /* no aggregation */
    CLSCTX_INPROC_SERVER, /* the object lives in the current process */
    IID_ISession,         /* IID of the interface */
    (void**)&vb_session);
  util::throw_if_failed(rc, "Error creating Session instance");

  vb::wrapper::unknown<IProgress> progress;

  /* Start a VM session using the delivered VBox GUI. */
  rc = vb_machine.ptr()->LaunchVMProcess(vb_session.ptr(), wrapper::bstr("gui").get(), NULL, &progress.ptr());
  util::throw_if_failed(rc, "Could not open remote session");

  rc = progress.ptr()->WaitForCompletion(-1);
}
