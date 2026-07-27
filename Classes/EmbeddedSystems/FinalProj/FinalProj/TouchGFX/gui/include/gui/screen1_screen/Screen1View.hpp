#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}

    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    virtual void scrollList1UpdateItem(SongItem& item, int16_t itemIndex);

private:
    uint8_t listLoaded;
    uint8_t cachedCount;
    char cachedSongs[32][64];

    // kept for older list logic, but screen 1 mainly uses songNames directly
    void loadSongCache();

    // callback used when a song row is pressed
    touchgfx::Callback<Screen1View, int16_t> itemClickCb;
    void itemClickHandler(int16_t itemIndex);
};

#endif // SCREEN1VIEW_HPP
