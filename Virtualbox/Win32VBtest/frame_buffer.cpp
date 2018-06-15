#include "frame_buffer.h"
#include <cstdio>

vb::frame_buffer::frame_buffer()
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
}

vb::frame_buffer::~frame_buffer()
{
  printf("==>frame_buffer:%s\n", __FUNCTION__);
}

HRESULT vb::frame_buffer::get_Width(ULONG * aWidth)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::get_Height(ULONG * aHeight)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::get_BitsPerPixel(ULONG * aBitsPerPixel)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::get_BytesPerLine(ULONG * aBytesPerLine)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::get_PixelFormat(BitmapFormat * aPixelFormat)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::get_HeightReduction(ULONG * aHeightReduction)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::get_Overlay(IFramebufferOverlay ** aOverlay)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::get_WinId(LONG64 * aWinId)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::get_Capabilities(SAFEARRAY ** aCapabilities)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::NotifyUpdate(ULONG aX, ULONG aY, ULONG aWidth, ULONG aHeight)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::NotifyUpdateImage(ULONG aX, ULONG aY, ULONG aWidth, ULONG aHeight, SAFEARRAY * aImage)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::NotifyChange(ULONG aScreenId, ULONG aXOrigin, ULONG aYOrigin, ULONG aWidth, ULONG aHeight)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::VideoModeSupported(ULONG aWidth, ULONG aHeight, ULONG aBpp, BOOL * aSupported)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::GetVisibleRegion(BYTE * aRectangles, ULONG aCount, ULONG * aCountCopied)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::SetVisibleRegion(BYTE * aRectangles, ULONG aCount)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::ProcessVHWACommand(BYTE * aCommand, LONG aEnmCmd, BOOL aFromGuest)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}

HRESULT vb::frame_buffer::Notify3DEvent(ULONG aType, SAFEARRAY * aData)
{
  printf("==>frame_buffer: %s\n", __FUNCTION__);
  return E_NOTIMPL;
}
