#include "../include/starwar.h"
#include "../include/UI.h"

BOARD board[13][13]; // index 1 - 12
PLAYER player_info[10]; // index 1 - 9
RANK player_rank[10]; // index 1 - 9
ITEM item_info[10]; // index 1 - 9
MTMINE mtmine_info[6]; // index 1 - 5
CSMINE csmine_info[5]; // index 1 - 4
SDMINE sdmine_info[4]; // index 1 - 3
CLMINE clmine_info[3]; // index 1 - 2
int mtmine_num = 0;
int csmine_num = 0;
int sdmine_num = 0;
int clmine_num = 0;

int round, // 0 - 4
	controller, curplayer;

PHASE phase = preload; // 0 - 5
GAMESTATUS status = end_game;
int control_status = -1;

void Initialize()
{
	initgraph(1280, 720);

	setbkcolor(RGB(200, 200, 200));
	setbkmode(TRANSPARENT); // make text background transparent

	BeginBatchDraw();
}

void Launch()
{
	do
	{
		// start menu
		while (status == end_game)
		{
			update_start_menu();
			draw_start_menu();
		}

		// status check
		if (status == exit_game)
		{
			break;
		}

		// game main loop
		while (status == start_game)
		{
			// preload
			reset();
			round = 1,phase = preload;

			//test1();
			//test2();
			//test3();
			
			// begin
			while (round < 5 || phase < blank)
			{
				update();
				// status check
				if (round == 5 && phase == blank)
				{
					status = end_game;
				}
				if (status != start_game)
				{
					break;
				}
				// update round and phase
				if (phase > occupy && phase < blank)
				{
					if (round < 4 && phase > purchase) // future 5 phase with purchase
					{
						phase = preload;
						round += 1;
					}
					else if (round == 4 && phase == purchase)
					{
						phase = result;
						round = 5;
					}
					//else if (phase != result) // now 4 phases for no purchase
					//{
					//	phase = preload;
					//}
				}
				draw();
			}
			// status check
			if (status == restart_game)
			{
				status = start_game;
			}
		}
	} while (status != exit_game);
}

void ClearUp()
{
	EndBatchDraw();
	closegraph();
}

void reset()
{
	// board reset
	memset(board, 0, sizeof(BOARD) * 13 * 13);

	// player reset
	for (int i = 0; i < 10; i++)
	{
		player_info[i].isban = false;
		player_info[i].point = 0;
		player_info[i].coin = 100;
		memset(player_info[i].item, 0, sizeof(int) * 10);
	}
	memset(player_rank, 0, sizeof(RANK) * 10);

	// item list reset
	item_info[0].left_num = 0;
	item_info[0].total_num = 0;
	item_info[0].price = 0;
	// ¾ØÕóµØÀ×
	item_info[matrix_mine].left_num = 5;
	item_info[matrix_mine].total_num = 5;
	item_info[matrix_mine].price = 20;
	// ×·»÷µØÀ×
	item_info[chase_mine].left_num = 4;
	item_info[chase_mine].total_num = 4;
	item_info[chase_mine].price = 30;
	// æß×ÓµØÀ×
	item_info[spread_mine].left_num = 3;
	item_info[spread_mine].total_num = 3;
	item_info[spread_mine].price = 40;
	// ¾»»¯µØÀ×
	item_info[clear_mine].left_num = 2;
	item_info[clear_mine].total_num = 2;
	item_info[clear_mine].price = 50;
	// ÅÅÀ×ÊÖ
	item_info[mine_remover].left_num = 5;
	item_info[mine_remover].total_num = 5;
	item_info[mine_remover].price = 30;
	// ³å·æ³µ
	item_info[mobfix_patrol].left_num = 4;
	item_info[mobfix_patrol].total_num = 4;
	item_info[mobfix_patrol].price = 40;
	// Í¸ÊÓÒÇ
	item_info[scanner].left_num = 3;
	item_info[scanner].total_num = 3;
	item_info[scanner].price = 50;
	// É¨À×»úÆ÷ÈË
	item_info[mine_remove_robot].left_num = 2;
	item_info[mine_remove_robot].total_num = 2;
	item_info[mine_remove_robot].price = 60;
	// Ì½²âÀ×´ï
	item_info[radar].left_num = 1;
	item_info[radar].total_num = 1;
	item_info[radar].price = 70;

	// special mine reset
	memset(mtmine_info, 0, sizeof(MTMINE) * 6);
	memset(csmine_info, 0, sizeof(CSMINE) * 5);
	memset(sdmine_info, 0, sizeof(SDMINE) * 4);
	memset(clmine_info, 0, sizeof(SDMINE) * 3);
	mtmine_num = 0;
	csmine_num = 0;
	sdmine_num = 0;
	clmine_num = 0;

	// game info reset
	round = 0;
	phase = preload;
	controller = 0;
	curplayer = 0;

	// control
	control_status = -1;
	reset_phase();
}

void update()
{
	// status check
	if (is_key_down(VK_Q))
	{
		status = exit_game;
		return;
	}
	if (is_key_down(VK_ESCAPE))
	{
		status = end_game;
		return;
	}
	if (is_key_down(VK_R))
	{
		status = restart_game;
		return;
	}
	// mouse message
	pMOUSEMESSAGE mouse;
	peek_mouse_message();
	mouse = get_mouse_message();
	// update
	// matrix mine
	if (item_seq[selected_item] == matrix_mine 
		|| ((last_select.x == 0 || last_select.y == 0) 
			&& mtmine_select.x && mtmine_select.y))
	{
		update_matrix_mine();
	}
	// board
	update_board(mouse);
	// radar
	if (is_radar_use)
	{
		update_radar();
	}
	// mine remove robot
	else if (is_robot_use)
	{
		update_robot(mouse);
	}
	else if (!is_select_reset && is_scanner_use)
	{
		board[cur_draw_block.y][cur_draw_block.x].keystatus = 5;
	}
	update_gameinfo(mouse);
	switch (phase)
	{
	case preload:
		reset_phase();
		reset_item();
		update_spread_mine();
		phase = (PHASE)((int)phase + 1);
		break;
	case fre:
		update_freephase(mouse);
		break;
	case control:
		update_controlphase(mouse);
		break;
	case mine_lay:
		update_layminephase(mouse);
		break;
	case occupy:
		update_occupyphase(mouse);
		break;
	case purchase:
		update_purchasephase(mouse);
		break;
	case result:
		update_resultphase(mouse);
		break;
	default:
		break;
	}
}

void draw()
{
	cleardevice();

	draw_board();
	draw_gameinfo();
	switch (phase)
	{
	case preload:
		break;
	case fre:
		draw_freephase();
		break;
	case control:
		draw_controlphase();
		break;
	case mine_lay:
		draw_layminephase();
		break;
	case occupy:
		draw_occupyphase();
		break;
	case purchase:
		draw_purchasephase();
		break;
	case result:
		draw_resultphase();
		break;
	default:
		break;
	}

	FlushBatchDraw();
	// preload end
	if (phase == preload)
	{
		Sleep(500);
	}
	// bloom
	if (judge_occupy_status < 0 && !is_bloom_done)
	{
		Sleep(1000);
		is_bloom_done = true;
	}
	else if (!is_mobfix_patrol_bloom_done
			&& mobfix_begin == mobfix_move)
	{
		Sleep(1000);
		is_mobfix_patrol_bloom_done = true;
	}
	// mine remove robot
	else if (!is_robot_done && cur_line)
	{
		robot_deal();
		player_info[curplayer].item[mine_remove_robot] -= 1;
		indx2[cur_line] = 0;
		cur_line = 0;
		is_robot_use = false;
		is_robot_done = true;
		selected_item = 0;
		Sleep(1000);
	}
	Sleep(20);
}

// develop
void test1()
{
	board[1][1].occupant = 8;
	board[2][1].occupant = 8;
	board[3][1].occupant = 0;
	board[4][1].occupant = 2;
	board[5][1].occupant = 2;
	board[6][1].occupant = 2;
	board[7][1].occupant = 2;
	board[8][1].occupant = 9;
	board[9][1].occupant = 9;
	board[10][1].occupant = 9;
	board[11][1].occupant = 9;
	board[12][1].occupant = 9;

	board[1][2].occupant = 8;
	board[2][2].occupant = 8;
	board[3][2].occupant = 0;
	board[4][2].occupant = 0;
	board[5][2].occupant = 8;
	board[6][2].occupant = 8;
	board[7][2].occupant = 8;
	board[8][2].occupant = 9;
	board[9][2].occupant = 9;
	board[10][2].occupant = 9;
	board[11][2].occupant = 0;
	board[12][2].occupant = 9;

	board[1][3].occupant = 1;
	board[2][3].occupant = 1;
	board[3][3].occupant = 5;
	board[4][3].occupant = 5;
	board[5][3].occupant = 0;
	board[6][3].occupant = 0;
	board[7][3].occupant = 0;
	board[8][3].occupant = 9;
	board[9][3].occupant = 9;
	board[10][3].occupant = 9;
	board[11][3].occupant = 6;
	board[12][3].occupant = 9;

	board[1][4].occupant = 1;
	board[2][4].occupant = 1;
	board[3][4].occupant = 1;
	board[4][4].occupant = 6;
	board[5][4].occupant = 7;
	board[6][4].occupant = 0;
	board[7][4].occupant = 7;
	board[8][4].occupant = 7;
	board[9][4].occupant = 7;
	board[10][4].occupant = 0;
	board[11][4].occupant = 6;
	board[12][4].occupant = 9;

	board[1][5].occupant = 1;
	board[2][5].occupant = 0;
	board[3][5].occupant = 6;
	board[4][5].occupant = 6;
	board[5][5].occupant = 3;
	board[6][5].occupant = 3;
	board[7][5].occupant = 7;
	board[8][5].occupant = 7;
	board[9][5].occupant = 7;
	board[10][5].occupant = 6;
	board[11][5].occupant = 6;
	board[12][5].occupant = 9;

	board[1][6].occupant = 1;
	board[2][6].occupant = 1;
	board[3][6].occupant = 6;
	board[4][6].occupant = 0;
	board[5][6].occupant = 3;
	board[6][6].occupant = 3;
	board[7][6].occupant = 7;
	board[8][6].occupant = 7;
	board[9][6].occupant = 7;
	board[10][6].occupant = 0;
	board[11][6].occupant = 6;
	board[12][6].occupant = 6;

	board[1][7].occupant = 8;
	board[2][7].occupant = 8;
	board[3][7].occupant = 6;
	board[4][7].occupant = 0;
	board[5][7].occupant = 3;
	board[6][7].occupant = 3;
	board[7][7].occupant = 7;
	board[8][7].occupant = 7;
	board[9][7].occupant = 0;
	board[10][7].occupant = 6;
	board[11][7].occupant = 6;
	board[12][7].occupant = 0;

	board[1][8].occupant = 8;
	board[2][8].occupant = 8;
	board[3][8].occupant = 3;
	board[4][8].occupant = 3;
	board[5][8].occupant = 3;
	board[6][8].occupant = 2;
	board[7][8].occupant = 2;
	board[8][8].occupant = 0;
	board[9][8].occupant = 0;
	board[10][8].occupant = 0;
	board[11][8].occupant = 6;
	board[12][8].occupant = 0;

	board[1][9].occupant = 8;
	board[2][9].occupant = 8;
	board[3][9].occupant = 1;
	board[4][9].occupant = 0;
	board[5][9].occupant = 2;
	board[6][9].occupant = 2;
	board[7][9].occupant = 2;
	board[8][9].occupant = 0;
	board[9][9].occupant = 4;
	board[10][9].occupant = 4;
	board[11][9].occupant = 6;
	board[12][9].occupant = 0;

	board[1][10].occupant = 8;
	board[2][10].occupant = 1;
	board[3][10].occupant = 1;
	board[4][10].occupant = 0;
	board[5][10].occupant = 2;
	board[6][10].occupant = 2;
	board[7][10].occupant = 2;
	board[8][10].occupant = 0;
	board[9][10].occupant = 4;
	board[10][10].occupant = 4;
	board[11][10].occupant = 6;
	board[12][10].occupant = 0;

	board[1][11].occupant = 8;
	board[2][11].occupant = 1;
	board[3][11].occupant = 1;
	board[4][11].occupant = 0;
	board[5][11].occupant = 2;
	board[6][11].occupant = 2;
	board[7][11].occupant = 2;
	board[8][11].occupant = 2;
	board[9][11].occupant = 4;
	board[10][11].occupant = 0;
	board[11][11].occupant = 0;
	board[12][11].occupant = 4;

	board[1][12].occupant = 8;
	board[2][12].occupant = 1;
	board[3][12].occupant = 1;
	board[4][12].occupant = 4;
	board[5][12].occupant = 4;
	board[6][12].occupant = 4;
	board[7][12].occupant = 4;
	board[8][12].occupant = 4;
	board[9][12].occupant = 4;
	board[10][12].occupant = 4;
	board[11][12].occupant = 4;
	board[12][12].occupant = 4;

	player_info[1].coin = 20;
	player_info[2].coin = 10;
	player_info[3].coin = 20;
	player_info[4].coin = 20;
	player_info[5].coin = 0;
	player_info[6].coin = 20;
	player_info[7].coin = 20;
	player_info[8].coin = 10;
	player_info[9].coin = 0;
	round = 4;
	phase = purchase;
}

void test2()
{
	board[1][1].occupant = 6;
	board[2][1].occupant = 6;
	board[3][1].occupant = 8;
	board[4][1].occupant = 8;
	board[5][1].occupant = 0;
	board[6][1].occupant = 0;
	board[7][1].occupant = 3;
	board[8][1].occupant = 3;
	board[9][1].occupant = 3;
	board[10][1].occupant = 3;
	board[11][1].occupant = 0;
	board[12][1].occupant = 7;

	board[1][2].occupant = 6;
	board[2][2].occupant = 6;
	board[3][2].occupant = 8;
	board[4][2].occupant = 8;
	board[5][2].occupant = 9;
	board[6][2].occupant = 4;
	board[7][2].occupant = 4;
	board[8][2].occupant = 3;
	board[9][2].occupant = 7;
	board[10][2].occupant = 7;
	board[11][2].occupant = 7;
	board[12][2].occupant = 7;

	board[1][3].occupant = 6;
	board[2][3].occupant = 9;
	board[3][3].occupant = 8;
	board[4][3].occupant = 8;
	board[5][3].occupant = 9;
	board[6][3].occupant = 4;
	board[7][3].occupant = 4;
	board[8][3].occupant = 3;
	board[9][3].occupant = 7;
	board[10][3].occupant = 7;
	board[11][3].occupant = 7;
	board[12][3].occupant = 7;

	board[1][4].occupant = 0;
	board[2][4].occupant = 9;
	board[3][4].occupant = 9;
	board[4][4].occupant = 0;
	board[5][4].occupant = 9;
	board[6][4].occupant = 4;
	board[7][4].occupant = 4;
	board[8][4].occupant = 4;
	board[9][4].occupant = 4;
	board[10][4].occupant = 0;
	board[11][4].occupant = 7;
	board[12][4].occupant = 7;

	board[1][5].occupant = 5;
	board[2][5].occupant = 5;
	board[3][5].occupant = 9;
	board[4][5].occupant = 9;
	board[5][5].occupant = 0;
	board[6][5].occupant = 4;
	board[7][5].occupant = 4;
	board[8][5].occupant = 4;
	board[9][5].occupant = 4;
	board[10][5].occupant = 4;
	board[11][5].occupant = 7;
	board[12][5].occupant = 6;

	board[1][6].occupant = 5;
	board[2][6].occupant = 5;
	board[3][6].occupant = 5;
	board[4][6].occupant = 9;
	board[5][6].occupant = 9;
	board[6][6].occupant = 0;
	board[7][6].occupant = 0;
	board[8][6].occupant = 6;
	board[9][6].occupant = 6;
	board[10][6].occupant = 6;
	board[11][6].occupant = 6;
	board[12][6].occupant = 6;

	board[1][7].occupant = 5;
	board[2][7].occupant = 5;
	board[3][7].occupant = 5;
	board[4][7].occupant = 5;
	board[5][7].occupant = 9;
	board[6][7].occupant = 9;
	board[7][7].occupant = 3;
	board[8][7].occupant = 3;
	board[9][7].occupant = 3;
	board[10][7].occupant = 6;
	board[11][7].occupant = 6;
	board[12][7].occupant = 6;

	board[1][8].occupant = 2;
	board[2][8].occupant = 2;
	board[3][8].occupant = 5;
	board[4][8].occupant = 5;
	board[5][8].occupant = 5;
	board[6][8].occupant = 9;
	board[7][8].occupant = 0;
	board[8][8].occupant = 0;
	board[9][8].occupant = 0;
	board[10][8].occupant = 8;
	board[11][8].occupant = 8;
	board[12][8].occupant = 8;

	board[1][9].occupant = 2;
	board[2][9].occupant = 2;
	board[3][9].occupant = 0;
	board[4][9].occupant = 5;
	board[5][9].occupant = 5;
	board[6][9].occupant = 9;
	board[7][9].occupant = 0;
	board[8][9].occupant = 0;
	board[9][9].occupant = 8;
	board[10][9].occupant = 8;
	board[11][9].occupant = 8;
	board[12][9].occupant = 8;

	board[1][10].occupant = 2;
	board[2][10].occupant = 0;
	board[3][10].occupant = 0;
	board[4][10].occupant = 0;
	board[5][10].occupant = 2;
	board[6][10].occupant = 1;
	board[7][10].occupant = 1;
	board[8][10].occupant = 0;
	board[9][10].occupant = 0;
	board[10][10].occupant = 0;
	board[11][10].occupant = 0;
	board[12][10].occupant = 0;

	board[1][11].occupant = 2;
	board[2][11].occupant = 1;
	board[3][11].occupant = 1;
	board[4][11].occupant = 2;
	board[5][11].occupant = 2;
	board[6][11].occupant = 1;
	board[7][11].occupant = 1;
	board[8][11].occupant = 0;
	board[9][11].occupant = 0;
	board[10][11].occupant = 0;
	board[11][11].occupant = 0;
	board[12][11].occupant = 0;

	board[1][12].occupant = 2;
	board[2][12].occupant = 2;
	board[3][12].occupant = 2;
	board[4][12].occupant = 2;
	board[5][12].occupant = 1;
	board[6][12].occupant = 1;
	board[7][12].occupant = 1;
	board[8][12].occupant = 1;
	board[9][12].occupant = 1;
	board[10][12].occupant = 1;
	board[11][12].occupant = 1;
	board[12][12].occupant = 7;

	player_info[1].coin = 0;
	player_info[2].coin = 30;
	player_info[3].coin = 10;
	player_info[4].coin = 30;
	player_info[5].coin = 60;
	player_info[6].coin = 0;
	player_info[7].coin = 10;
	player_info[8].coin = 10;
	player_info[9].coin = 0;
	round = 4;
	phase = purchase;
}

void test3()
{
	board[1][1].occupant = 1;
	board[1][2].occupant = 2;
	board[1][3].occupant = 3;
	board[1][4].occupant = 4;
	board[1][5].occupant = 5;
	board[1][6].occupant = 6;
	board[1][7].occupant = 7;
	board[1][8].occupant = 8;
	board[1][9].occupant = 9;

	for (int i = 0; i < 9; i++)
	{
		board[12][12].nmmine[i] = i + 1;
	}
	board[12][12].nmminenum = 9;

	player_info[1].coin = 40;
	player_info[2].coin = 70;
	player_info[3].coin = 60;
	player_info[4].coin = 50;
	player_info[5].coin = 70;
	player_info[6].coin = 60;
	player_info[7].coin = 50;
	player_info[8].coin = 40;
	player_info[9].coin = 30;

	player_info[1].item[matrix_mine] = 1;
	player_info[2].item[chase_mine] = 1;
	player_info[3].item[spread_mine] = 1;
	player_info[4].item[clear_mine] = 1;
	player_info[5].item[mine_remover] = 1;
	player_info[6].item[mobfix_patrol] = 2;
	player_info[7].item[scanner] = 1;
	player_info[8].item[mine_remove_robot] = 1;
	player_info[9].item[radar] = 1;

	round = 2;
	phase = preload;
}