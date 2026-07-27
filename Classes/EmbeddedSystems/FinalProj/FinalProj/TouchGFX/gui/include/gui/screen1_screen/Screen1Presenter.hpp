#ifndef SCREEN1PRESENTER_HPP
#define SCREEN1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen1View;

/*
 * Presenter for the song selection screen.
 * Most of the logic is handled directly in the view.
 */
class Screen1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen1Presenter(Screen1View& v);
    virtual ~Screen1Presenter() {}

    virtual void activate();
    virtual void deactivate();

private:
    Screen1Presenter();

    // reference to the active screen 1 view
    Screen1View& view;
};

#endif // SCREEN1PRESENTER_HPP
