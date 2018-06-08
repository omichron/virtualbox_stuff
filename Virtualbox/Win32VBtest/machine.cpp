#include <comutil.h>  
#include "machine.h"
#include "utilities.h"

using namespace vb;

machine::machine(IMachine* vb_machine_)
  : vb_machine(vb_machine_)
{
  vb_machine->AddRef();
}

machine::~machine()
{
}

std::string machine::get_name()
{
  BSTR bstr;
  HRESULT rc = vb_machine->get_Name(&bstr);

  return SUCCEEDED(rc) ? wrapper::bstr(bstr).str() : std::string();
}

void machine::start()
{
  if (vb_session.is_valid())
    return;

  HRESULT rc = CoCreateInstance(CLSID_Session,    /* the VirtualBox base object */
    NULL,                 /* no aggregation */
    CLSCTX_INPROC_SERVER, /* the object lives in the current process */
    IID_ISession,         /* IID of the interface */
    (void**)&vb_session);
  util::throw_if_failed(rc, "Error creating Session instance");

  vb::wrapper::unknown<IProgress> progress;

  /* Start a VM session using the delivered VBox GUI. */
  rc = vb_machine->LaunchVMProcess(vb_session, wrapper::bstr("gui"), NULL, progress);
  util::throw_if_failed(rc, "Could not open remote session");

  rc = progress->WaitForCompletion(-1);
  util::throw_if_failed(rc, "Could not wait for completion");

  /* Get console object. */
  vb_session->get_Console(vb_console);
}

void vb::machine::power_down()
{
  if (!vb_session.is_valid())
    return;

  vb::wrapper::unknown<IProgress> progress;

  auto rc = vb_console->PowerDown(progress);
  util::throw_if_failed(rc, "Could not power machine down");

  rc = progress->WaitForCompletion(-1);
  util::throw_if_failed(rc, "Could not wait for completion");

  rc = vb_session->UnlockMachine();

  vb_console = nullptr;
  vb_session = nullptr;
}

