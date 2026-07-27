#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{
}

void Screen1Presenter::activate()
{
    // screen 1 handles the sd song list directly
}

void Screen1Presenter::deactivate()
{
    // nothing special needed when leaving the list screen
}
