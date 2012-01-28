#ifndef GAMEFONT_H
#define GAMEFONT_H

#include <D3DX10.h>
#include "Globals.hpp"

#pragma comment(lib, "d3dx10.lib")

class GameFont
{
public:
	enum AlignVertical
	{
		Top, Middle, Bottom
	};

	enum AlignHorizontal
	{
		Left, Center, Right
	};

	GameFont();
	GameFont(ID3D10Device* graphicsDevice, TCHAR* name = "Times New Roman", int size = 18, 
		bool italic = false, bool bold = false);
	~GameFont();
	void WriteText(LPCWSTR text, POINT position, D3DXCOLOR textColor);
	void WriteText(LPCWSTR text, RECT* destinationRect, D3DXCOLOR textColor, AlignHorizontal alignHor, 
					AlignVertical alignVert);
	void ChangeFontName(TCHAR* newName);
	void ChangeSize(int newSize);
	void ChangeBoldItalic(bool italic, bool bold);

private:
	ID3DX10Font* mFont;
	D3DX10_FONT_DESC mFontDesc;
	ID3D10Device* mGraphicsDevice;
};

#endif