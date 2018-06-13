#include "display.h"

vb::display::display(vb::wrapper::com<IConsole> console)
  : vb_console(std::move(console))
  , vb_display(vb_console.create_invoke(&IConsole::get_Display))
{
  update();
}

void vb::display::update()
{
  vb_display->GetScreenResolution(0, &width, &height, &bits_per_pixel, &x_origin, &y_origin, &status);
}