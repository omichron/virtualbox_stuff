#pragma once
#include "utilities.h"
#include <list>
#include <vector>
#include <type_traits>
#include <windows.h>

namespace vb::wrapper
{
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

    auto as_std_vector()
    {
      return as_std_collection<std::vector<TElement>>();
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
}