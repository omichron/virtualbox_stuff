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
  template<typename TCom, typename TEnable = void>
  class com;

  template<typename TElement>
  class safe_array;

  template<typename TCom>
  using derived_from_IUnknown = std::is_base_of<IUnknown, TCom>;

  template<typename TDeclared, typename TCasted, typename F, typename... Args>
  static auto create_invoke_impl(F&& f, Args&&... args)
  {
    TDeclared* u = nullptr;

    auto rc = std::invoke(std::forward<F>(f), std::forward<Args>(args)..., reinterpret_cast<TCasted**>(&u));
    util::throw_if_failed(rc, typeid(F).name());

    return com<TDeclared>::wrap(u);
  }

  template<typename F, typename... Args>
  auto create_invoke(F&& f, Args&&... args)
  {
    using unknown_t = std::remove_pointer<std::remove_pointer<ArgumentInfo<F>::last_argument_t>::type>::type;
    return create_invoke_impl<unknown_t, unknown_t>(std::forward<F>(f), std::forward<Args>(args)...);
  }

  template<typename TCom, typename F, typename... Args>
  auto create_invoke_void(F&& f, Args&&... args)
  {
    return create_invoke_impl<TCom, void>(std::forward<F>(f), std::forward<Args>(args)...);
  }

  template<typename TCom>
  auto create_invoke_CoCreateInstance(const CLSID clsid, const IID iid)
  {
    return create_invoke_void<TCom>(CoCreateInstance, clsid, nullptr, CLSCTX_INPROC_SERVER, iid);
  }

  template<typename TCom>
  class com<TCom, typename std::enable_if<derived_from_IUnknown<TCom>::value>::type>
  {
  template <typename TComOther, typename TEnable>
  friend class com;

  template<typename TDeclared, typename TCasted, typename F, typename... Args>
  friend auto create_invoke_impl(F&& f, Args&&... args);

  public:
    template<typename F, typename... Args>
    auto create_invoke(F&& f, Args&&... args)
    {
      return vb::wrapper::create_invoke(std::forward<F>(f), com_object, std::forward<Args>(args)...);
   }

    using com_type = TCom;

    com(TCom* com = nullptr)
    { 
      com_object = com;
      
      if(com_object)
        add_reference();
    }

    com(const com<TCom>& other)
    {
      com_object = other.com_object;
      add_reference();
    }

    com& operator=(const com<TCom&> other)
    {
      if (this != &other)
      {
        safe_release();
        com_object = other.com_object;
        
        if(com_object)
          add_reference();
      }

      return *this;
    }

    com(com<TCom>&& other)
    {
      com_object = other.com_object;
      other.com_object = nullptr;
    }

    com& operator=(com<TCom>&& other)
    {
      if (this != &other)
      {
        safe_release();
        com_object = other.com_object;
        other.com_object = nullptr;
      }

      return *this;
    }

    com& operator=(TCom* com)
    {
      if (com_object != com)
      {
        safe_release();
        com_object = com;
      }
      
      return *this;
    }
        
    ~com()
    {
      safe_release();
    }

    auto operator->()
    {
      return com_object;
    }

    bool is_valid()
    {
      return com_object != nullptr;
    }
 
    operator TCom*()
    {
      return com_object;
    }
    
    private:
    void add_reference()
    {
      auto count = com_object->AddRef();
      printf("==> %s: add_reference: %u\n", typeid(TCom).name(), count);
    }

    void safe_release()
    {
      if (com_object)
      {
        auto count = com_object->Release();
        printf("==> %s: safe_release:  %u\n", typeid(TCom).name(), count);
        com_object = nullptr;
      }
    }

    static auto wrap(TCom* u)
    {
      com wrapped = u;
      u->Release();
      return wrapped;
    }

    TCom* com_object = nullptr;
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
      TElement::com_type **safe_array_items;
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