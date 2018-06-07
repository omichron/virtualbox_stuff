#pragma once
#include "utilities.h"
#include <list>
#include <type_traits>
#include <windows.h>

namespace vb::wrapper
{
  template<typename TUnknown, typename TEnable = void>
  class unknown;

  template<typename TUnknown>
  class unknown<TUnknown, 
    typename std::enable_if<std::is_base_of_v<IUnknown, TUnknown>>::type>
  {
  public:
    unknown(TUnknown* unknown = nullptr)
      : _unknown(unknown)
    { }
        
    ~unknown()
    {
      safe_release();
    }

    auto& ptr() { return _unknown; }

  private:
    void safe_release()
    {
      if (_unknown)
      {
        _unknown->Release();
        _unknown = nullptr;
      }
    }

    TUnknown* _unknown = nullptr;
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

    auto& get()
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

    auto& ptr()
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