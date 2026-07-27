#ifndef SCREEN2VIEW_HPP
#define SCREEN2VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>
#include <touchgfx/Unicode.hpp>
#include <stdint.h>

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    virtual void function1();
    virtual void function2();
    virtual void function3();
    virtual void function4();
    virtual void function5();
    virtual void function6();

private:
    // small helper functions for cleaner button code
    void sendAudioCommand(uint8_t cmd, int32_t param);
    void refreshSongTitle();
    void refreshTimeText();

    touchgfx::Unicode::UnicodeChar songNameBuf[64];
    touchgfx::Unicode::UnicodeChar timeBuf[20];
};

#endif // SCREEN2VIEW_HPP
