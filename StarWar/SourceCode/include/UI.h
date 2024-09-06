#pragma once

#ifndef _UI_H_
#define _UI_H_

#include <Windows.h>
#include <easyx.h>
#include "starwar.h"
#include "message.h"

typedef struct _stylegroup
{
	COLORREF bkcolor1;
	COLORREF strcolor1;
	COLORREF bkcolor2;
	COLORREF strcolor2;
	COLORREF linecolor;
	int size;
} STYLEGROUP,*pSTYLEGROUP;

// update
// update main ui
void update_start_menu();
void update_board(pMOUSEMESSAGE mouse);
void update_gameinfo(pMOUSEMESSAGE mouse);
// update game phase process
void update_freephase(pMOUSEMESSAGE mouse);
void update_controlphase(pMOUSEMESSAGE mouse);
void update_layminephase(pMOUSEMESSAGE mouse);
void update_occupyphase(pMOUSEMESSAGE mouse);
void update_purchasephase(pMOUSEMESSAGE mouse);
void update_resultphase(pMOUSEMESSAGE mouse);
// update item
void update_matrix_mine();
void update_spread_mine();
void update_radar();
void update_robot(pMOUSEMESSAGE mouse);

// draw
// draw main ui
void draw_start_menu();
void draw_board();
void draw_gameinfo();
// draw basic element
void draw_text(RECT r, COLORREF color, const wchar_t* str);
void draw_button(RECT button, const wchar_t* str, int mode);
void draw_block(RECT block, int id, int keystatus, const STYLEGROUP blockstyle);
void draw_num_info(RECT button,const wchar_t* str,int num,int mode);
// draw phase ui
void draw_freephase();
void draw_controlphase();
void draw_layminephase();
void draw_occupyphase();
void draw_purchasephase();
void draw_resultphase();

void draw_mineinfo_block(int i, int j);

// judge
bool isselected();
bool is_item_enable(int cur_item);

#endif

const wchar_t font[] = L"Source Han Sans CN";

const RECT mid = { 750,350,1250,400 };
extern RECT first_block;

const STYLEGROUP blockstyle[] =
{
	{RGB(200,200,200),RGB(50,50,50),RGB(100,100,100),RGB(255,255,255),RGB(255,255,255),50}, // blank
	{RGB(69,152,161),RGB(96,236,237),RGB(59,203,209),RGB(70,252,249),RGB(128,210,215),50}, // 1
	{RGB(57,102,140),RGB(102,181,242),RGB(60,134,194),RGB(119,209,249),RGB(129,186,225),50}, // 2
	{RGB(153,123,86),RGB(236,180,101),RGB(192,128,61),RGB(255,196,76),RGB(217,186,130),50}, // 3
	{RGB(135,50,57),RGB(255,110,75),RGB(192,44,46),RGB(255,122,84),RGB(221,123,130),50}, // 4
	{RGB(102,86,138),RGB(191,159,244),RGB(127,98,179),RGB(222,184,255),RGB(194,173,226),50}, // 5
	{RGB(156,157,78),RGB(229,227,68),RGB(198,194,57),RGB(255,244,75),RGB(207,207,130),50}, // 6
	{RGB(63,131,101),RGB(107,218,104),RGB(66,180,122),RGB(97,244,170),RGB(127,205,173),50}, // 7
	{RGB(148,172,181),RGB(198,223,229),RGB(185,207,211),RGB(220,242,246),RGB(194,218,224),50}, // 8
	{RGB(150,112,128),RGB(255,217,234),RGB(222,157,172),RGB(255,238,253),RGB(225,183,200),50}, // 9
	{RGB(58,147,209),RGB(255,255,255),RGB(212,126,138),RGB(236,195,196),RGB(255,255,255),50}, // blue and red
	{RGB(185,222,201),RGB(60,149,102),RGB(242,230,206),RGB(229,183,81),RGB(255,255,255),50} // green and yellow
};

const wchar_t round_name[][10] =
{
	L"请等待",
	L"第一轮",
	L"第二轮",
	L"第三轮",
	L"第四轮",
	L"最终结算"
};

const wchar_t phase_name[][10] =
{
	L"加载中..",
	L"自由阶段",
	L"控制阶段",
	L"埋雷阶段",
	L"占领阶段",
	L"购买阶段",
	L"查看排名"
};

const wchar_t item_name[][10] =
{
	L"普通地雷",
	L"矩阵地雷",
	L"追击地雷",
	L"孢子地雷",
	L"净化地雷",
	L"排雷手",
	L"冲锋车",
	L"透视仪",
	L"扫雷机器人",
	L"探测雷达",
};