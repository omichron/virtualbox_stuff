#include "utilities.h"
#include <exception>
#include <comutil.h>  
#include "wrappers.hpp"
#include <windows.h>
#include <comdef.h>

vb::util::error_info::error_info()
{
  IErrorInfo* error_info = nullptr;
  auto ret = GetErrorInfo(0, &error_info);

  vb_error_info = SUCCEEDED(ret) ? error_info : nullptr;
}

std::string vb::util::error_info::get_description()
{
  if (!vb_error_info.is_valid())
    return "No error information available";
  
  BSTR errorDescription = nullptr;
  auto rc = vb_error_info->GetDescription(&errorDescription);

  if (FAILED(rc) || !errorDescription)
    return "Could not retrieve error information";

  return wrapper::bstr(errorDescription).str();
}

void vb::util::throw_if_failed(HRESULT rc, const char* message)
{
  if (FAILED(rc))
  {
    auto error_message = std::string(_com_error(rc).ErrorMessage());
    throw (std::exception((std::string(message) + ": " + error_message + " - " + error_info().get_description()).c_str()));
  }  
}