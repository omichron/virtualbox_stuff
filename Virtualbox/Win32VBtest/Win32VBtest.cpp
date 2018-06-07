// Win32VBtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include "virtual_box.h"
#include "VirtualBox.h"
#include "machine.h"
#include "utilities.h"

#define SAFE_RELEASE(x) \
    if (x) { \
        x->Release(); \
        x = NULL; \
    }

int listVMs(vb::virtual_box& virtual_box)
{
  for(auto& machine : virtual_box.get_machines())
    std::cout << machine.get_name() << std::endl;;
 
  return 0;
}


int testErrorInfo(vb::virtual_box& virtual_box)
{
  try
  {
    virtual_box.find_machine("Foobar");
    std::cout << "Success" << std::endl;
  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
  }

	return 0;
}


int testStartVM(vb::virtual_box& virtual_box)
{
	HRESULT rc;
  auto virtualBox = virtual_box.get_virtual_box_instance();

  try
  {
    auto machine = virtual_box.find_machine("Android");

    IConsole *console = NULL;
    IProgress *progress = NULL;

    auto vm_machine = machine.get_machine_instance();

    do
    {
      /* Wait until VM is running. */
      printf("Starting VM, please wait ...\n");
      
      machine.start();

      /* Get console object. */
      machine.get_session_instance()->get_Console(&console);

      /* Bring console window to front. */
      vm_machine->ShowConsoleWindow(0);

      printf("Press enter to power off VM and close the session...\n");
      getchar();

      /* Power down the machine. */
      rc = console->PowerDown(&progress);

      /* Wait until VM is powered down. */
      printf("Powering off VM, please wait ...\n");
      rc = progress->WaitForCompletion(-1);

      printf("Done ...\n");

      /* Close the session. */
      rc = machine.get_session_instance()->UnlockMachine();

    } while (0);

    SAFE_RELEASE(console);
    SAFE_RELEASE(progress);
  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
    return 0;
  }

	return 0;
}


int main()
{
  /* Initialize the COM subsystem. */
  CoInitialize(nullptr);
  {
    vb::virtual_box virtual_box;

    listVMs(virtual_box);
    testErrorInfo(virtual_box);

    /* Enable the following line to get a VM started. */
    testStartVM(virtual_box);
  }
  CoUninitialize();

	return 0;
}