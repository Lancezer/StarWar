#pragma once

#ifndef _STARWAR_H_
#define _STARWAR_H_

typedef struct _coordinate
{
	int x, y;
} COORDINATE, *pCOORDINATE;

typedef struct _board
{
	int nmmine[40], nmminenum; // maxnum 20 normal mine, index - normal mine owner
	int spmine[5]; // 4 kinds special mine num, index - itemID
	int occupant; // who owns the square
	int keystatus; 
	// 0 - normal; 1 - holding; 2 - mine; 3 - special selected;
	// 4 - safe; 5 - check mine num; 6 - mine remove robot
	bool is_passed;
} BOARD,*pBOARD;

typedef struct _player
{
	bool isban; // is ban for control
	int point; // move point num
	int coin; // remain coins
	int item[10]; // 9 kinds item num, index - itemID
} PLAYER,*pPLAYER;

typedef struct _rank
{
	int ocpnum;
	int maxlink;
	int finalnum;
	int rank;
} RANK, *pRANK;

typedef struct _item
{
	int left_num; // left num
	int total_num;
	int price;
} ITEM,*pITEM;

typedef struct _matrrix_mine
{
	COORDINATE pos; // mine leftup coordinate (2x2)
	int owner; // owner playerID
	bool is_enable;
} MTMINE,*pMTMINE;

typedef struct _chase_mine
{
	COORDINATE pos; // mine coordinate
	COORDINATE target; // target coordinate
	int owner;
	bool is_enable;
} CSMINE,*pCSMINE;

typedef struct _spread_mine
{
	int owner; // owner playerID
	COORDINATE pos; // center coordinate
	bool issprd; // spread flag
	bool is_enable;
} SDMINE,*pSDMINE;

typedef struct _clear_mine
{
	int owner;
	COORDINATE pos;
	bool is_enable;
} CLMINE,*pCLMINE;

enum item
{
	normal_mine, // ÆÕÍ¨µØÀ×
	matrix_mine, // ¾ØÕóµØÀ×
	chase_mine, // ×·»÷µØÀ×
	spread_mine, // æß×ÓµØÀ×
	clear_mine, // ¾»»¯µØÀ×
	mine_remover, // ÅÅÀ×ÊÖ
	mobfix_patrol, // ³å·æ³µ
	scanner, // Í¸ÊÓÒÇ
	mine_remove_robot, // É¨À×»úÆ÷ÈË
	radar // Ì½²âÀ×´ï
};

enum PHASE
{
	preload,
	fre, // ×ÔÓÉ½×¶Î
	control, // ¿ØÖÆ½×¶Î
	mine_lay, // ÂñÀ×½×¶Î
	occupy, // Õ¼Áì½×¶Î
	purchase, // ¹ºÂò½×¶Î
	result,
	blank // ¿Õ
};

enum GAMESTATUS
{
	start_game,
	end_game,
	exit_game,
	restart_game
};
// game proccesses
void Initialize();
void Launch();
void ClearUp();

void reset();
void update();
void draw();

// develop
void test1();
void test2();
void test3();

// game algo
void reset_phase();
void reset_item();
void judge_occupy();
int dfs_link(int curlink, int i, int j, int id);
int cmp1(const void* e1, const void* e2);
int cmp2(const void* e1, const void* e2);

// game item
void normal_mine_deal();
void matrix_mine_deal();
void chase_mine_deal();
void spread_mine_deal();
void clear_mine_deal();
void mine_remover_deal();
void robot_deal();

#endif

// starwar.cpp
extern BOARD board[13][13];
extern PLAYER player_info[10];
extern RANK player_rank[10];
extern ITEM item_info[10];
extern MTMINE mtmine_info[6]; // index 1 - 5
extern CSMINE csmine_info[5]; // index 1 - 4
extern SDMINE sdmine_info[4]; // index 1 - 3
extern CLMINE clmine_info[3];
extern int mtmine_num;
extern int csmine_num;
extern int sdmine_num;
extern int clmine_num;
extern GAMESTATUS status;
extern int round;
extern PHASE phase;
extern int controller, curplayer;
// board.cpp
extern COORDINATE last_select;
// phase.cpp
extern bool is_curplayer_finish;
extern int selected_item;
extern int item_seq[10]; // save item button itemID sequence, begin from index : 1
extern int occupyphase_status;
extern int judge_occupy_status;
extern COORDINATE have_occupied[17];
extern int occupy_num;
extern bool is_bloom_done;
// item.cpp
extern bool is_mine_remover_enable;
extern COORDINATE mtmine_select;
const int mtmine_direction[][2] = { {0,0},{0,1},{1,0},{1,1} };
extern bool is_chase_mine_use;
const int all_direction[][2] =
{ {-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1} };
extern bool is_radar_use;
extern bool is_mine_remover_use;
extern bool is_mobfix_patrol_use;
extern bool is_mobfix_patrol_enable;
extern bool is_mobfix_patrol_bloom_done;
extern int mobfix_move, mobfix_begin;
extern bool is_scanner_use;
extern bool is_select_reset;
extern COORDINATE cur_draw_block;
extern bool is_robot_use;
extern bool is_robot_done;
extern int cur_line;
extern int indx2[25];