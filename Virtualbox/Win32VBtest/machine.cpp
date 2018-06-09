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

  vb_session = vb::wrapper::create_invoke_void<ISession>(
    CoCreateInstance,
    CLSID_Session,
    nullptr,
    CLSCTX_INPROC_SERVER,
    IID_ISession);

  auto progress = vb_machine.create_invoke<IProgress>(&IMachine::LaunchVMProcess, 
    vb_session, wrapper::bstr("gui"), nullptr);

  auto rc = progress->WaitForCompletion(-1);
  util::throw_if_failed(rc, "Could not wait for completion");

  vb_console = vb_session.create_invoke<IConsole>(&ISession::get_Console);
}

void vb::machine::power_down()
{
  if (!vb_session.is_valid())
    return;

  auto progress = vb_console.create_invoke<IProgress>(&IConsole::PowerDown);
  auto rc = progress->WaitForCompletion(-1);

  util::throw_if_failed(rc, "Could not wait for completion");

  rc = vb_session->UnlockMachine();

  vb_console = nullptr;
  vb_session = nullptr;
}

