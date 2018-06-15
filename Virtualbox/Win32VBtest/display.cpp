#include "display.h"

const ULONG screen_id = 0;

vb::display::display(vb::wrapper::com<IConsole> console)
  : vb_console(std::move(console))
  , vb_display(vb_console.create_invoke(&IConsole::get_Display))
{
}

void vb::display::update()
{
  vb_display->GetScreenResolution(screen_id, &width, &height, &bits_per_pixel, &x_origin, &y_origin, &status);
}

void vb::display::take_screenshot()
{
  //uint8_t* screen_shot = new uint8_t[width*height*3];
  SAFEARRAY* screen_data = nullptr;
  auto rc = vb_display->TakeScreenShotToArray(screen_id, width, height, BitmapFormat::BitmapFormat_PNG, &screen_data);

 // if(FAILED(rc))
 //   delete[] screen_shot;

  vb::util::throw_if_failed(rc, "Could not take screenshot");

 // delete[] screen_shot;
}

