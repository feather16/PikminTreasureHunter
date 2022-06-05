#pragma once

/*
	C / C++ - コード生成 - 基本ランタイムチェック
	両方→規定

	C / C++ - 最適化
	無効→O2
*/

//#define DEBUG

using namespace std;

#include "DxLib.h"
#include "MT.h"
#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <chrono>
#include <thread>
#include <tuple>
#include <algorithm>
#include <unordered_map>

#define _USE_MATH_DEFINES
#include <math.h>

#define WHITE GetColor(255,255,255)
#define BLACK GetColor(0,0,0)

#define PLAYER obj[WALLS]

#define WINDOW_MESSAGE "Pikmin Treasure Hunter Ver.1.0"

#define STAGE_NUM 16

#define IMAGE_SIZE 48
#define SPACE 100
#define SIZE_X 40
#define SIZE_Y 20
#define WALLS 4

#define MAX_DEPTH 4

#define SCREEN_X IMAGE_SIZE * SIZE_X
#define SCREEN_Y IMAGE_SIZE * SIZE_Y + SPACE
#define SCREEN_BIT 32

#define FRAMES_PER_SECOND 60

#define BEFORE_EXP 100
#define EXP_LASTS 60

#define INVINCIBLE_TIME 70
#define FROG_JUMP 12
#define MARO_JUMP 15
#define FROG_WAIT 2
#define MARO_WAIT 1
#define HEBI_WAIT 15
#define DODORO_HP 5

#define KIKU_WAIT 100

#define KIND_OF_TREASURE 9

#define STRLEN 256

// 方向
#define NONE 0
#define U 1
#define L 2
#define D 3
#define R 4
#define RANDOM 5
#define DIR_NUM 5

// enumが使えないっぽいので代わりにこれを使う
typedef int8_t dir;

void DrainWater(const int, const int);
void Explodes(const int, const int, const int, const string);
int getID(const int, const int, const string);
int getID(const string);
void getTreasure(const int);
int injured(int);
void on_death();
void LoadImages();
void LoadSounds();
int ReadSaveData();
int ReadStageData();
int Save();
void SetOuterWalls();
void NowLoading();

/*void msleep(const int millisec) {
	chrono::milliseconds dura(millisec);
	this_thread::sleep_for(dura);
}*/

dir reverse_direction(const dir d) {
	return d == NONE ? NONE : ((d - 1) ^ 2) + 1;
}

tuple<int, int> Moved(const int x, const int y, const dir d, const int dist) {
	switch (d) {
	case R: return{ x + dist,y };
	case L: return{ x - dist,y };
	case D: return{ x,y + dist };
	case U: return{ x,y - dist };
	default: return{ x,y };
	}
}

void WindowSettings() {
	SetMainWindowText(WINDOW_MESSAGE);
	//ChangeWindowMode(TRUE); //ウィンドウモードで起動
	SetGraphMode(SCREEN_X, SCREEN_Y, SCREEN_BIT); //画面の解像度指定
	SetWindowSizeChangeEnableFlag(FALSE); //画面サイズ変更不可
}

void WaitEnter() {
	int key;
	while (key = WaitKey()) {
		if (key == KEY_INPUT_RETURN) {
			break;
		}
	}
}

void Draw(const int x, const int y, const int handle) {
	DrawGraph(x, y, handle, TRUE);
}

void DrawTurn(const int x, const int y, const int handle) {
	DrawTurnGraph(x, y, handle, TRUE);
}

void Play(const int handle) {
	PlaySoundMem(handle, DX_PLAYTYPE_BACK);
}

void NowLoading() {
	SetFontSize(96);
	DrawString(520, 760, "Now Loading...", WHITE);
}

vector<string> get_all_files(const string directory) {
	vector<string> ret;
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	string search = directory + "\\*";

	hFind = FindFirstFile(search.c_str(), &win32fd);

	do {
		if (win32fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			ret.push_back(win32fd.cFileName);
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return ret;
}