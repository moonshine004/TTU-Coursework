#include <gui/screen2_screen/Screen2View.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

Screen2Presenter::Screen2Presenter(Screen2View& v)
    : view(v)
{
}

void Screen2Presenter::activate()
{
    // playback values are updated from main.c and shown by the view
}

void Screen2Presenter::deactivate()
{
    // no cleanup needed when leaving playback screen
}
