#include <gui/screen2_screen/Screen2View.hpp>
#include <touchgfx/Unicode.hpp>

extern "C" {
    #include "cmsis_os.h"

    extern osMessageQueueId_t audioCmdQ;
    extern volatile uint32_t curTimeSec;
    extern volatile uint32_t totTimeSec;
    extern volatile uint8_t  isPlaying;
    extern volatile uint8_t  isPaused;
    extern volatile uint8_t  sdRemoved;
    extern char curSongName[64];

    typedef struct {
        uint8_t  cmd;
        int32_t  param;
        char     filename[64];
    } AudioCmd_t;

    #define cmdpauz    2
    #define cmdresume  3
    #define cmdfwd     4
    #define cmdrew     5
    #define cmdnext    8
    #define cmdprev    9
    #define cmdshufl   10
}

Screen2View::Screen2View()
{
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();

    // setup song title and timer text
    songNameText.setWildcard(songNameBuf);
    songNameText.setPosition(70, 25, 160, 35);

    timeText.setWildcard(timeBuf);
    timeText.setPosition(70, 70, 160, 30);

    refreshSongTitle();
    refreshTimeText();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::handleTickEvent()
{
    // leave playback screen if sd card is removed
    if (sdRemoved)
    {
        application().gotoScreen1ScreenNoTransition();
        return;
    }

    refreshTimeText();
    refreshSongTitle();
}

void Screen2View::sendAudioCommand(uint8_t cmd, int32_t param)
{
    AudioCmd_t ac = {0};

    ac.cmd = cmd;
    ac.param = param;

    osMessageQueuePut(audioCmdQ, &ac, 0, 0);
}

void Screen2View::refreshSongTitle()
{
    if (curSongName[0] == '\0')
    {
        touchgfx::Unicode::fromUTF8((const uint8_t *)"No song", songNameBuf, 64);
    }
    else
    {
        touchgfx::Unicode::fromUTF8((const uint8_t *)curSongName, songNameBuf, 64);
    }

    songNameBuf[63] = 0;
    songNameText.invalidate();
}

void Screen2View::refreshTimeText()
{
    uint32_t curMin = curTimeSec / 60;
    uint32_t curSec = curTimeSec % 60;
    uint32_t totMin = totTimeSec / 60;
    uint32_t totSec = totTimeSec % 60;

    touchgfx::Unicode::snprintf(timeBuf, 20,
        "%d:%02d / %d:%02d", curMin, curSec, totMin, totSec);

    timeText.invalidate();
}

void Screen2View::function1()
{
    // pause or resume depending on current state
    if (isPlaying && !isPaused)
    {
        sendAudioCommand(cmdpauz, 0);
    }
    else
    {
        sendAudioCommand(cmdresume, 0);
    }
}

void Screen2View::function2()
{
    // previous song
    sendAudioCommand(cmdprev, 0);
}

void Screen2View::function3()
{
    // next song
    sendAudioCommand(cmdnext, 0);
}

void Screen2View::function4()
{
    // rewind 15 sec
    sendAudioCommand(cmdrew, 15);
}

void Screen2View::function5()
{
    // fast forward 15 sec
    sendAudioCommand(cmdfwd, 15);
}

void Screen2View::function6()
{
    // toggle shuffle mode
    sendAudioCommand(cmdshufl, 0);
}
