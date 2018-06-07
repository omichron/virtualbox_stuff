#pragma once
#include "utilities.h"

namespace vb::wrapper
{
  template<typename TUnknown>
  class unknown
  {
  public:
    unknown<TUnknown>(TUnknown* iunknown_ = nullptr)
      : iunknown(iunknown_)
    {}
        
    ~unknown<TUnknown>()
    {
      safe_release();
    }

    TUnknown*& ptr() { return iunknown; }

  private:
    void safe_release()
    {
      if (iunknown)
      {
        iunknown->Release();
        iunknown = nullptr;
      }
    }

    TUnknown* iunknown = nullptr;
  };

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

    BSTR& get()
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