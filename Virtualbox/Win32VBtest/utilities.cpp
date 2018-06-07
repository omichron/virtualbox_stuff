#include "utilities.h"
#include <exception>

using namespace vb;

util::error_info::error_info()
{
  auto rc = GetErrorInfo(0, &vb_error_info.ptr());
  if (FAILED(rc))
    vb_error_info = nullptr;
}

util::error_info::~error_info()
{}

std::string util::error_info::get_description()
{
  if (!vb_error_info.ptr())
    return "No error information available";
  
  BSTR errorDescription = nullptr;
  auto rc = vb_error_info.ptr()->GetDescription(&errorDescription);

  if (FAILED(rc) || !errorDescription)
    return "Could not retrieve error information";

  return wrapper::bstr(errorDescription).str();
}

void util::throw_if_failed(HRESULT rc, const char* message)
{
  if (FAILED(rc))
    throw (std::exception((std::string(message) + ": " + error_info().get_description()).c_str()));
}