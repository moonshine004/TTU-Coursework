#include <gui/containers/SongItem.hpp>
#include <touchgfx/Unicode.hpp>

int8_t SongItem::lastTapped = -1;

SongItem::SongItem() :
    songIdx(0)
{
    textArea1Buf[0] = 0;
}

void SongItem::initialize()
{
    SongItemBase::initialize();

    /*
     * Keep the row itself non-touchable so
     * ScrollList dragging stays smooth.
     */
    button1.setTouchable(false);

    textArea1.setWildcard(textArea1Buf);
    textArea1.setPosition(70, 30, 160, 35);

    textArea1.invalidate();
}

void SongItem::setName(const char* name)
{
    // fallback text if the slot is empty
    if (name == 0 || name[0] == '\0')
    {
        touchgfx::Unicode::fromUTF8(
            (const uint8_t*)"EMPTY",
            textArea1Buf,
            64);
    }
    else
    {
        touchgfx::Unicode::fromUTF8(
            (const uint8_t*)name,
            textArea1Buf,
            64);
    }

    textArea1Buf[63] = 0;

    textArea1.invalidate();
}

void SongItem::setSongIdx(uint8_t idx)
{
    songIdx = idx;
}

void SongItem::songClicked()
{
    /*
     * Touch handling is managed by Screen1View.
     * Keep this disabled so scrolling stays responsive.
     */
    lastTapped = -1;
}
