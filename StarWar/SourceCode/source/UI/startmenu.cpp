#include "../../include/starwar.h"
#include "../../include/UI.h"

RECT title = { 400,120,880,240 };
RECT start_button = { 540,400,740,450 };
RECT exit_button = { 540,500,740,550 };
RECT version = { 1100,685,1280,715 };
static int button1 = 0, button2 = 0;

void update_start_menu()
{
	pMOUSEMESSAGE mouse;
	peek_mouse_message();
	mouse = get_mouse_message();
	if (is_key_down(VK_Q))
	{
		status = exit_game;
		return;
	}
	if (is_mouse_in_rect(mouse->pos, start_button))
	{
		button1 = 1;
		if (mouse->is_down)
		{
			status = start_game;
			return;
		}
	}
	else
	{
		button1 = 0;
	}
	if (is_mouse_in_rect(mouse->pos, exit_button))
	{
		button2 = 1;
		if (mouse->is_down)
		{
			status = exit_game;
			return;
		}
	}
	else
	{
		button2 = 0;
	}
	return;
}

void draw_start_menu()
{
	cleardevice();

	draw_text(title, RGB(0, 0, 0), L"星际之战");
	draw_button(start_button, L"开始游戏", button1);
	draw_button(exit_button, L"退出游戏", button2);
	draw_text(version, RGB(0, 0, 0), L"version - 0.3.0");

	FlushBatchDraw();
}