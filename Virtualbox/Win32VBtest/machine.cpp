#include <comutil.h>  
#include "machine.h"
#include "utilities.h"
#include "com_wrapper.hpp"
#include "frame_buffer.h"
#include <chrono>

vb::machine::machine(vb::wrapper::com<IMachine> vb_machine_)
  : vb_machine(std::move(vb_machine_))
  , vb_session(vb::wrapper::create_invoke_CoCreateInstance<ISession>()) //@todo: constructor
{
}

vb::machine::~machine()
{
  if (vb_session.get_invoke(&ISession::get_State) == SessionState::SessionState_Locked)
    vb_session->UnlockMachine();
}

std::string vb::machine::get_name()
{
  BSTR bstr;
  HRESULT rc = vb_machine->get_Name(&bstr);

  return SUCCEEDED(rc) ? wrapper::bstr(bstr).str() : std::string();
}

void vb::machine::start()
{
  auto state = vb_machine.get_invoke(&IMachine::get_State);

  switch (vb_machine.get_invoke(&IMachine::get_State))
  {
  case MachineState::MachineState_PoweredOff:
  case MachineState::MachineState_Aborted:
  {
    auto progress = vb_machine.create_invoke(&IMachine::LaunchVMProcess,
      vb_session, wrapper::bstr("gui"), nullptr);

    auto rc = progress->WaitForCompletion(-1);
    util::throw_if_failed(rc, "Could not wait for completion of LaunchVMProcess");
    break;
  }
  case MachineState::MachineState_Running:
  {
    auto rc = vb_machine->LockMachine(vb_session, LockType::LockType_Shared);
    util::throw_if_failed(rc, "Could not lock runnin machine for session");
    break;
  }
  default:
    throw(std::exception("Unimplemented"));
  }

  vb_console = vb_session.create_invoke(&ISession::get_Console);
  
  IFramebuffer* fb = new frame_buffer();
//  CoCreateInstance(CLSID_ )

  display = vb::display(vb_console);

  auto begin = std::chrono::system_clock::now();
  display.update();
  display.take_screenshot();
  auto end = std::chrono::system_clock::now();

  auto elapsed = end - begin;

  printf("Elapsed time=%u ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count());

  /*IFramebuffer* buffers[10];
  
  BSTR bstr;
  display.vb_display->QueryFramebuffer(0, buffers);
  buffers[0]->Release();*/

 // auto rc = display.vb_display->DetachFramebuffer(0, nullptr);

 // auto rc = display.vb_display->AttachFramebuffer(0, fb, &bstr);

  //display.vb_display->QueryFramebuffer(0, buffers);
 // util::throw_if_failed(rc, "Attach failed");

  //delete fb;
}

void vb::machine::power_down()
{
  auto progress = vb_console.create_invoke(&IConsole::PowerDown);
  
  auto rc = progress->WaitForCompletion(-1);
  util::throw_if_failed(rc, "Could not wait for completion");

  rc = vb_session->UnlockMachine();

  vb_console = nullptr;
}

