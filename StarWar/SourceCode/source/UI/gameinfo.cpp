#include "../../include/UI.h"
#include "../../include/starwar.h"

RECT rnd = { 750,30,960,90 };
RECT pha = { 970,30,1250,90 };
RECT tip = { 750,110,1250,150 };

static int rndbutton = 0, phabutton = 0;

void update_gameinfo(pMOUSEMESSAGE mouse)
{
	RECT gameinfo_area = { 720,0,1280,150 };
	if (!is_mouse_in_rect(mouse->pos, gameinfo_area))
	{
		rndbutton = 0, phabutton = 0;
		return;
	}

	if (is_mouse_in_rect(mouse->pos, rnd))
	{
		rndbutton = 1;
	}
	else
	{
		rndbutton = 0;
	}
	if (is_mouse_in_rect(mouse->pos, pha))
	{
		phabutton = 1;
	}
	else
	{
		phabutton = 0;
	}
}

void draw_gameinfo()
{
	draw_button(rnd, round_name[round], rndbutton);
	draw_button(pha, phase_name[phase], phabutton);
	// every phase
	switch (phase)
	{
	case preload:
		draw_text(mid, RGB(0, 0, 0), L"即将开始");
		break;
	case fre:
		draw_text(mid, RGB(0, 0, 0), L"自由交流 限时10分钟");
		break;
	case control:
		draw_text(tip, RGB(0, 0, 0), L"选择玩家-输入金额-参与竞拍");
		break;
	case mine_lay:
		draw_text(tip, RGB(0, 0, 0), L"选择地雷-选择格子-埋入地雷");
		break;
	case occupy:
		draw_text(tip, RGB(0, 0, 0), L"选择道具-选择格子-等待判定");
		break;
	case purchase:
		draw_text(tip, RGB(0, 0, 0), L"查看余量-购买道具");
		break;
	case result:
		draw_text(tip, RGB(0, 0, 0), L"排名如下所示");
		break;
	default:
		draw_text(tip, RGB(0, 0, 0), L"加载中，请稍候...");
		break;
	}
}