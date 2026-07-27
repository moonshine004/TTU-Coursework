#include <gui/common/FrontendApplication.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <gui/screen2_screen/Screen2View.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>
#include <touchgfx/transitions/NoTransition.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{
}

void FrontendApplication::gotoScreen2ScreenNoTransition()
{
    // custom jump from song list to playback screen
    transitionCallback =
        touchgfx::Callback<FrontendApplicationBase>(
            this,
            (void (FrontendApplicationBase::*)())&FrontendApplication::gotoScreen2ScreenNoTransitionImpl
        );

    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoScreen2ScreenNoTransitionImpl()
{
    touchgfx::makeTransition<Screen2View, Screen2Presenter, touchgfx::NoTransition, Model>(
        &currentScreen,
        &currentPresenter,
        frontendHeap,
        &currentTransition,
        &model
    );
}
