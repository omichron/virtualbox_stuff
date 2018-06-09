#pragma once
#include "utilities.h"
#include <list>
#include <type_traits>
#include <windows.h>
#include <algorithm>
#include <functional>
#include <typeinfo>

namespace vb::wrapper
{
  template<typename TUnknown, typename TEnable = void>
  class unknown;

  template<typename TElement>
  class safe_array;

  template<typename TUnknown>
  using derived_from_IUnknown = std::is_base_of<IUnknown, TUnknown>;

  template<typename TUnknown, typename F, typename... Args>
  auto create_invoke_void(F&& f, Args&&... args)
  {
    return unknown<TUnknown>::create_impl<TUnknown, void>(std::forward<F>(f), std::forward<Args>(args)...);
  }

  template<typename TUnknown, typename F, typename... Args>
  auto create_invoke(F&& f, Args&&... args)
  {
    return unknown<TUnknown>::create_impl<TUnknown, TUnknown>(std::forward<F>(f), std::forward<Args>(args)...);
  }

  template<typename TUnknown>
  class unknown<TUnknown, typename std::enable_if<derived_from_IUnknown<TUnknown>::value>::type>
  {
  template <typename TUnknownOther, typename TEnable>
  friend class unknown;

  public:
    using i_unknown = TUnknown;

    unknown(TUnknown* unknown = nullptr)
    { 
      _unknown = unknown;
      
      if(_unknown)
        add_reference();
    }

    unknown(const unknown<TUnknown>& other)
    {
      _unknown = other._unknown;
      add_reference();
    }

    unknown& operator=(const unknown<TUnknown&> other)
    {
      if (this != &other)
      {
        safe_release();
        _unknown = other._unknown;
        
        if(_unknown)
          add_reference();
      }

      return *this;
    }

    unknown(unknown<TUnknown>&& other)
    {
      _unknown = other._unknown;
      other._unknown = nullptr;
    }

    unknown& operator=(unknown<TUnknown>&& other)
    {
      if (this != &other)
      {
        safe_release();
        _unknown = other._unknown;
        other._unknown = nullptr;
      }

      return *this;
    }

    unknown& operator=(TUnknown* unknown)
    {
      if (_unknown != unknown)
      {
        safe_release();
        _unknown = unknown;
      }
      
      return *this;
    }
        
    ~unknown()
    {
      safe_release();
    }

    auto operator->()
    {
      return _unknown;
    }

    bool is_valid()
    {
      return _unknown != nullptr;
    }

    template<typename TDeclared, typename TCasted, typename F, typename... Args>
    static auto create_impl(F&& f, Args&&... args)
    {
      TDeclared* u = nullptr;

      auto rc = std::invoke(std::forward<F>(f), std::forward<Args>(args)..., 
        reinterpret_cast<TCasted**>(&u));
      util::throw_if_failed(rc, typeid(F).name());

      return unknown<TDeclared>::wrap(u);
    }

    template<typename TOutput, typename F, typename... Args>
    auto create_invoke(F&& f, Args&&... args)
    {
      return create_impl<TOutput, TOutput>(std::forward<F>(f), _unknown, std::forward<Args>(args)...);
    }
  
    operator TUnknown*()
    {
      return _unknown;
    }
    
  private:
    void add_reference()
    {
      auto count = _unknown->AddRef();
      printf("==> %s: add_reference: %u\n", typeid(TUnknown).name(), count);
    }

    void safe_release()
    {
      if (_unknown)
      {
        auto count = _unknown->Release();
        printf("==> %s: safe_release:  %u\n", typeid(TUnknown).name(), count);
        _unknown = nullptr;
      }
    }

    static auto wrap(TUnknown* u)
    {
      unknown wrapped = u;
      u->Release();
      return wrapped;
    }

    TUnknown* _unknown = nullptr;
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

    operator SAFEARRAY*()
    {
      return _safe_array;
    }

    operator SAFEARRAY**()
    {
      return &_safe_array;
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