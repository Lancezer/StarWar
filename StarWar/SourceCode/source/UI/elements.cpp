#include "../../include/UI.h"

void draw_text(RECT r, COLORREF color, const wchar_t* str)
{
	settextcolor(color);
	settextstyle(r.bottom - r.top, 0, font,0,0,0,false,false,false,DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH);
	drawtext(str, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void draw_button(RECT button, const wchar_t* str,int keystatus)
{
	setlinecolor(RGB(100, 100, 100));
	setlinestyle(PS_SOLID,3);
	setfillcolor(RGB(100,100,100));
	if (!keystatus)
	{
		setfillstyle(BS_NULL);
	}
	else
	{
		setfillstyle(BS_SOLID);
	}
	fillrectangle(button.left, button.top, button.right, button.bottom);
	
	int width = button.right - button.left, height = button.bottom - button.top;
	RECT _button = { button.left + width / 20,button.top + height / 20,
						button.right - width / 20,button.bottom - height / 20 };
	if (!keystatus)
	{
		draw_text(_button, RGB(50, 50, 50), str);
	}
	else
	{
		draw_text(_button, RGB(255, 255, 255), str);
	}
}

void draw_block(RECT block, int id, int keystatus, const STYLEGROUP blockstyle)
{
	setlinecolor(blockstyle.linecolor);
	setlinestyle(PS_SOLID, 3);
	setfillstyle(BS_SOLID);
	if (!keystatus)
	{
		setfillcolor(blockstyle.bkcolor1);
	}
	else if (keystatus == 1)
	{
		setfillcolor(blockstyle.bkcolor2);
	}
	else
	{
		setfillcolor(RGB(255, 0, 0));
	}
	fillrectangle(block.left, block.top, block.right, block.bottom);

	wchar_t num[10] = { 0 };
	if (id && id < 10)
	{
		wsprintf(num, L"%d", id);
	}
	int width = block.right - block.left, height = block.bottom - block.top;
	RECT _block = { block.left + width / 20,block.top + height / 20,
						block.right - width / 20,block.bottom - height / 20 - 1 };
	if (!keystatus)
	{
		draw_text(_block, blockstyle.strcolor1, num);
	}
	else if (keystatus == 1)
	{
		draw_text(_block, blockstyle.strcolor2, num);
	}
}

void draw_num_info(RECT button, const wchar_t* str, int num, int mode)
{
	draw_button(button, str, mode);

	wchar_t numch[10] = { 0 };
	wsprintf(numch, L"%d", num);
	int height = button.bottom - button.top,width = 100;
	if (phase == purchase)
	{
		width = 60;
	}
	RECT text = { button.right + 20,button.top - height / 5 - 2,
					button.right + 20 + width,button.bottom + height / 5 - 2};
	draw_text(text, RGB(0, 0, 0), numch);
}