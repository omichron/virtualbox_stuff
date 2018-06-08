#pragma once
#include <string>
#include <comutil.h>  
#include <list>
#include "wrappers.hpp"

namespace vb::util
{
    void throw_if_failed(HRESULT rc, const char* message);

    class error_info
    {
    public:
      error_info();
      ~error_info();
      std::string get_description();
    private:
      vb::wrapper::unknown<IErrorInfo> vb_error_info;
    };
}


