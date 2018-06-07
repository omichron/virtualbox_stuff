#pragma once
#include "utilities.h"
#include <list>

namespace vb::wrapper
{
  template<typename TUnknown>
  class unknown
  {
  public:
    unknown<TUnknown>(TUnknown* iunknown_ = nullptr)
      : iunknown(iunknown_)
    { }
        
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

  template<typename TElement>
  class safe_array
  {
  public:
    safe_array(SAFEARRAY* safe_array = nullptr)
      : _safe_array(safe_array)
    { }

    ~safe_array()
    {
      SafeArrayDestroy(_safe_array);
    }

    SAFEARRAY*& ptr()
    {
      return _safe_array;
    }

    auto as_std_list()
    {
      return as_std_collection<std::list<TElement>>();
    }

  private:
    template<typename TCollection>
    TCollection as_std_collection()
    {
      TElement::i_unknown **safe_array_items;
      int rc = SafeArrayAccessData(_safe_array, (void **)&safe_array_items);
      vb::util::throw_if_failed(rc, "Could not access safe array");

      auto begin = &safe_array_items[0];
      auto end = begin + _safe_array->rgsabound[0].cElements;

      TCollection return_collection(begin, end);

      SafeArrayUnaccessData(_safe_array);

      return return_collection;
    }

  private:
    SAFEARRAY* _safe_array;
  };
}