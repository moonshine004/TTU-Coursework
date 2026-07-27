#ifndef SONGITEM_HPP
#define SONGITEM_HPP

#include <gui_generated/containers/SongItemBase.hpp>
#include <touchgfx/Unicode.hpp>

class SongItem : public SongItemBase
{
public:
    SongItem();
    virtual ~SongItem() {}

    virtual void initialize();

    void setName(const char* name);
    void setSongIdx(uint8_t idx);

    virtual void songClicked();

    // debug helper used during scroll touch testing
    static int8_t lastTapped;

private:
    uint8_t songIdx;

    // text buffer for the wav file name
    touchgfx::Unicode::UnicodeChar textArea1Buf[64];
};

#endif // SONGITEM_HPP
