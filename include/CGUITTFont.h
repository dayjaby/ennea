#ifndef __C_GUI_TTFONT_H_INCLUDED__
#define __C_GUI_TTFONT_H_INCLUDED__

#include <iostream>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <irrlicht/irrlicht.h>

namespace irr
{
namespace gui
{
class CGUITTFace : public IReferenceCounted
{
public:
	FT_Library	library;
	FT_Face		face;
	bool load(const c8* filename);
};
class CGUITTGlyph : public IReferenceCounted
{
public:
	bool cached;
	video::IVideoDriver* Driver;
	CGUITTGlyph();
	virtual ~CGUITTGlyph();
	void cache(u32 idx);
	FT_Face *face;
	u32 size;
	u32 top;
	u32 left;
	u32 texw;
	u32 texh;
	u32 imgw;
	u32 imgh;
	video::ITexture *tex;
	u32 top16;
	u32 left16;
	u32 texw16;
	u32 texh16;
	u32 imgw16;
	u32 imgh16;
	video::ITexture *tex16;
	s32 offset;
	u8 *image;
};
class CGUITTFont : public IGUIFont
{
public:

	//! constructor
	CGUITTFont(video::IVideoDriver* Driver);

	//! destructor
	virtual ~CGUITTFont();

	//! loads a truetype font file
	bool attach(CGUITTFace *Face,u32 size);

	//! draws an text and clips it to the specified rectangle if wanted
	virtual void draw(const core::stringw& text, const core::rect<s32>& position, video::SColor color, bool hcenter=false, bool vcenter=false, const core::rect<s32>* clip=0);

	//! returns the dimension of a text
	virtual core::dimension2d<u32> getDimension(const wchar_t* text) const;

	//! Calculates the index of the character in the text which is on a specific position.
	virtual s32 getCharacterFromPos(const wchar_t* text, s32 pixel_x) const;
	virtual void setKerningWidth(s32 w) { kw = w; }
	virtual void setKerningHeight(s32 h) { kh = h; }
	s32 getKerningWidth(const wchar_t*, const wchar_t*) const {
	    return kw;
	}
	s32 getKerningHeight() const {
	    return kh;
	}
	virtual void setInvisibleCharacters(const wchar_t* s) {}
	s32 getWidthFromWord(const core::stringw& w) const {
	    s32 total = 0;
	    for(int i=0;i<w.size();i++)
            total+=getWidthFromCharacter(w[i]);
	    return total;
	}

	bool AntiAlias;
	bool TransParency;
private:
    s32 kw,kh;
	s32 getWidthFromCharacter(wchar_t c) const;
	u32 getGlyphByChar(wchar_t c);
	video::IVideoDriver* Driver;
	core::array< CGUITTGlyph > Glyphs;
	CGUITTFace *tt_face;
};

} // end namespace gui
} // end namespace irr

#endif

