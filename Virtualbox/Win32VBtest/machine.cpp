#include <comutil.h>  
#include "machine.h"
#include "utilities.h"
#include "com_wrapper.hpp"

vb::machine::machine(vb::wrapper::com<IMachine> vb_machine_)
  : vb_machine(std::move(vb_machine_))
{
}

std::string vb::machine::get_name()
{
  BSTR bstr;
  HRESULT rc = vb_machine->get_Name(&bstr);

  return SUCCEEDED(rc) ? wrapper::bstr(bstr).str() : std::string();
}

void vb::machine::start()
{
  if (vb_session.is_valid())
    return;

  vb_session = vb::wrapper::create_invoke_CoCreateInstance<ISession>();

  auto progress = vb_machine.create_invoke(&IMachine::LaunchVMProcess, 
    vb_session, wrapper::bstr("gui"), nullptr);

  auto rc = progress->WaitForCompletion(-1);
  util::throw_if_failed(rc, "Could not wait for completion");

  vb_console = vb_session.create_invoke(&ISession::get_Console);
  vb_display = vb_console.create_invoke(&IConsole::get_Display);
  
  unsigned long width, height, bitsPerPixel;
  long xOrigin, yOrigin;
  GuestMonitorStatus status;

  Sleep(10000);

  vb_display->GetScreenResolution(0, &width, &height, &bitsPerPixel, &xOrigin, &yOrigin, &status);

}

void vb::machine::power_down()
{
  if (!vb_session.is_valid())
    return;

  auto progress = vb_console.create_invoke(&IConsole::PowerDown);
  auto rc = progress->WaitForCompletion(-1);

  util::throw_if_failed(rc, "Could not wait for completion");

  rc = vb_session->UnlockMachine();

  vb_console = nullptr;
  vb_session = nullptr;
  vb_display = nullptr;
}

