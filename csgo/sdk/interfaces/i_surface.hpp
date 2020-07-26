#pragma once
#include "../../utils/other/color.hpp"
#include "../../utils/other/utils.hpp"

enum e_font_flags_t
{
	font_flag_none,
	font_flag_italic = 0x001,
	font_flag_underline = 0x002,
	font_flag_strikeout = 0x004,
	font_flag_symbol = 0x008,
	font_flag_antialias = 0x010,
	font_flag_gaussianblur = 0x020,
	font_flag_rotary = 0x040,
	font_flag_dropshadow = 0x080,
	font_flag_additive = 0x100,
	font_flag_outline = 0x200,
	font_flag_custom = 0x400,
	font_flag_bitmap = 0x800,
};


struct Vertex_t
{
	Vector2D	mPosition;
	Vector2D	m_TexCoord;

	Vertex_t() {}
	Vertex_t(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		mPosition = pos;
		m_TexCoord = coord;
	}
	void Init(const Vector2D &pos, const Vector2D &coord = Vector2D(0, 0))
	{
		mPosition = pos;
		m_TexCoord = coord;
	}
};

typedef  Vertex_t FontVertex_t;

class ISurface
{
public:
	void unlock_cursor()
	{
		typedef void(__thiscall* Fn)(void*);
		Utils::GetVFunc< Fn >(this, 66)(this);
	}

	void DrawSetcolor_t(color_t col)
	{
		typedef void(__thiscall* Fn)(PVOID, color_t);
		return Utils::GetVFunc< Fn >(this, 14)(this, col);
	}

	void DrawSetcolor_t(int r, int g, int b, int a)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 15)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 16)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 18)(this, x0, y0, x1, y1);
	}

	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 19)(this, x0, y0, x1, y1);
	}

	void DrawPolyLine(int *x, int *y, int count)
	{
		typedef void(__thiscall* Fn)(PVOID, int *, int *, int);
		Utils::GetVFunc< Fn >(this, 20)(this, x, y, count);
	}

	void DrawSetTextFont(unsigned long font)
	{
		typedef void(__thiscall* Fn)(PVOID, unsigned long);
		Utils::GetVFunc< Fn >(this, 23)(this, font);
	}

	void DrawSetTextcolor_t(int r, int g, int b, int a)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 25)(this, r, g, b, a);
	}

	void DrawSetTextcolor_t(color_t col)
	{
		typedef void(__thiscall* Fn)(PVOID, color_t);
		return Utils::GetVFunc< Fn >(this, 24)(this, col);
	}

	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int);
		Utils::GetVFunc< Fn >(this, 26)(this, x, y);
	}

	void DrawPrintText(const wchar_t* text, int textLen)
	{
		typedef void(__thiscall* Fn)(PVOID, const wchar_t*, int, int);
		Utils::GetVFunc< Fn >(this, 28)(this, text, textLen, 0);
	}

	void DrawSetTexture(int textureID)
	{
		typedef void(__thiscall* Fn)(PVOID, int);
		return Utils::GetVFunc< Fn >(this, 38)(this, textureID);
	}

	void DrawSetTextureRGBA(int textureID, unsigned char  const* color_ts, int w, int h)
	{
		typedef void(__thiscall* Fn)(PVOID, int, unsigned char  const*, int, int);
		return Utils::GetVFunc< Fn >(this, 37)(this, textureID, color_ts, w, h);
	}

	int CreateNewTextureID(bool procedural)
	{
		typedef int(__thiscall* Fn)(PVOID, bool);
		return Utils::GetVFunc< Fn >(this, 43)(this, procedural);
	}

	void DrawTexturedPolygon(int vtxCount, FontVertex_t *vtx, bool bClipVertices = true)
	{
		typedef void(__thiscall* Fn)(PVOID, int, FontVertex_t*, bool);
		return Utils::GetVFunc< Fn >(this, 106)(this, vtxCount, vtx, bClipVertices);
	}

	unsigned long FontCreate()
	{
		typedef unsigned int(__thiscall* Fn)(PVOID);
		return Utils::GetVFunc< Fn >(this, 71)(this);
	}

	void SetFontGlyphSet(unsigned long font, const char* windowsFontName, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* Fn)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		Utils::GetVFunc< Fn >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void ISurface::GetTextSize(DWORD font, const wchar_t *text, int &wide, int &tall)
	{
		typedef void(__thiscall* Fn)(void*, DWORD, const wchar_t*, int&, int&);
		return Utils::GetVFunc<Fn>(this, 79)(this, font, text, wide, tall);
	}

	void DrawOutlinedCircle(int x, int y, int r, int seg)
	{
		typedef void(__thiscall* Fn)(PVOID, int, int, int, int);
		return Utils::GetVFunc< Fn >(this, 103)(this, x, y, r, seg);
	}

	void SurfaceGetCursorPos(int &x, int &y)
	{
		typedef void(__thiscall* Fn)(PVOID, int&, int&);
		return Utils::GetVFunc< Fn >(this, 66)(this, x, y);
	}

	void RoundedFilledRect(int x, int y, int width, int height, float radius, color_t col) { // UC https://www.unknowncheats.me/forum/1498179-post4.html
		// TODO: make the quality not hardcoded -green
		// don't you have to give it quality in the formula you wrote? 8 + 4 * ( quality ) ? -dex
		constexpr int quality = 24;

		static Vertex_t verts[quality];

		Vector2D add = { 0, 0 };

		for (int i = 0; i < quality; i++) {
			float angle = (static_cast < float > (i) / -quality) * 6.28f - (6.28f / 16.f);

			verts[i].mPosition.x = radius + x + add.x + (radius * sin(angle));
			verts[i].mPosition.y = height - radius + y + add.y + (radius * cos(angle));

			if (i == 4) {
				add.y = -height + (radius * 2);
			}
			else if (i == 10) {
				add.x = width - (radius * 2);
			}
			else if (i == 16) {
				add.y = 0;
			}
			else if (i == 22) {
				add.x = 0;
			}
		}
		DrawSetcolor_t(col.red, col.green, col.blue, col.alpha);
		DrawTexturedPolygon(quality, verts);
	}

	void FilledRect(int x, int y, int w, int h, color_t color_t)
	{
		DrawSetcolor_t(color_t.red, color_t.green, color_t.blue, color_t.alpha);
		DrawFilledRect(x, y, x + w, y + h);
	}

	void OutlinedRect(int x, int y, int w, int h, color_t color_t)
	{
		DrawSetcolor_t(color_t.red, color_t.green, color_t.blue, color_t.alpha);
		DrawOutlinedRect(x, y, x + w, y + h);
	}

	void OutlinedVecRect(int x, int y, int x2, int y2, color_t color_t)
	{
		DrawSetcolor_t(color_t.red, color_t.green, color_t.blue, color_t.alpha);
		DrawLine(x, y, x2, y);
		DrawLine(x2, y, x2, y2);
		DrawLine(x2, y2, x, y2);
		DrawLine(x, y2, x, y);
	}

	void Line(int x, int y, int x2, int y2, color_t color_t)
	{
		DrawSetcolor_t(color_t.red, color_t.green, color_t.blue, color_t.alpha);
		DrawLine(x, y, x2, y2);
	}

	void DrawT(int X, int Y, color_t color_t, int Font, bool Center, const char* _Input, ...)
	{
		int apple = 0;
		char Buffer[2048] = { '\0' };
		va_list Args;

		va_start(Args, _Input);
		vsprintf_s(Buffer, _Input, Args);
		va_end(Args);

		size_t Size = strlen(Buffer) + 1;
		wchar_t* WideBuffer = new wchar_t[Size];

		mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

		int Width = 0, Height = 0;

		if (Center)
			GetTextSize(Font, WideBuffer, Width, Height);

		DrawSetTextcolor_t(color_t.red, color_t.green, color_t.blue, color_t.alpha);
		DrawSetTextFont(Font);
		DrawSetTextPos(X - (Width / 2), Y);
		DrawPrintText(WideBuffer, wcslen(WideBuffer));
	}

	RECT GetTextSizeRect(DWORD font, const char* text) // ayyware or something
	{
		size_t origsize = strlen(text) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

		RECT rect; int x, y;
		GetTextSize(font, wcstring, x, y);
		rect.left = x; rect.bottom = y;
		rect.right = x;
		return rect;
	}

	Vector2D get_mouse_position() // bolbi ware
	{
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
		return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
	}

	bool mouse_in_region(int x, int y, int x2, int y2) {
		if ( get_mouse_position().x > x && get_mouse_position().y > y && get_mouse_position().x < x2 + x && get_mouse_position().y < y2 + y)
			return true;
		return false;
	}
};

extern ISurface* csgo_surface;