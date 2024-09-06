#include "../include/UI.h"
#include "../include/starwar.h"

COORDINATE mtmine_select = { 0,0 };

bool is_mine_remover_enable = false;
bool is_chase_mine_use = false;
bool is_clear_mine_use = false;
bool is_radar_use = false;
bool is_mine_remover_use = false;
bool is_mobfix_patrol_use = false;
bool is_mobfix_patrol_enable = true;
bool is_mobfix_patrol_bloom_done = true;
int mobfix_move = 0, mobfix_begin = 0;
bool is_scanner_use = false;
bool is_select_reset = false;
COORDINATE cur_draw_block = { 0,0 };
bool is_robot_use = false;
bool is_robot_done = true;
int cur_line = 0;
int indx2[25] = { 0 };

void reset_item()
{
	is_mine_remover_enable = false;
	mtmine_select = { 0,0 };
	is_chase_mine_use = false;
	is_clear_mine_use = false;
	is_radar_use = false;
	is_mobfix_patrol_use = false;
	is_mobfix_patrol_enable = true;
	is_mobfix_patrol_bloom_done = true;
	mobfix_move = 0, mobfix_begin = 0;
	is_scanner_use = false;
	cur_draw_block = { 0,0 };
	is_select_reset = false;
	is_robot_use = false;
	cur_line = 0;
	is_robot_done = true;
}

bool is_item_enable(int cur_item)
{
	if (judge_occupy_status < 0 || is_radar_use)
	{
		return false;
	}
	switch (cur_item)
	{
	case normal_mine: case matrix_mine: case chase_mine: 
	case spread_mine: case clear_mine: case radar:
		return true;
		break;
	case mine_remover:
		if (is_mine_remover_enable && !is_mine_remover_use)
		{
			return true;
		}
		return false;
		break;
	case mobfix_patrol: case mine_remove_robot:
		if (occupy_num || is_mine_remover_enable || !is_mobfix_patrol_enable)
		{
			return false;
		}
		return true;
		break;
	case scanner:
		if (occupy_num && !is_mine_remover_enable && !is_select_reset
			&& player_info[curplayer].point)
		{
			return true;
		}
		return false;
		break;
	default:
		return false;
		break;
	}
}

void normal_mine_deal()
{
	if (!is_mine_remover_use && !is_robot_use && !is_clear_mine_use)
	{
		for (int i = 0; i < board[last_select.y][last_select.x].nmminenum; i++)
		{
			player_info[board[last_select.y][last_select.x].nmmine[i]].coin += 10;
		}
	}
	board[last_select.y][last_select.x].nmminenum = 0;
}

void update_matrix_mine()
{
	if ((mtmine_select.x && mtmine_select.y) 
		|| last_select.x == 0 || last_select.y == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			if (board[mtmine_select.y + mtmine_direction[i][0]]
				[mtmine_select.x + mtmine_direction[i][1]].occupant == 0)
			{
				board[mtmine_select.y + mtmine_direction[i][0]]
					[mtmine_select.x + mtmine_direction[i][1]].keystatus = 0;
			}
		}
	}
	mtmine_select = last_select;
	if (last_select.x && last_select.y)
	{
		if (last_select.y == 12)
		{
			last_select.y = 11;
		}
		if (last_select.x == 12)
		{
			last_select.x = 11;
		}
		for (int i = 0; i < 4; i++)
		{
			if (board[last_select.y + mtmine_direction[i][0]]
				[last_select.x + mtmine_direction[i][1]].occupant == 0)
			{
				board[last_select.y + mtmine_direction[i][0]]
					[last_select.x + mtmine_direction[i][1]].keystatus = 3;
			}
		}
	}
}

void matrix_mine_deal()
{
	for (int i = 1; i <= mtmine_num; i++)
	{
		// find owner
		if (last_select.x - mtmine_info[i].pos.x >= 0
			&& last_select.x - mtmine_info[i].pos.x < 2
			&& last_select.y - mtmine_info[i].pos.y >= 0
			&& last_select.y - mtmine_info[i].pos.y < 2
			&& mtmine_info[i].is_enable)
		{
			for (int j = 0; j < 4; j++)
			{
				if (!board[mtmine_info[i].pos.y + mtmine_direction[j][0]]
					[mtmine_info[i].pos.x + mtmine_direction[j][1]].occupant)
				{
					board[mtmine_info[i].pos.y + mtmine_direction[j][0]]
						[mtmine_info[i].pos.x + mtmine_direction[j][1]].spmine[matrix_mine] -= 1;
					board[mtmine_info[i].pos.y + mtmine_direction[j][0]]
						[mtmine_info[i].pos.x + mtmine_direction[j][1]].keystatus = 2;
				}
			}
			if (!is_clear_mine_use)
			{
				player_info[mtmine_info[i].owner].coin += 10;
			}
			mtmine_info[i].is_enable = false;
		}
	}
}

void chase_mine_deal()
{
	for (int i = 1; i <= csmine_num; i++)
	{
		if (last_select.x == csmine_info[i].pos.x
			&& last_select.y == csmine_info[i].pos.y
			&& csmine_info[i].is_enable)
		{
			board[last_select.y][last_select.x].spmine[chase_mine] -= 1;
			board[last_select.y][last_select.x].keystatus = 2;
			if (!is_clear_mine_use)
			{
				player_info[csmine_info[i].owner].coin += 10;
				board[csmine_info[i].target.y][csmine_info[i].target.x].occupant = 0;
				board[csmine_info[i].target.y][csmine_info[i].target.x].keystatus = 2;
			}
			csmine_info[i].is_enable = false;
		}
	}
}

void update_spread_mine()
{
	for (int i = 1; i <= sdmine_num; i++)
	{
		if (sdmine_info[i].is_enable && !sdmine_info[i].issprd)
		{
			for (int j = 0; j < 8; j++)
			{
				if (sdmine_info[i].pos.y + all_direction[j][0] > 0
					&& sdmine_info[i].pos.y + all_direction[j][0] < 13
					&& sdmine_info[i].pos.x + all_direction[j][1] > 0
					&& sdmine_info[i].pos.x + all_direction[j][1] < 13
					&& !board[sdmine_info[i].pos.y + all_direction[j][0]]
					[sdmine_info[i].pos.x + all_direction[j][1]].occupant)
				{
					board[sdmine_info[i].pos.y + all_direction[j][0]]
						[sdmine_info[i].pos.x + all_direction[j][1]].
						nmmine[board[sdmine_info[i].pos.y + all_direction[j][0]]
						[sdmine_info[i].pos.x + all_direction[j][1]].nmminenum++]
						= sdmine_info[i].owner;
				}
			}
			sdmine_info[i].issprd = true;
			board[sdmine_info[i].pos.y][sdmine_info[i].pos.x].
				spmine[spread_mine] -= 1;
			sdmine_info[i].is_enable = false;
		}
	}
}

void spread_mine_deal()
{
	for (int i = 1; i <= sdmine_num; i++)
	{
		if (last_select.x == sdmine_info[i].pos.x
			&& last_select.y == sdmine_info[i].pos.y
			&& sdmine_info[i].is_enable)
		{
			board[last_select.y][last_select.x].spmine[spread_mine] -= 1;
			sdmine_info[i].is_enable = false;
		}
	}
}

void clear_mine_deal()
{
	for (int i = 1; i <= clmine_num; i++)
	{
		if (last_select.x == clmine_info[i].pos.x
			&& last_select.y == clmine_info[i].pos.y
			&& clmine_info[i].is_enable)
		{
			is_clear_mine_use = true;
			COORDINATE tmpsave = last_select;
			// around 8 blocks
			for (int j = 0; j < 8; j++)
			{
				last_select = { tmpsave.x + all_direction[j][1],
								tmpsave.y + all_direction[j][0] };
				// special mine
				matrix_mine_deal();
				chase_mine_deal();
				spread_mine_deal();
				board[last_select.y][last_select.x].spmine[clear_mine] = 0;
				// normal mine
				board[last_select.y][last_select.x].nmminenum = 0;
				// occupant
				board[last_select.y][last_select.x].occupant = 0;
				board[last_select.y][last_select.x].keystatus = 2;
			}
			// itself
			last_select = tmpsave;
			is_clear_mine_use = false;
			board[last_select.y][last_select.x].spmine[clear_mine] -= 1;
			board[last_select.y][last_select.x].keystatus = 2;
			player_info[clmine_info[i].owner].coin += 10;
			clmine_info[i].is_enable = false;
		}
	}
}

void draw_mineinfo_block(int i, int j)
{
	RECT cur = { first_block.left + (j - 1) * 50,
			first_block.top + (i - 1) * 50,
			first_block.right + (j - 1) * 50,
			first_block.bottom + (i - 1) * 50 };
	int h = 50, w = 50;
	RECT _cur = { first_block.left + (j - 1) * 50 + w / 20,
				first_block.top + (i - 1) * 50 + h / 20,
				first_block.right + (j - 1) * 50 - w / 20 + 1,
				first_block.bottom + (i - 1) * 50 - h / 20 - 1 };
	int cnt = 0;
	// special mine
	for (int k = 1; k < 5; k++)
	{
		cnt += board[i][j].spmine[k];
	}
	// normal mine
	cnt += board[i][j].nmminenum;
	wchar_t tmp[10] = { 0 };
	wsprintf(tmp, L"%d", cnt);
	if (cnt)
	{
		draw_block(cur, 10, 1, blockstyle[10]);
		draw_text(_cur, blockstyle[10].strcolor2, tmp);
	}
	else
	{
		draw_block(cur, 11, 0, blockstyle[11]);
		draw_text(_cur, blockstyle[11].strcolor1, tmp);
	}
}

void update_radar()
{
	for (int i = 1; i < 13; i++)
	{
		for (int j = 1; j < 13; j++)
		{
			if (board[i][j].occupant)
			{
				continue;
			}
			board[i][j].keystatus = 5;
		}
	}
}

void mine_remover_deal()
{
	is_mine_remover_use = true;
	normal_mine_deal();
	is_mine_remover_enable = false;
	player_info[curplayer].item[mine_remover] -= 1;
	is_mine_remover_use = false;
	selected_item = 0;
}

void update_robot(pMOUSEMESSAGE mouse)
{
	// reset
	indx2[cur_line] = 0;
	for (int i = 1; i < 13; i++)
	{
		if (cur_line < 13 && board[i][cur_line].occupant == 0)
		{
			board[i][cur_line].keystatus = 0;
		}
		else if (cur_line > 12 && board[cur_line - 12][i].occupant == 0)
		{
			board[cur_line - 12][i].keystatus = 0;
		}
	}
	// 1 - 12
	for (int i = 0; i < 12; i++)
	{
		RECT cur = { first_block.left + 5 + i * 50,
					first_block.top - 43,
					first_block.right - 5 + i * 50,
					first_block.bottom - 53 };
		if (is_mouse_in_rect(mouse->pos, cur))
		{
			indx2[i + 1] = 1;
			if (mouse->is_down)
			{
				if (cur_line == i + 1)
				{
					cur_line = 0;
				}
				else
				{
					cur_line = i + 1;
				}
			}
		}
		else
		{
			indx2[i + 1] = 0;
		}
	}
	// A - L
	for (int i = 0; i < 12; i++)
	{
		RECT cur = { first_block.left - 43,
					first_block.top + 5 + i * 50,
					first_block.right - 53,
					first_block.bottom - 5 + i * 50 };
		if (is_mouse_in_rect(mouse->pos, cur))
		{
			indx2[i + 13] = 1;
			if (mouse->is_down)
			{
				if (cur_line == i + 13)
				{
					cur_line = 0;
				}
				else
				{
					cur_line = i + 13;
				}
			}
		}
		else
		{
			indx2[i + 13] = 0;
		}
	}
	indx2[cur_line] = 1;
	for (int i = 1; i < 13; i++)
	{
		if (cur_line < 13 && board[i][cur_line].occupant == 0)
		{
			board[i][cur_line].keystatus = 3;
		}
		else if (cur_line > 12 && board[cur_line - 12][i].occupant == 0)
		{
			board[cur_line - 12][i].keystatus = 3;
		}
	}
}

void robot_deal()
{
	COORDINATE tmpsave = last_select;
	for (int i = 1; i < 13; i++)
	{
		if (cur_line < 13 && board[i][cur_line].occupant == 0)
		{
			last_select = { cur_line,i };
		}
		else if (cur_line > 12 && board[cur_line - 12][i].occupant == 0)
		{
			last_select = { i,cur_line - 12 };
		}
		normal_mine_deal();
	}
	last_select = tmpsave;
}