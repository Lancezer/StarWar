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
		draw_text(mid, RGB(0, 0, 0), L"������ʼ");
		break;
	case fre:
		draw_text(mid, RGB(0, 0, 0), L"���ɽ��� ��ʱ10����");
		break;
	case control:
		draw_text(tip, RGB(0, 0, 0), L"ѡ�����-������-���뾺��");
		break;
	case mine_lay:
		draw_text(tip, RGB(0, 0, 0), L"ѡ�����-ѡ�����-�������");
		break;
	case occupy:
		draw_text(tip, RGB(0, 0, 0), L"ѡ�����-ѡ�����-�ȴ��ж�");
		break;
	case purchase:
		draw_text(tip, RGB(0, 0, 0), L"�鿴����-�������");
		break;
	case result:
		draw_text(tip, RGB(0, 0, 0), L"����������ʾ");
		break;
	default:
		draw_text(tip, RGB(0, 0, 0), L"�����У����Ժ�...");
		break;
	}
}