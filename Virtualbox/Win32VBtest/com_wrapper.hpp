#pragma once
#include "utilities.h"
#include <list>
#include <type_traits>
#include <windows.h>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <tuple>
#include "VirtualBox.h"
#include "function_traits.hpp"

namespace vb::wrapper
{
  template<typename TCom, typename TEnable = void>
  class com;

  template<typename TElement>
  class safe_array;

  template<typename TCom>
  using derived_from_IUnknown = std::is_base_of<IUnknown, TCom>;

  template<typename TCom>
  struct ids_of_com_object;

  #define SPECIALIZE_IDS_FOR_COM_OBJECT_TYPE_FOR(object_name) \
  template<> \
  struct ids_of_com_object<I##object_name> { \
    static constexpr CLSID const* clsid = &CLSID_##object_name; \
    static constexpr IID const* iid = &IID_I##object_name; }

  SPECIALIZE_IDS_FOR_COM_OBJECT_TYPE_FOR(Session);
  SPECIALIZE_IDS_FOR_COM_OBJECT_TYPE_FOR(VirtualBoxClient);

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
  auto create_invoke_CoCreateInstance()
  {
    return create_invoke_CoCreateInstance<TCom>(*ids_of_com_object<TCom>::clsid, *ids_of_com_object<TCom>::iid);
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
    void print_reference_count(const char* function_name)
    {
      printf("==> %s: %s:%p %u\n", function_name, typeid(TCom).name(), com_object, reference_count);
    }

    void add_reference()
    {
      reference_count = com_object->AddRef();
      print_reference_count("add_reference");
    }

    void safe_release()
    {
      if (com_object)
      {
        reference_count = com_object->Release();
        print_reference_count("safe_release");
        com_object = nullptr;
      }
    }

    static auto wrap(TCom* u)
    {
      com wrapped = u;
      wrapped.reference_count = u->Release();
      wrapped.print_reference_count("wrap");
      return wrapped;
    }

    TCom* com_object = nullptr;
    unsigned int reference_count;
  };
}