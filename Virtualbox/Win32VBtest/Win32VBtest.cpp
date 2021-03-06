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

int listVMs(vb::virtual_box& virtual_box)
{
  for(auto& machine : virtual_box.get_machines())
    std::cout << vb::machine(machine).get_name() << std::endl;;
 
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
  try
  {
    auto machine = virtual_box.find_machine("Android");

      /* Wait until VM is running. */
      printf("Starting VM, please wait ...\n");
      
    machine.start();

    printf("Press enter to power off VM and close the session...\n");
    getchar();

    machine.power_down();
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
    testStartVM(virtual_box);
  }
  CoUninitialize();

	return 0;
}