#include "../../include/starwar.h"
#include "../../include/UI.h"

RECT first_block = { 80, 75, 130, 125 };
COORDINATE mouse_board_pos = { 0,0 };
COORDINATE last_select = { 0,0 };
int indx[25] = { 0 };

COORDINATE mouse_to_board_pos(pMOUSEMESSAGE mouse)
{
	int mouse_xblock = 0, mouse_yblock = 0;
	bool is_x_in = false, is_y_in = false;
	for (int i = 0; i < 12; i++)
	{
		if (first_block.left + i * 50 < (mouse->pos).x
			&& (mouse->pos).x < first_block.right + i * 50)
		{
			mouse_xblock = i + 1;
			is_x_in = true;
			break;
		}
	}
	for (int i = 0; i < 12; i++)
	{
		if (first_block.top + i * 50 < (mouse->pos).y
			&& (mouse->pos).y < first_block.bottom + i * 50)
		{
			mouse_yblock = i + 1;
			is_y_in = true;
			break;
		}
	}
	if (is_x_in && is_y_in)
	{
		return { mouse_xblock,mouse_yblock };
	}
	else
	{
		return { 0,0 };
	}
}

bool isselected()
{
	// matrix mine lay
	if (phase == mine_lay && item_seq[selected_item] == matrix_mine)
	{
		return true;
	}
	// chase mine lay
	if (is_chase_mine_use)
	{
		if (board[mouse_board_pos.y][mouse_board_pos.x].occupant)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// have occupied
	if (board[mouse_board_pos.y][mouse_board_pos.x].occupant)
	{
		return false;
	}
	if (is_curplayer_finish)
	{
		return false;
	}
	// mine lay phase
	if (phase == mine_lay)
	{
		if (selected_item == 0 || is_radar_use || item_seq[selected_item] == radar)
		{
			return false;
		}
		return true;
	}
	// occupy phase
	if (phase == occupy)
	{
		// after bloom
		if (judge_occupy_status < 0)
		{
			return false;
		}
		// no point
		if (player_info[curplayer].point <= 0)
		{
			return false;
		}
		// mine remove robot use
		if (is_robot_use || item_seq[selected_item] == mine_remove_robot)
		{
			return false;
		}
		// no occupy
		if ((occupy_num == 0 && !is_mobfix_patrol_use) 
			|| (mobfix_begin == mobfix_move && is_mobfix_patrol_use))
		{
			return true;
		}
		// not link
		int tmpnum = occupy_num;
		if (is_mobfix_patrol_use)
		{
			tmpnum = mobfix_move - mobfix_begin;
		}
		int delta_x = abs(mouse_board_pos.x - have_occupied[tmpnum - 1].x),
			delta_y = abs(mouse_board_pos.y - have_occupied[tmpnum - 1].y);
		if (delta_x > 1 || delta_y > 1)
		{
			return false;
		}
		if (delta_x == 1 && delta_y == 1)
		{
			return false;
		}
	}
	if (phase == purchase)
	{
		return false;
	}
	// other phase - can choose any one
	return true;
}

void update_board(pMOUSEMESSAGE mouse)
{
	RECT board_area = { 0,0,720,720 };
	if ((!(mtmine_select.x && mtmine_select.y)
		|| !(mouse_board_pos.x - mtmine_select.x >= 0
		&& mouse_board_pos.x - mtmine_select.x < 2
		&& mouse_board_pos.y - mtmine_select.y >= 0
		&& mouse_board_pos.y - mtmine_select.y < 2)
		|| board[mouse_board_pos.y][mouse_board_pos.x].occupant)
		&& board[mouse_board_pos.y][mouse_board_pos.x].keystatus < 4)
	{
		board[mouse_board_pos.y][mouse_board_pos.x].keystatus = 0;
	}
	indx[mouse_board_pos.x] = 0;
	indx[mouse_board_pos.y + 12] = 0;
	if (!is_mouse_in_rect(mouse->pos,board_area))
	{
		return;
	}

	mouse_board_pos = mouse_to_board_pos(mouse);
	if (mouse_board_pos.x && mouse_board_pos.y)
	{
		indx[mouse_board_pos.x] = 1;
		indx[mouse_board_pos.y + 12] = 1;
		// highlight
		if ((!(mtmine_select.x && mtmine_select.y)
			|| !(mouse_board_pos.x - mtmine_select.x >= 0
			&& mouse_board_pos.x - mtmine_select.x < 2
			&& mouse_board_pos.y - mtmine_select.y >= 0
			&& mouse_board_pos.y - mtmine_select.y < 2)
			|| board[mouse_board_pos.y][mouse_board_pos.x].occupant)
			&& board[mouse_board_pos.y][mouse_board_pos.x].keystatus < 4)
		{
			board[mouse_board_pos.y][mouse_board_pos.x].keystatus = 1;
		}
		// select block
		if (mouse->is_down && curplayer && isselected() && !is_mine_remover_enable)
		{
			// click again to cancle choose
			if (last_select.x && last_select.y 
				&& mouse_board_pos.x == last_select.x
				&& mouse_board_pos.y == last_select.y)
			{
				last_select = { 0,0 };
			}
			else
			{
				last_select = mouse_board_pos;
			}
		}
	}
}

void draw_board()
{
	// board
	for (int i = 1; i < 13; i++)
	{
		for (int j = 1; j < 13; j++)
		{
			RECT cur = { first_block.left + (j - 1) * 50,
						first_block.top + (i - 1) * 50,
						first_block.right + (j - 1) * 50,
						first_block.bottom + (i - 1) * 50 };
			if ((phase == occupy && board[i][j].keystatus >= 4 && occupyphase_status == 0)
				|| (phase == mine_lay && !is_radar_use && board[i][j].keystatus != 1
					&& board[i][j].keystatus != 3))
			{
				board[i][j].keystatus = 0;
			}
			// mine remove robot
			if (board[i][j].keystatus == 6)
			{
				draw_block(cur, 11, 1, blockstyle[11]);
				board[i][j].keystatus = 0;
			}
			// selected
			else if ((phase == mine_lay || phase == occupy)
				&& ((last_select.x == j && last_select.y == i 
					&& (board[i][j].occupant == 0 || is_chase_mine_use))
					|| board[i][j].keystatus == 3))
			{
				draw_block(cur, 10, 0, blockstyle[10]);
			}
			// bloom
			else if (board[i][j].keystatus == 2)
			{
				draw_block(cur, 10, 1, blockstyle[10]);
				board[i][j].keystatus = 0;
			}
			// safe
			else if (board[i][j].keystatus == 4)
			{
				draw_block(cur, 11, 0, blockstyle[11]);
			}
			// check mine num
			else if (board[i][j].keystatus == 5)
			{
				draw_mineinfo_block(i,j);
			}
			else
			{
				draw_block(cur, board[i][j].occupant, board[i][j].keystatus, 
							blockstyle[board[i][j].occupant]);
				board[i][j].keystatus = 0;
			}
			//draw_block(cur, i, board[i][j].keystatus, blockstyle[i]);
		}
	}
	// board index
	// 1 - 12
	for (int i = 0; i < 12; i++)
	{
		RECT cur = { first_block.left + 5 + i * 50,
					first_block.top - 43,
					first_block.right - 5 + i * 50,
					first_block.bottom - 53 };
		wchar_t ch[10];
		wsprintf(ch, L"%d", 1 + i);
		if (indx[i + 1] || indx2[i + 1])
		{
			draw_button(cur, ch, 1);
		}
		else
		{
			draw_text(cur, RGB(0, 0, 0), ch);
		}
	}
	// A - L
	for (int i = 0; i < 12; i++)
	{
		RECT cur = { first_block.left - 43,
					first_block.top + 5 + i * 50,
					first_block.right - 53,
					first_block.bottom - 5 + i * 50 };
		wchar_t ch[10];
		wsprintf(ch, L"%c", L'A' + i);
		if (indx[i + 13] || indx2[i + 13])
		{
			draw_button(cur, ch, 1);
		}
		else
		{
			draw_text(cur, RGB(0, 0, 0), ch);
		}
	}
}