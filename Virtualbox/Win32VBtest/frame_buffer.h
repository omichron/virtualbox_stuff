#pragma once
#include <windows.h>
#include "VirtualBox.h"
#include <cstdio>

#define _ATL_FREE_THREADED
#include <atlbase.h>
#include <atlcom.h>
#include <atomic>


//static const GUID 
//aa59cc82 - 55c5 - 4913 - 93d7 - 93bf6c94a9c7
//const GUID IID_Framebuffer = { 0xaa59cc82, 0x55c5, 0x4913,{ 0x93, 0xbf, 0x6c, 0x94, 0xa9, 0xc7, 0xa2, 0xb5 } };

//https://github.com/evanphx/yoke/blob/master/src/VBox/Frontends/VirtualBox/src/runtime/UIFrameBuffer.h
namespace vb
{
  class frame_buffer :
    public ATL::IDispatchImpl<IFramebuffer, &IID_IFramebuffer, &LIBID_VirtualBox, kTypeLibraryMajorVersion, kTypeLibraryMinorVersion>
  {
  private:
    std::atomic<unsigned int> reference_count;

  public:
    frame_buffer();
    virtual ~frame_buffer();

    // IUnknown
    STDMETHOD_(ULONG, AddRef)() override 
    { 
      printf("==>frame_buffer: %s\n", __FUNCTION__);
      return ++reference_count;
    };

    STDMETHOD_(ULONG, Release)() override 
    { 
      printf("==>frame_buffer: %s\n", __FUNCTION__);
      return --reference_count;
    };

    STDMETHOD(QueryInterface)(REFIID riid, void **ppObj)                     \
    {
      if (riid == IID_IFramebuffer)
      {
        *ppObj = (IFramebuffer*)this;
        AddRef();
        printf("==>frame_buffer: %s IID_IFramebuffer\n", __FUNCTION__);
        return S_OK;
      }

      if (riid == IID_IUnknown)
      {
        *ppObj = (IUnknown*)this;
        AddRef();
        printf("==>frame_buffer: %s IID_IUnknown\n", __FUNCTION__);
        return S_OK;
      }

      if (riid == IID_IDispatch)
      {
        *ppObj = (IDispatch*)this;
        AddRef();
        printf("==>frame_buffer: %s IID_IDispatch\n", __FUNCTION__);
        return S_OK;
      }

      if (riid == IID_IMarshal)
      {
        *ppObj = (IMarshal*)this;
        AddRef();
        printf("==>frame_buffer: %s IID_IMarshal\n", __FUNCTION__);
        return S_OK;
      }

      *ppObj = NULL;
      return E_NOINTERFACE;
    }

    // IFrameBuffer
    HRESULT STDMETHODCALLTYPE get_Width(ULONG* aWidth) override;
    HRESULT STDMETHODCALLTYPE get_Height(ULONG *aHeight) override;
    HRESULT STDMETHODCALLTYPE get_BitsPerPixel(ULONG *aBitsPerPixel) override;
    HRESULT STDMETHODCALLTYPE get_BytesPerLine(ULONG *aBytesPerLine) override;
    HRESULT STDMETHODCALLTYPE get_PixelFormat(BitmapFormat *aPixelFormat) override;
    HRESULT STDMETHODCALLTYPE get_HeightReduction(ULONG *aHeightReduction) override;
    HRESULT STDMETHODCALLTYPE get_Overlay(IFramebufferOverlay **aOverlay) override;
    HRESULT STDMETHODCALLTYPE get_WinId(LONG64 *aWinId) override;
    HRESULT STDMETHODCALLTYPE get_Capabilities(SAFEARRAY * *aCapabilities) override;
    virtual HRESULT STDMETHODCALLTYPE NotifyUpdate(ULONG aX, ULONG aY, ULONG aWidth, ULONG aHeight) override;
    virtual HRESULT STDMETHODCALLTYPE NotifyUpdateImage(ULONG aX, ULONG aY, ULONG aWidth, ULONG aHeight, SAFEARRAY * aImage) override;
    virtual HRESULT STDMETHODCALLTYPE NotifyChange(ULONG aScreenId, ULONG aXOrigin, ULONG aYOrigin, ULONG aWidth, ULONG aHeight) override;
    virtual HRESULT STDMETHODCALLTYPE VideoModeSupported(ULONG aWidth, ULONG aHeight, ULONG aBpp, BOOL *aSupported) override;
    virtual HRESULT STDMETHODCALLTYPE GetVisibleRegion(BYTE *aRectangles, ULONG aCount, ULONG *aCountCopied) override;
    virtual HRESULT STDMETHODCALLTYPE SetVisibleRegion(BYTE *aRectangles, ULONG aCount) override;
    virtual HRESULT STDMETHODCALLTYPE ProcessVHWACommand(BYTE *aCommand, LONG aEnmCmd, BOOL aFromGuest) override;
    virtual HRESULT STDMETHODCALLTYPE Notify3DEvent(ULONG aType, SAFEARRAY * aData) override;
  };
}


