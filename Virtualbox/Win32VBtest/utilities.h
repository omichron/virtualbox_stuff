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
      vb::wrapper::unknown<IErrorInfo> vb_error_info = nullptr;
    };

    template<typename TCollection, typename TInputType>
    auto to_std_collection(SAFEARRAY *safe_array)
    {
      TInputType **safe_array_items;
      int rc = SafeArrayAccessData(safe_array, (void **)&safe_array_items);
      vb::util::throw_if_failed(rc, "Could not access safe array");

      auto begin = &safe_array_items[0];
      auto end = begin + safe_array->rgsabound[0].cElements;

      TCollection return_collection(begin, end);

      SafeArrayUnaccessData(safe_array);
      SafeArrayDestroy(safe_array);

      return return_collection;
    }

    template<typename TOutputType, typename TInputType>
    auto to_std_list(SAFEARRAY *safe_array)
    {
      return to_std_collection<std::list<TOutputType>, TInputType>(safe_array);
    }
}


