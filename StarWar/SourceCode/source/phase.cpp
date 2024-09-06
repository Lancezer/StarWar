#include "../include/starwar.h"
#include "../include/UI.h"

// public vars
RECT first_button = { 875,265,925,315 };
RECT stop_button = { 900,565,1100,615 };
RECT phase_area = { 720,150,1280,720 };
RECT prompt = { 750,175,1250,225 };
RECT first_item;

bool is_curplayer_finish = true;
static int stopbt = 0;
static int holding_button[10] = { 0 };
static int curplayer_button = 0; 
int item_seq[10] = { 0 }; // save item button itemID sequence, begin from index : 1
static int curitem_button = 0;
static int curplayer_item_num = 0;
int selected_item = 0;

wchar_t p[30] = { 0 }; // prompt text

// control phase vars
static int maxcoin = 0;
static int control_status = 0; // 0 - no control; 1 - continue; 2 - success; 3 - fall

// mine lay phase vars
static int laymine_status = 0; // 0 - control; num - player; -1 - next phase
static bool is_load_item = false;

// occupy phase
int occupyphase_status = 0; // 0 - control; num - player; -1 - next phase
int judge_occupy_status = 0; // 0 - blank; 1 - success; -1 - bloom
COORDINATE have_occupied[17] = { 0 };
int occupy_num = 0;
bool is_bloom_done = false;

// purchase phase
static int purchase_status = 0; // 0 - control; num - player; -1 - next phase
static int display_status = 0; // 0 - special mine; 1 - other items
RECT left_button = { 750,250,870,290 };
RECT right_button = { 880,250,1030,290 };
static int leftbt = 1, rightbt = 0;
static int bgnitem = matrix_mine, stopitem = mine_remover;

// result phase
static int result_status = 0; // 0 - game result; 1 - game rank
static bool is_cal = false;
static int sort_result[9] = { 1,2,3,4,5,6,7,8,9 }; // player sort sequence

// all phase vars reset
void reset_phase()
{
	last_select = { 0,0 };
	
	stopbt = 0;
	curplayer_button = 0;
	is_curplayer_finish = true;
	memset(holding_button, 0, sizeof(int) * 10);
	memset(item_seq, 0, sizeof(int) * 8);
	curitem_button = 0;
	curplayer_item_num = 0;
	selected_item = 0;

	maxcoin = 0;
	control_status = 0;
	controller = 0;

	laymine_status = 0;
	is_load_item = false;

	occupyphase_status = 0;
	judge_occupy_status = 0;
	memset(have_occupied, 0, sizeof(COORDINATE) * 17);
	occupy_num = 0;
	is_bloom_done = false;

	purchase_status = 0;
	display_status = 0;
	leftbt = 1, rightbt = 0;
	bgnitem = matrix_mine, stopitem = mine_remover;

	result_status = 0;
	is_cal = false;
	for (int i = 0; i < 9; i++)
	{
		sort_result[i] = i + 1;
	}

	// spread mine
}

// free phase
void update_freephase(pMOUSEMESSAGE mouse)
{
	if (!is_mouse_in_rect(mouse->pos, phase_area))
	{
		return;
	}

	if (is_mouse_in_rect(mouse->pos, stop_button))
	{
		stopbt = 1;
		if (mouse->is_down)
		{
			phase = (PHASE)((int)phase + 1);
			return;
		}
	}
	else
	{
		stopbt = 0;
	}
}

void draw_freephase()
{
	draw_button(stop_button, L"结束讨论", stopbt);
}

// control phase

void update_controlphase(pMOUSEMESSAGE mouse)
{
	curplayer_button = 0;
	if (!is_mouse_in_rect(mouse->pos, phase_area))
	{
		if (controller == 0)
		{
			control_status = 0;
		}
		else
		{
			control_status = 1;
		}
		return;
	}
	// select player
	wchar_t input[10] = { 0 };
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int tmpplayer = 0, tmpcoin = 0;
			RECT cur = { first_button.left + j * 100,
						first_button.top + i * 100,
						first_button.right + j * 100,
						first_button.bottom + i * 100 };
			if (is_mouse_in_rect(mouse->pos, cur))
			{
				curplayer_button = i * 3 + j + 1;
				if (mouse->is_down)
				{
					tmpplayer = curplayer_button;
					bool isconfirm = InputBox(input, 8, L"在此处输入金额",
												L"竞拍控制权",0,0,0,false);
					if (isconfirm && player_info[tmpplayer].isban == false)
					{
						int is_input = swscanf_s(input, L"%d", &tmpcoin);
						if (is_input && tmpcoin > maxcoin)
						{
							controller = tmpplayer;
							maxcoin = tmpcoin;
							control_status = 1;
						}
					}
				}
			}
		}
	}
	// stop sale
	if (controller == 0)
	{
		return;
	}
	if (is_mouse_in_rect(mouse->pos,stop_button))
	{
		stopbt = 1;
		if (mouse->is_down)
		{
			if (player_info[controller].coin < maxcoin)
			{
				player_info[controller].isban = true;
				maxcoin = 0;
				control_status = 3;
			}
			else
			{
				player_info[controller].coin -= maxcoin;
				control_status = 2;
			}
		}
	}
	else
	{
		stopbt = 0;
	}
}

void draw_controlphase()
{
	// select player
	if (control_status < 2)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				RECT cur = { first_button.left + j * 100,first_button.top + i * 100,
							first_button.right + j * 100,first_button.bottom + i * 100 };
				wchar_t num[10] = { 0 };
				wsprintf(num, L"%d", i * 3 + j + 1);
				if ((i * 3 + j + 1) == curplayer_button)
				{
					draw_button(cur, num, 1);
				}
				else
				{
					draw_button(cur, num, 0);
				}
			}
		}

		draw_button(stop_button, L"结束竞拍", stopbt);
	}
	// prompt
	if (control_status == 0)
	{
		wsprintf(p, L"当前无人出价");
		draw_text(prompt, RGB(0, 0, 0), p);
	}
	else if (control_status == 1)
	{
		wsprintf(p, L"当前 %d 号出价最高 为 %d 金币", controller,maxcoin);
		draw_text(prompt, RGB(0, 0, 0), p);
	}
	else if (control_status == 2)
	{
		wsprintf(p, L"%d 号玩家出价最高 获得控制权", controller);
		draw_text(mid, RGB(0, 0, 0), p);
		phase = (PHASE)((int)phase + 1);

		FlushBatchDraw();
		Sleep(2000);

		// set next phase var
		for (int i = 1; i < 10; i++)
		{
			player_info[i].item[normal_mine] = 1;
		}
	}
	else
	{
		wsprintf(p, L"%d 号无力支付 失去竞拍权", controller);
		draw_text(mid, RGB(0, 0, 0), p);
		controller = 0;
		control_status = 0;

		FlushBatchDraw();
		Sleep(2000);
	}
}

// mine lay phase
void update_layminephase(pMOUSEMESSAGE mouse)
{
	if (!is_mouse_in_rect(mouse->pos, phase_area))
	{
		curplayer_button = 0;
		return;
	}

	// finish lay mine, set vars for next phase
	if (laymine_status < 0)
	{
		memset(holding_button, 0, sizeof(int) * 10);
		is_curplayer_finish = true;
		// end communication
		if (is_mouse_in_rect(mouse->pos, stop_button))
		{
			stopbt = 1;
			if (mouse->is_down)
			{
				phase = (PHASE)((int)phase + 1);
				// add move point
				for (int i = 1; i < 10; i++)
				{
					player_info[i].point += 4;
				}
				return;
			}
		}
		else
		{
			stopbt = 0;
		}
	}
	else
	{
		// select player
		if (is_curplayer_finish)
		{
			curplayer_button = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					RECT cur = { first_button.left + j * 100,
								first_button.top + i * 100,
								first_button.right + j * 100,
								first_button.bottom + i * 100 };
					if (is_mouse_in_rect(mouse->pos, cur))
					{
						curplayer_button = i * 3 + j + 1;
						if (mouse->is_down && is_curplayer_finish
							&& holding_button[curplayer_button] == 0)
						{
							holding_button[curplayer] = 0;
							holding_button[curplayer_button] = 1;
							curplayer = curplayer_button;
						}
					}
				}
			}
			// confirm player
			if (is_mouse_in_rect(mouse->pos, stop_button) && curplayer)
			{
				stopbt = 1;
				if (mouse->is_down)
				{
					is_curplayer_finish = false;
					laymine_status = curplayer;
					is_load_item = false;
				}
			}
			else
			{
				stopbt = 0;
			}
		}
		else
		{
			// select mine
			// set vars
			curitem_button = 0; // curitem button : 1 - 6 item
			first_item = { 810,255,960,295 };
			// load player item
			if (!is_load_item)
			{
				curplayer_item_num = 0;
				for (int i = normal_mine; i < mine_remover; i++)
				{
					if (player_info[curplayer].item[i] != 0)
					{
						item_seq[++curplayer_item_num] = i;
					}
				}
				if (player_info[curplayer].item[radar] != 0)
				{
					item_seq[++curplayer_item_num] = radar;
				}
				is_load_item = true;
			}
			// select item
			if (!is_chase_mine_use)
			{
				for (int i = 1; i <= curplayer_item_num; i++)
				{
					RECT cur = { first_item.left,first_item.top + i * 55,
								first_item.right,first_item.bottom + i * 55 };
					if (is_mouse_in_rect(mouse->pos, cur))
					{
						curitem_button = i;
						if (mouse->is_down)
						{
							if ((curitem_button == selected_item 
								&& item_seq[selected_item] != radar)
								|| (item_seq[selected_item] == radar
									&& !is_radar_use))
							{
								selected_item = 0;
							}
							else if (player_info[curplayer].item[item_seq[curitem_button]] > 0
								&& is_item_enable(curitem_button))
							{
								selected_item = curitem_button;
							}
							last_select = { 0,0 };
						}
					}
				}
			}
			// mine lay
			if (is_mouse_in_rect(mouse->pos, stop_button)
				&& ((last_select.x && last_select.y) 
					|| (player_info[curplayer].item[normal_mine] == 0 && selected_item == 0)
					|| item_seq[selected_item] == radar))
			{
				stopbt = 1;
				if (mouse->is_down)
				{
					// confirm
					if (selected_item)
					{
						switch (item_seq[selected_item])
						{
						case normal_mine:
							board[last_select.y][last_select.x].
								nmmine[board[last_select.y][last_select.x].nmminenum++]
								= curplayer;
							break;
						case matrix_mine:
							mtmine_num += 1;
							mtmine_info[mtmine_num] = { last_select,curplayer };
							mtmine_info[mtmine_num].is_enable = true;
							for (int i = 0; i < 4; i++)
							{
								board[last_select.y + mtmine_direction[i][0]]
									[last_select.x + mtmine_direction[i][1]].
									spmine[matrix_mine] += 1;
							}
							break;
						case chase_mine:
							if (!is_chase_mine_use)
							{
								csmine_num += 1;
								board[last_select.y][last_select.x].spmine[chase_mine] += 1;
								csmine_info[csmine_num].pos = last_select;
								csmine_info[csmine_num].owner = curplayer;
								csmine_info[csmine_num].is_enable = true;
								is_chase_mine_use = true;
							}
							else
							{
								csmine_info[csmine_num].target = last_select;
								is_chase_mine_use = false;
							}
							break;
						case spread_mine:
							sdmine_num += 1;
							sdmine_info[sdmine_num] = { curplayer,last_select,false,true };
							board[last_select.y][last_select.x].spmine[spread_mine] += 1; // no coin process here
							board[last_select.y][last_select.x].
								nmmine[board[last_select.y][last_select.x].nmminenum++] 
								= curplayer; // coin process here
							break;
						case clear_mine:
							clmine_num += 1;
							clmine_info[clmine_num] = { curplayer,last_select,true };
							board[last_select.y][last_select.x].spmine[clear_mine] += 1;
							break;
						case radar:
							if (!is_radar_use)
							{
								is_radar_use = true;
							}
							else
							{
								is_radar_use = false;
							}
							break;
						default:
							break;
						}
						if (!is_chase_mine_use && !is_radar_use)
						{
							player_info[curplayer].item[item_seq[selected_item]] -= 1;
							selected_item = 0;
						}
					}
					// end mine lay
					else
					{
						is_curplayer_finish = true;
						curplayer = 0;
						curplayer_item_num = 0;
						laymine_status = 0;
						mtmine_select = { 0,0 };
						// next phase judge
						bool to_nextphase = true;
						for (int i = 1; i < 10; i++)
						{
							if (holding_button[i] == 0)
							{
								to_nextphase = false;
								break;
							}
						}
						if (to_nextphase)
						{
							laymine_status = -1;
						}
					}
					last_select = { 0,0 };
				}
			}
			else
			{
				stopbt = 0;
			}
		}
	}
}

void draw_layminephase()
{
	// select player
	if (laymine_status == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				RECT cur = { first_button.left + j * 100,first_button.top + i * 100,
							first_button.right + j * 100,first_button.bottom + i * 100 };
				wchar_t num[10] = { 0 };
				wsprintf(num, L"%d", i * 3 + j + 1);
				if ((i * 3 + j + 1) == curplayer_button || holding_button[i * 3 + j + 1])
				{
					draw_button(cur, num, 1);
				}
				else
				{
					draw_button(cur, num, 0);
				}
			}
		}
	}
	// select item
	else if (laymine_status > 0)
	{
		draw_button(first_item, L"可用道具", 1);
		for (int i = 1; i <= curplayer_item_num; i++)
		{
			RECT cur = { first_item.left,first_item.top + i * 55,
						first_item.right,first_item.bottom + i * 55 };
			if (i == selected_item || i == curitem_button
				|| player_info[curplayer].item[item_seq[i]] == 0 
				|| !is_item_enable(item_seq[i]))
			{
				draw_num_info(cur, item_name[item_seq[i]],
					player_info[curplayer].item[item_seq[i]], 1);
			}
			else
			{
				draw_num_info(cur, item_name[item_seq[i]],
					player_info[curplayer].item[item_seq[i]], 0);
			}
		}
	}
	// stop button
	if (laymine_status > 0)
	{
		if (item_seq[selected_item] == radar)
		{
			if (!is_radar_use)
			{
				draw_button(stop_button, L"确认使用", stopbt);
			}
			else
			{
				draw_button(stop_button, L"结束查看", stopbt);
			}
		}
		else if (is_chase_mine_use)
		{
			draw_button(stop_button, L"确认选择", stopbt);
		}
		else if (selected_item)
		{
			draw_button(stop_button, L"确认埋雷", stopbt);
		}
		else
		{
			draw_button(stop_button, L"结束埋雷", stopbt);
		}
	}
	else if (laymine_status == 0)
	{
		draw_button(stop_button, L"确认选择", stopbt);
	}
	else
	{
		draw_button(stop_button, L"结束讨论", stopbt);
	}
	// prompt
	if (laymine_status > 0)
	{
		wsprintf(p, L"请 %d 号玩家使用道具 / 埋雷", curplayer);
		draw_text(prompt, RGB(0, 0, 0), p);
		if (is_chase_mine_use)
		{
			RECT tmptip = { first_item.right + 20,first_item.top - 3,
							first_item.right + 270,first_item.bottom + 2 };
			wsprintf(p, L"请选择追击地雷目标点");
			draw_text(tmptip, RGB(0, 0, 0), p);
		}
	}
	else if (laymine_status == 0)
	{
		wsprintf(p, L"请 %d 号玩家决定下一埋雷玩家",controller);
		draw_text(prompt, RGB(0, 0, 0), p);
	}
	else
	{
		wsprintf(p, L"所有玩家埋雷完毕 进行自由交流");
		draw_text(mid, RGB(0, 0, 0), p);
	}
}

// occupy phase
void update_occupyphase(pMOUSEMESSAGE mouse)
{
	if (!is_mouse_in_rect(mouse->pos, phase_area))
	{
		curplayer_button = 0;
		return;
	}

	// finish occupy
	if (occupyphase_status < 0)
	{
		is_curplayer_finish = true;
		memset(holding_button, 0, sizeof(int) * 10);
		phase = (PHASE)((int)phase + 1);
		// tmp reset here, should be in select items part below
		selected_item = 0;
		Sleep(1000);
		return;
	}
	else
	{
		if (is_curplayer_finish)
		{
			// select player
			curplayer_button = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					RECT cur = { first_button.left + j * 100,
								first_button.top + i * 100,
								first_button.right + j * 100,
								first_button.bottom + i * 100 };
					if (is_mouse_in_rect(mouse->pos, cur))
					{
						curplayer_button = i * 3 + j + 1;
						if (mouse->is_down && is_curplayer_finish
							&& holding_button[curplayer_button] == 0)
						{
							holding_button[curplayer] = 0;
							holding_button[curplayer_button] = 1;
							curplayer = curplayer_button;
						}
					}
				}
			}
			// confirm player
			if (is_mouse_in_rect(mouse->pos, stop_button) && curplayer)
			{
				stopbt = 1;
				if (mouse->is_down)
				{
					is_curplayer_finish = false;
					occupyphase_status = curplayer;
					is_load_item = false;
				}
			}
			else
			{
				stopbt = 0;
			}
		}
		else
		{
			// set vars
			curitem_button = 0; // curitem button : 1 - 5 item
			first_item = { 810,255,960,295 };
			// load player item
			if (!is_load_item)
			{
				curplayer_item_num = 0;
				for (int i = mine_remover; i < radar; i++)
				{
					if (player_info[curplayer].item[i] != 0)
					{
						item_seq[++curplayer_item_num] = i;
					}
				}
				is_load_item = true;
			}
			// select item
			for (int i = 1; i <= curplayer_item_num; i++)
			{
				RECT cur = { first_item.left,
							first_item.top + i * 52 + 52,
							first_item.right,
							first_item.bottom + i * 52 + 52 };
				if (is_mouse_in_rect(mouse->pos, cur))
				{
					curitem_button = i;
					if (mouse->is_down && !is_mobfix_patrol_use)
					{
						if ((curitem_button == selected_item 
							&& item_seq[selected_item] != scanner
							&& item_seq[selected_item] != mine_remove_robot)
							|| (item_seq[selected_item] == scanner && !is_scanner_use)
							|| (item_seq[selected_item] == mine_remove_robot
								&& !is_robot_use))
						{
							selected_item = 0;
							if (!is_mine_remover_enable)
							{
								last_select = { 0,0 };
							}
						}
						else if (player_info[curplayer].item[item_seq[curitem_button]] > 0
							&& is_item_enable(item_seq[curitem_button]))
						{
							selected_item = curitem_button;
							if (!is_mine_remover_enable)
							{
								last_select = { 0,0 };
							}
						}
					}
				}
			}
			// scanner reset
			if (is_scanner_use && !is_select_reset)
			{
				last_select = { 0,0 };
				selected_item = 0;
				is_select_reset = true;
			}
			// confirm
			if (is_mouse_in_rect(mouse->pos, stop_button)
				&& is_curplayer_finish == false 
				&& ((last_select.x && last_select.y) 
					|| (occupy_num > 0 && item_seq[selected_item] != scanner)
					|| judge_occupy_status < 0
					//|| is_mobfix_patrol_use
					|| (item_seq[selected_item] == mine_remove_robot
						&& (!is_robot_use || (is_robot_use && cur_line)))))
			{
				stopbt = 1;
				if (mouse->is_down)
				{
					// mine remover
					if (is_mine_remover_enable && item_seq[selected_item] == mine_remover)
					{
						mine_remover_deal();
					}
					// mobfix patrol
					else if (occupy_num == 0 && item_seq[selected_item] == mobfix_patrol
						&& !is_mobfix_patrol_use)
					{
						is_mobfix_patrol_use = true;
						is_mobfix_patrol_bloom_done = false;
					}
					// scanner
					else if (item_seq[selected_item] == scanner && !is_scanner_use 
							&& player_info[curplayer].point)
					{
						is_scanner_use = true;
						cur_draw_block = last_select;
						return;
					}
					// mine remove robot
					else if (item_seq[selected_item] == mine_remove_robot && !is_robot_use)
					{
						is_robot_use = true;
						cur_line = 0;
						return;
					}
					else if (item_seq[selected_item] == mine_remove_robot
							&& is_robot_use && cur_line && is_robot_done)
					{
						for (int i = 1; i < 13; i++)
						{
							if (cur_line < 13 && board[i][cur_line].occupant == 0)
							{
								board[i][cur_line].keystatus = 6;
							}
							else if (cur_line > 12 && board[cur_line - 12][i].occupant == 0)
							{
								board[cur_line - 12][i].keystatus = 6;
							}
						}
						is_robot_done = false;
						return;
					}
					// judge
					if (last_select.x && last_select.y && player_info[curplayer].point > 0)
					{
						judge_occupy();
						if (!is_mine_remover_enable)
						{
							last_select = { 0,0 };
							if (!is_mobfix_patrol_use)
							{
								player_info[curplayer].point -= 1;
							}
							if (judge_occupy_status < 0
								&& item_seq[selected_item] == mobfix_patrol)
							{
								selected_item = 0;
								judge_occupy_status = 0;
								is_mobfix_patrol_use = false;
								if (player_info[curplayer].item[mobfix_patrol] == 0
									|| mobfix_move == player_info[curplayer].point)
								{
									is_mobfix_patrol_enable = false;
								}
							}
							if (is_scanner_use && is_select_reset)
							{
								is_scanner_use = false;
								is_select_reset = false;
							}
						}
					}
					else if (occupy_num > 0 || judge_occupy_status < 0)
					{
						is_curplayer_finish = true;
						curplayer = 0;
						last_select = { 0,0 };
						selected_item = 0;
						occupyphase_status = 0;
						judge_occupy_status = 0;
						memset(have_occupied, 0, sizeof(COORDINATE) * 17);
						occupy_num = 0;
						is_bloom_done = false;
						is_load_item = false;
						is_mobfix_patrol_enable = true;
						is_mobfix_patrol_bloom_done = true;
						mobfix_move = 0;
						mobfix_begin = 0;
						cur_draw_block = { 0,0 };
						is_select_reset = false;
						is_scanner_use = false;
						is_robot_use = false;
						cur_line = 0;
						is_robot_done = true;
						cur_draw_block = { 0,0 };
						// next phase judge
						bool to_nextphase = true;
						for (int i = 1; i < 10; i++)
						{
							if (holding_button[i] == 0)
							{
								to_nextphase = false;
								break;
							}
						}
						if (to_nextphase)
						{
							occupyphase_status = -1;
						}
					}
					// mobfix patrol
					if (is_mobfix_patrol_use && mobfix_move == player_info[curplayer].point)
					{
						is_mobfix_patrol_use = false;
						is_mobfix_patrol_bloom_done = true;
						is_mobfix_patrol_enable = false;
						selected_item = 0;
						occupy_num = 0;
					}
				}
			}
			else
			{
				stopbt = 0;
			}
		}
	}
}

void draw_occupyphase()
{
	if (occupyphase_status == 0)
	{
		// select player
		if (occupyphase_status == 0)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					RECT cur = { first_button.left + j * 100,first_button.top + i * 100,
								first_button.right + j * 100,first_button.bottom + i * 100 };
					wchar_t num[10] = { 0 };
					wsprintf(num, L"%d", i * 3 + j + 1);
					if ((i * 3 + j + 1) == curplayer_button || holding_button[i * 3 + j + 1])
					{
						draw_button(cur, num, 1);
					}
					else
					{
						draw_button(cur, num, 0);
					}
				}
			}
		}
	}
	// select item and move
	else if (occupyphase_status > 0)
	{
		draw_num_info(first_item, L"剩余行动点", player_info[curplayer].point, 1);
		for (int i = 0; i <= curplayer_item_num; i++)
		{
			RECT cur = { first_item.left,first_item.top + i * 52 + 52,
						first_item.right,first_item.bottom + i * 52 + 52};
			if (i == 0)
			{
				draw_button(cur, L"可用道具", 1);
			}
			if (curplayer_item_num == 0)
			{
				RECT tmp = { mid.left,mid.top + 50,mid.right,mid.bottom + 50 };
				draw_text(tmp, RGB(0, 0, 0), L"暂无道具");
			}
			else if (i > 0)
			{
				if (i == selected_item || i == curitem_button
					|| player_info[curplayer].item[item_seq[i]] == 0
					|| !is_item_enable(item_seq[i]))
				{
					draw_num_info(cur, item_name[item_seq[i]],
						player_info[curplayer].item[item_seq[i]], 1);
				}
				else
				{
					draw_num_info(cur, item_name[item_seq[i]],
						player_info[curplayer].item[item_seq[i]], 0);
				}
			}
		}
	}
	// stop button
	if (occupyphase_status == 0)
	{
		draw_button(stop_button, L"确认选择", stopbt);
	}
	else if (occupyphase_status > 0)
	{
		if (is_mobfix_patrol_use)
		{
			draw_button(stop_button, L"确认冲锋", stopbt);
		}
		else if (is_robot_use)
		{
			draw_button(stop_button, L"确认扫雷", stopbt);
		}
		else if (selected_item && !is_select_reset)
		{
			draw_button(stop_button, L"确认使用", stopbt);
		}
		else if (last_select.x && last_select.y)
		{
			draw_button(stop_button, L"确认占领", stopbt);
		}
		else
		{
			draw_button(stop_button, L"结束占领", stopbt);
		}
	}
	// prompt
	if (occupyphase_status > 0)
	{
		wsprintf(p, L"请 %d 号玩家使用道具 / 进行占领", curplayer);
		draw_text(prompt, RGB(0, 0, 0), p);
		if (is_mine_remover_enable)
		{
			RECT tmptip = { first_item.right + 20,first_item.top - 3 + 52,
							first_item.right + 270,first_item.bottom + 2 + 52 };
			wsprintf(p, L"该格仅有普通地雷");
			draw_text(tmptip, RGB(0, 0, 0), p);
			tmptip = { first_item.right + 120,first_item.top - 3 + 52 * 2,
							first_item.right + 270,first_item.bottom + 2 + 52 * 2 };
			wsprintf(p, L"可选择使用");
			draw_text(tmptip, RGB(0, 0, 0), p);
		}
		else if (!is_mobfix_patrol_enable)
		{
			RECT tmptip = { first_item.right + 120,first_item.top - 3 + 52 * 2,
				first_item.right + 270,first_item.bottom + 2 + 52 * 2 };
			wsprintf(p, L"已使用完毕");
			draw_text(tmptip, RGB(0, 0, 0), p);
		}
	}
	else if (occupyphase_status == 0)
	{
		wsprintf(p, L"请 %d 号玩家决定下一占领玩家", controller);
		draw_text(prompt, RGB(0, 0, 0), p);
	}
	else
	{
		wsprintf(p, L"所有玩家占领完毕"/* 进入购买阶段*/);
		draw_text(mid, RGB(0, 0, 0), p);
	}
}

void judge_occupy()
{
	// jugde special mine
	for (int i = 1; i < 5; i++)
	{
		if (board[last_select.y][last_select.x].spmine[i] != 0)
		{
			judge_occupy_status = -1;
			break;
		}
	}
	// mine remover judge
	if (judge_occupy_status > 0 && board[last_select.y][last_select.x].nmminenum 
		&& !is_mine_remover_enable && player_info[curplayer].item[mine_remover])
	{
		is_mine_remover_enable = true;
		return;
	}
	// occupy fall
	if (judge_occupy_status == -1 || board[last_select.y][last_select.x].nmminenum)
	{
		judge_occupy_status = -1;
		// spmine deal
		// clear mine
		clear_mine_deal();
		// matrix mine
		matrix_mine_deal();
		// chase mine
		chase_mine_deal();
		// spread mine
		spread_mine_deal();
		// nmmine deal
		normal_mine_deal();
		// mobfix patrol deal
		board[last_select.y][last_select.x].keystatus = 2;
		if (is_mobfix_patrol_use)
		{
			player_info[curplayer].item[mobfix_patrol] -= 1;
			mobfix_begin = mobfix_move;
		}
		// remove have occupied block
		else
		{
			board[last_select.y][last_select.x].occupant = 0;
			is_mine_remover_enable = false;
			for (int i = 0; i < occupy_num; i++)
			{
				board[have_occupied[i].y][have_occupied[i].x].occupant = 0;
				board[have_occupied[i].y][have_occupied[i].x].keystatus = 1;
			}
			occupy_num = 0;
		}
	}
	// occupy success
	else if (board[last_select.y][last_select.x].nmminenum == 0 
		&& board[last_select.y][last_select.x].occupant == 0
		&& player_info[curplayer].point >= 0)
	{
		// var update
		judge_occupy_status = 1;
		// mobfix patrol deal
		if (is_mobfix_patrol_use)
		{
			board[last_select.y][last_select.x].keystatus = 4;
			have_occupied[mobfix_move - mobfix_begin] = last_select;
			mobfix_move += 1;
		}
		else
		{
			board[last_select.y][last_select.x].occupant = curplayer;
			board[last_select.y][last_select.x].keystatus = 0;
			// save one round have occupied 
			have_occupied[occupy_num++] = last_select;
		}
	}
}

// purchase phase

void update_purchasephase(pMOUSEMESSAGE mouse)
{
	if (!is_mouse_in_rect(mouse->pos, phase_area))
	{
		curplayer_button = 0;
		return;
	}

	// finish purchase
	if (purchase_status < 0)
	{
		phase = (PHASE)((int)phase + 1);
		Sleep(1000);
		return;
	}
	else
	{
		// select player
		if (is_curplayer_finish)
		{
			curplayer_button = 0;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					RECT cur = { first_button.left + j * 100,
								first_button.top + i * 100,
								first_button.right + j * 100,
								first_button.bottom + i * 100 };
					if (is_mouse_in_rect(mouse->pos, cur))
					{
						curplayer_button = i * 3 + j + 1;
						if (mouse->is_down && is_curplayer_finish
							&& holding_button[curplayer_button] == 0)
						{
							holding_button[curplayer] = 0;
							holding_button[curplayer_button] = 1;
							curplayer = curplayer_button;
						}
					}
				}
			}
			// confirm player
			if (is_mouse_in_rect(mouse->pos, stop_button) && curplayer)
			{
				stopbt = 1;
				if (mouse->is_down)
				{
					is_curplayer_finish = false;
					purchase_status = curplayer;
				}
			}
			else
			{
				stopbt = 0;
			}
		}
		else
		{
			// judge display status
			if (is_mouse_in_rect(mouse->pos, left_button))
			{
				leftbt = 1;
				if (mouse->is_down)
				{
					display_status = 0;
					selected_item = 0;
				}
			}
			else
			{
				leftbt = 0;
			}
			if (is_mouse_in_rect(mouse->pos, right_button))
			{
				rightbt = 1;
				if (mouse->is_down)
				{
					display_status = 1;
					selected_item = 0;
				}
			}
			else
			{
				rightbt = 0;
			}
			// set vars
			curitem_button = 0; // curitem button : 1 - 4 / 5 item
			// special mine
			if (!display_status) 
			{
				first_item = { 810,335,960,373 };
				bgnitem = matrix_mine, stopitem = mine_remover;
			}
			// other items
			else 
			{
				first_item = { 810,310,960,348 };
				bgnitem = mine_remover, stopitem = radar + 1;
			}
			// select item
			for (int i = bgnitem; i < stopitem; i++)
			{
				RECT cur = { first_item.left,
							first_item.top + (i - bgnitem) * 48,
							first_item.right,
							first_item.bottom + (i - bgnitem) * 48 };
				if (is_mouse_in_rect(mouse->pos, cur))
				{
					curitem_button = i;
					if (mouse->is_down && item_info[curitem_button].left_num)
					{
						if (curitem_button == selected_item)
						{
							selected_item = 0;
						}
						else if (player_info[curplayer].coin >= item_info[curitem_button].price)
						{
							selected_item = curitem_button;
						}
					}
				}
			}
			// end purchase
			if (is_mouse_in_rect(mouse->pos, stop_button))
			{
				stopbt = 1;
				if (mouse->is_down)
				{
					// purchase
					if (selected_item)
					{
						player_info[curplayer].coin -= item_info[selected_item].price;
						player_info[curplayer].item[selected_item] += 1;
						item_info[selected_item].left_num -= 1;
					}
					// set vars
					purchase_status = 0;
					is_curplayer_finish = true;
					curplayer = 0;
					selected_item = 0;
					display_status = 0;
					// next phase judge
					bool to_nextphase = true;
					for (int i = 1; i < 10; i++)
					{
						if (holding_button[i] == 0)
						{
							to_nextphase = false;
							break;
						}
					}
					if (to_nextphase)
					{
						purchase_status = -1;
					}

				}
			}
			else
			{
				stopbt = 0;
			}
		}
	}
}

void draw_purchasephase()
{
	// select player
	if (purchase_status == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				RECT cur = { first_button.left + j * 100,first_button.top + i * 100,
							first_button.right + j * 100,first_button.bottom + i * 100 };
				wchar_t num[10] = { 0 };
				wsprintf(num, L"%d", i * 3 + j + 1);
				if ((i * 3 + j + 1) == curplayer_button || holding_button[i * 3 + j + 1])
				{
					draw_button(cur, num, 1);
				}
				else
				{
					draw_button(cur, num, 0);
				}
			}
		}
	}
	else if (purchase_status > 0)
	{
		// type choose
		if (leftbt || !display_status)
		{
			draw_button(left_button, L"特殊地雷", 1);
		}
		else
		{
			draw_button(left_button, L"特殊地雷", 0);
		}
		if (rightbt || display_status)
		{
			draw_button(right_button, L"反地雷装备", 1);
		}
		else
		{
			draw_button(right_button, L"反地雷装备", 0);
		}
		// left coin
		RECT coin = { right_button.right + 10,right_button.top,
					right_button.right + 130,right_button.bottom };
		draw_num_info(coin, L"剩余金币", player_info[curplayer].coin, 1);
		// select item
		for (int i = bgnitem; i < stopitem; i++)
		{
			RECT cur = { first_item.left,
						first_item.top + (i - bgnitem) * 48,
						first_item.right,
						first_item.bottom + (i - bgnitem) * 48 };
			RECT tmptext = { cur.right + 20,cur.top - 5,cur.right + 270,cur.bottom + 5 };
			if (i == selected_item || i == curitem_button || item_info[i].left_num == 0
				|| player_info[curplayer].coin < item_info[i].price)
			{
				draw_button(cur, item_name[i], 1);
			}
			else
			{
				draw_button(cur, item_name[i], 0);
			}
			wsprintf(p, L" 余 %d / %d    %d 金币",
					item_info[i].left_num,item_info[i].total_num,item_info[i].price);
			draw_text(tmptext, RGB(0, 0, 0), p);
		}
	}
	// stop button
	if (purchase_status > 0)
	{
		if (selected_item)
		{
			draw_button(stop_button, L"确认购买", stopbt);
		}
		else
		{
			draw_button(stop_button, L"结束购买", stopbt);
		}
	}
	else if (purchase_status == 0)
	{
		draw_button(stop_button, L"确认选择", stopbt);
	}
	// prompt
	if (purchase_status > 0)
	{
		wsprintf(p, L"请 %d 号玩家查看余量 / 购买道具", curplayer);
		draw_text(prompt, RGB(0, 0, 0), p);
	}
	else if (purchase_status == 0)
	{
		wsprintf(p, L"请 %d 号玩家决定下一购买玩家", controller);
		draw_text(prompt, RGB(0, 0, 0), p);
	}
	else
	{
		wsprintf(p, L"所有玩家购买完毕 进入下一轮");
		draw_text(mid, RGB(0, 0, 0), p);
	}
}

// result phase
void update_resultphase(pMOUSEMESSAGE mouse)
{
	if (is_mouse_in_rect(mouse->pos, stop_button))
	{
		stopbt = 1;
		if (mouse->is_down)
		{
			if (result_status == 0)
			{
				result_status = 1;
				// final calculate
				for (int i = 0; i < 9; i++)
				{
					player_rank[sort_result[i]].finalnum = player_rank[sort_result[i]].ocpnum;
					if (player_rank[sort_result[i]].rank == 1)
					{
						player_rank[sort_result[i]].finalnum += 3;
					}
					if (player_rank[sort_result[i]].rank == 2)
					{
						player_rank[sort_result[i]].finalnum += 1;
					}
				}
				qsort(sort_result, 9, sizeof(int), cmp2); // final rank
				int currank = 1;
				for (int i = 0; i < 9; i++)
				{
					if (i > 0 && (player_rank[sort_result[i]].finalnum
						!= player_rank[sort_result[i - 1]].finalnum
						|| (player_rank[sort_result[i]].finalnum 
							== player_rank[sort_result[i - 1]].finalnum
							&& player_info[sort_result[i - 1]].coin 
							!= player_info[sort_result[i]].coin)))
					{
						currank = i + 1;
					}
					player_rank[sort_result[i]].rank = currank;
				}
			}
			else if (result_status)
			{
				phase = (PHASE)((int)phase + 1);
				return;
			}
		}
	}
	else
	{
		stopbt = 0;
	}
	// calculate result
	if (!is_cal)
	{
		for (int i = 1; i < 13; i++)
		{
			for (int j = 1; j < 13; j++)
			{
				board[i][j].is_passed = false;
				if (board[i][j].occupant)
				{
					player_rank[board[i][j].occupant].ocpnum += 1;
				}
			}
		}
		for (int i = 1; i < 13; i++)
		{
			for (int j = 1; j < 13; j++)
			{
				if (board[i][j].occupant && !board[i][j].is_passed)
				{
					int tmplink = 0;
					tmplink = dfs_link(tmplink,i,j, board[i][j].occupant);
					if (tmplink > player_rank[board[i][j].occupant].maxlink)
					{
						player_rank[board[i][j].occupant].maxlink = tmplink;
					}
				}
			}
		}
		qsort(sort_result, 9, sizeof(int), cmp1); // sort maxlink
		int currank = 1;
		for (int i = 0; i < 9; i++)
		{
			if (i > 0 && player_rank[sort_result[i]].maxlink
				!= player_rank[sort_result[i - 1]].maxlink)
			{
				currank += 1;
			}
			player_rank[sort_result[i]].rank = currank;
		}
		is_cal = true;
	}
}

int rect_left = 750, rect_top = 160, rect_bottom = 195;
int column[][8] = { {0,80,110,270,300,500}, {0,80,100,180,200,360,380,500} };

void draw_resultphase()
{
	if (!is_cal)
	{
		return;
	}
	if (result_status == 0)
	{
		// backgroud
		for (int i = 0; i < 10; i++)
		{
			RECT plus = { rect_left + 200,rect_top + i * 40,
				rect_left + 300,rect_bottom + i * 40 };
			if (i > 0 && player_rank[i].rank < 3)
			{
				setfillcolor(blockstyle[10].bkcolor1 
					+ RGB(15 * (player_rank[i].rank - 1),15 * (player_rank[i].rank - 1),
						15 * (player_rank[i].rank - 1)));
				setfillstyle(BS_SOLID);
				solidrectangle(rect_left + column[0][0], rect_top + i * 40 - 2,
					rect_left + column[0][5], rect_bottom + i * 40 + 3);
				if (player_rank[i].rank == 1)
				{
					draw_text(plus, RGB(255, 255, 255), L"+3");
				}
				else if (player_rank[i].rank == 2)
				{
					draw_text(plus, RGB(255, 255, 255), L"+1");
				}
			}
			else if (i % 2 == 1)
			{
				setfillcolor(RGB(240, 240, 240));
				setfillstyle(BS_SOLID);
				solidrectangle(rect_left + column[0][0], rect_top + i * 40 - 2,
								rect_left + column[0][5], rect_bottom + i * 40 + 3);
			}
		}
		// player
		for (int i = 0; i < 10; i++)
		{
			RECT cur = { rect_left + column[0][0],rect_top + i * 40,
						rect_left + column[0][1],rect_bottom + i * 40};
			if (i == 0)
			{
				wsprintf(p, L"玩家");
			}
			else
			{
				wsprintf(p, L"%d", i);
			}
			if (i > 0 && player_rank[i].rank < 3)
			{
				draw_text(cur, RGB(255,255,255), p);
			}
			else
			{
				draw_text(cur, RGB(0, 0, 0), p);
			}
		}
		// occupy num
		for (int i = 0; i < 10; i++)
		{
			RECT cur = { rect_left + column[0][2],rect_top + i * 40,
						rect_left + column[0][3],rect_bottom + i * 40 };
			if (i == 0)
			{
				wsprintf(p, L"占领格数");
			}
			else
			{
				wsprintf(p, L"%d", player_rank[i].ocpnum);
			}
			if (i > 0 && player_rank[i].rank < 3)
			{
				draw_text(cur, RGB(255, 255, 255), p);
			}
			else
			{
				draw_text(cur, RGB(0, 0, 0), p);
			}
		}
		// maxlink
		for (int i = 0; i < 10; i++)
		{
			RECT cur = { rect_left + column[0][4],rect_top + i * 40,
						rect_left + column[0][5],rect_bottom + i * 40 };
			if (i == 0)
			{
				wsprintf(p, L"最大连接数");
			}
			else
			{
				wsprintf(p, L"%d", player_rank[i].maxlink);
			}
			if (i > 0 && player_rank[i].rank < 3)
			{
				draw_text(cur, RGB(255, 255, 255), p);
			}
			else
			{
				draw_text(cur, RGB(0, 0, 0), p);
			}
		}

		draw_button(stop_button, L"查看排名", stopbt);
	}
	else if (result_status)
	{
		// backgroud
		for (int i = 0; i < 10; i++)
		{
			if (i > 0 && player_rank[sort_result[i - 1]].rank < 4)
			{
				setfillcolor(blockstyle[10].bkcolor1 
							+ RGB(15 * (player_rank[sort_result[i - 1]].rank - 1),
							15 * (player_rank[sort_result[i - 1]].rank - 1),
							15 * (player_rank[sort_result[i - 1]].rank - 1)));
				setfillstyle(BS_SOLID);
				solidrectangle(rect_left + column[1][0], rect_top + i * 40 - 2,
					rect_left + column[1][7], rect_bottom + i * 40 + 3);

			}
			else if (i % 2 == 1)
			{
				setfillcolor(RGB(240, 240, 240));
				setfillstyle(BS_SOLID);
				solidrectangle(rect_left + column[1][0], rect_top + i * 40 - 2,
					rect_left + column[1][7], rect_bottom + i * 40 + 3);
			}
		}		
		// rank
		for (int i = 0; i < 10; i++)
		{
			RECT cur = { rect_left + column[1][0],rect_top + i * 40,
						rect_left + column[1][1],rect_bottom + i * 40 };
			if (i == 0)
			{
				wsprintf(p, L"名次");
			}
			else
			{
				wsprintf(p, L"%d", player_rank[sort_result[i - 1]].rank);
			}
			if (i > 0 && player_rank[sort_result[i - 1]].rank < 4)
			{
				draw_text(cur, RGB(255, 255, 255), p);
			}
			else
			{
				draw_text(cur, RGB(0, 0, 0), p);
			}
		}
		// player
		for (int i = 0; i < 10; i++)
		{
			RECT cur = { rect_left + column[1][2],rect_top + i * 40,
						rect_left + column[1][3],rect_bottom + i * 40 };
			if (i == 0)
			{
				wsprintf(p, L"玩家");
			}
			else
			{
				wsprintf(p, L"%d", sort_result[i - 1]);
			}
			if (i > 0 && player_rank[sort_result[i - 1]].rank < 4)
			{
				draw_text(cur, RGB(255, 255, 255), p);
			}
			else
			{
				draw_text(cur, RGB(0, 0, 0), p);
			}
		}
		// final num
		for (int i = 0; i < 10; i++)
		{
			RECT cur = { rect_left + column[1][4],rect_top + i * 40,
						rect_left + column[1][5],rect_bottom + i * 40 };
			if (i == 0)
			{
				wsprintf(p, L"总占领格");
			}
			else
			{
				wsprintf(p, L"%d", player_rank[sort_result[i - 1]].finalnum);
			}
			if (i > 0 && player_rank[sort_result[i - 1]].rank < 4)
			{
				draw_text(cur, RGB(255, 255, 255), p);
			}
			else
			{
				draw_text(cur, RGB(0, 0, 0), p);
			}
		}
		// coin
		for (int i = 0; i < 10; i++)
		{
			RECT cur = { rect_left + column[1][6],rect_top + i * 40,
						rect_left + column[1][7],rect_bottom + i * 40 };
			if (i == 0)
			{
				wsprintf(p, L"金币数");
			}
			else
			{
				wsprintf(p, L"%d", player_info[sort_result[i - 1]].coin);
			}
			if (i > 0 && player_rank[sort_result[i - 1]].rank < 4)
			{
				draw_text(cur, RGB(255, 255, 255), p);
			}
			else
			{
				draw_text(cur, RGB(0, 0, 0), p);
			}
		}
		draw_button(stop_button, L"结束游戏", stopbt);
	}
}

int dfs_link(int curlink,int i,int j,int id)
{
	if (i == 0 || j == 0 || i > 12 || j > 12 || board[i][j].is_passed)
	{
		return curlink;
	}
	if (board[i][j].occupant != id)
	{
		return curlink;
	}
	board[i][j].is_passed = true;
	curlink += 1;
	curlink = dfs_link(curlink, i - 1, j, id); // up
	curlink = dfs_link(curlink, i, j - 1, id); // left
	curlink = dfs_link(curlink, i + 1, j, id); // down
	curlink = dfs_link(curlink, i, j + 1, id); // right
	return curlink;
}

int cmp1(const void* e1, const void* e2)
{
	if (player_rank[*(int*)e2].maxlink != player_rank[*(int*)e1].maxlink)
	{
		return (player_rank[*(int*)e2].maxlink - player_rank[*(int*)e1].maxlink);
	}
	return (*(int*)e1 - *(int*)e2);
}

int cmp2(const void* e1, const void* e2)
{
	if (player_rank[*(int*)e2].finalnum != player_rank[*(int*)e1].finalnum)
	{
		return (player_rank[*(int*)e2].finalnum - player_rank[*(int*)e1].finalnum);
	}
	if (player_info[*(int*)e2].coin != player_info[*(int*)e1].coin)
	{
		return (player_info[*(int*)e2].coin - player_info[*(int*)e1].coin);
	}
	return (*(int*)e1 - *(int*)e2);
}