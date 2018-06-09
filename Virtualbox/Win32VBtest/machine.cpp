#include <comutil.h>  
#include "machine.h"
#include "utilities.h"

using namespace vb;

machine::machine(vb::wrapper::unknown<IMachine>& vb_machine_)
  : vb_machine(vb_machine_)
{
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
  progress.create_mem(
    &IMachine::LaunchVMProcess, vb_machine, vb_session,
    wrapper::bstr("gui"), nullptr);

  rc = progress->WaitForCompletion(-1);
  util::throw_if_failed(rc, "Could not wait for completion");

  vb_console.create_mem(&ISession::get_Console, vb_session);
}

void vb::machine::power_down()
{
  if (!vb_session.is_valid())
    return;

  vb::wrapper::unknown<IProgress> progress;
  progress.create_mem(&IConsole::PowerDown, vb_console);

  auto rc = progress->WaitForCompletion(-1);
  util::throw_if_failed(rc, "Could not wait for completion");

  rc = vb_session->UnlockMachine();

  vb_console = nullptr;
  vb_session = nullptr;
}

