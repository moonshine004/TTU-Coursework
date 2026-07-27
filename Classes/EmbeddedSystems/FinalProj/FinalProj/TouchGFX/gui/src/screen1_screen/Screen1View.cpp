#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/containers/SongItem.hpp>
#include <string.h>

extern "C" {
    #include "cmsis_os.h"

    extern osMessageQueueId_t audioCmdQ;
    extern void gui_dbg(const char *s);

    extern char songNames[32][64];
    extern volatile uint8_t numSongs;
    extern volatile uint8_t fileListReady;
    extern volatile uint8_t sdRemoved;

    extern char curSongName[64];

    typedef struct {
        uint8_t  cmd;
        int32_t  param;
        char     filename[64];
    } AudioCmd_t;

    #define cmdplay 1
}

Screen1View::Screen1View() :
    listLoaded(0),
    cachedCount(0),
    itemClickCb(this, &Screen1View::itemClickHandler)
{
    memset(cachedSongs, 0, sizeof(cachedSongs));
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

    // connect the scroll list click callback
    scrollList1.setItemSelectedCallback(itemClickCb);

    // load whatever the SD scan currently has ready
    loadSongCache();

    scrollList1.setNumberOfItems(cachedCount);
    scrollList1.invalidate();

    listLoaded = (cachedCount > 0);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::loadSongCache()
{
    /*
     * Screen 1 uses songNames[][] directly.
     * cachedCount is only used to tell TouchGFX how many rows to draw.
     */
    if (!sdRemoved && fileListReady && numSongs > 0)
    {
        cachedCount = numSongs;

        if (cachedCount > 32)
        {
            cachedCount = 32;
        }
    }
    else
    {
        cachedCount = 0;
    }
}

void Screen1View::handleTickEvent()
{
    uint8_t oldCount = cachedCount;

    // check if the SD scan state changed
    loadSongCache();

    // only redraw when the number of visible songs changes
    if (oldCount != cachedCount || !listLoaded)
    {
        scrollList1.setItemSelectedCallback(itemClickCb);
        scrollList1.setNumberOfItems(cachedCount);
        scrollList1.invalidate();

        listLoaded = (cachedCount > 0);
    }
}

void Screen1View::scrollList1UpdateItem(SongItem& item, int16_t itemIndex)
{
    if (itemIndex >= 0 && itemIndex < numSongs)
    {
        item.setSongIdx((uint8_t)itemIndex);
        item.setName(songNames[itemIndex]);
    }
    else
    {
        item.setSongIdx(0);
        item.setName("EMPTY");
    }

    item.invalidate();
}

void Screen1View::itemClickHandler(int16_t itemIndex)
{
    gui_dbg("song row selected\r\n");

    if (itemIndex < 0 || itemIndex >= numSongs)
    {
        return;
    }

    // save the name so screen 2 can show it
    strncpy(curSongName, songNames[itemIndex], 63);
    curSongName[63] = '\0';

    // tell the audio task to play this index
    AudioCmd_t ac = {0};
    ac.cmd = cmdplay;
    ac.param = itemIndex;

    strncpy(ac.filename, songNames[itemIndex], 63);
    ac.filename[63] = '\0';

    osMessageQueuePut(audioCmdQ, &ac, 0, 0);

    application().gotoScreen2ScreenNoTransition();
}
