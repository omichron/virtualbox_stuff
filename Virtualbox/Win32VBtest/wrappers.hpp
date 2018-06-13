#pragma once
#include "utilities.h"
#include <list>
#include <type_traits>
#include <windows.h>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <tuple>
#include "function_traits.hpp"

namespace vb::wrapper
{
  class bstr
  {
  public:
    bstr(const std::string& str)
      : _bstr(_com_util::ConvertStringToBSTR(str.c_str()))
    { }

    bstr(BSTR str)
      : _bstr(str)
    { }

    ~bstr()
    {
      SysFreeString(_bstr);
    }

    operator BSTR&()
    {
      return _bstr;
    }

    std::string str()
    {
      auto str = _com_util::ConvertBSTRToString(_bstr);
      auto ret = std::string(str);
      delete[] str;
      return ret;
    }

  private:
    BSTR _bstr;
  };
}