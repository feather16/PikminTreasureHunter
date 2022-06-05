#include "def.hpp"
#include "Object.hpp"
#include "split_string.hpp"
#include "KeyState.hpp"

void per_frame();
void check_keys();
void draw_objects();

int t, T, overtime;
vector<Object> obj;
unordered_map<string, int> image;
unordered_map<string, int> sound;
int ground, outerwall;

int CurrentStage;
int Treasures, TreasuresYouHave;
bool cleared = false;

KeyState old = KeyState(false);

void per_frame() {

	check_keys();

	bool frogjumped = false;
	
	for (int i = WALLS + 1; i < obj.size(); i++) {

		if (obj[i].erased) {
			obj.erase(obj.begin() + i--);
			continue;
		}

		if (obj[i].name == "wall") continue; //�ǂ͉������Ȃ�

		//��]
		if (obj[i].rotated) {
			int cx = obj[i].centerX;
			int cy = obj[i].centerY;

			if (cx == -1 && cy == -1) {
				int ID = getID("last");
				if (ID == -1) {
					obj.erase(obj.begin() + i--);
					continue;
				}
				else {
					cx = obj[ID].x;
					cy = obj[ID].y;
				}
			}

			float theta = M_PI / 2.0 * obj[i].phase
				+ 2.0 * M_PI * obj[i].frequency * t / ((float)FRAMES_PER_SECOND);
			obj[i].x = cx + obj[i].radius * cos(theta);
			obj[i].y = cy - obj[i].radius * sin(theta);
		}

		//�v���C���[�����ɐG��Ď���
		if (obj[i].name == "water" || obj[i].name == "deepwater") {//�����[����
			if (//�ł��H�ł��Ȃ��Ƃ��ɐ��ɗ�����
				PLAYER.name != "blue" &&
				PLAYER.name != "winged" &&
				PLAYER.touch(obj[i], -0.25)
				)
			{
				on_death();
			}
			continue; //����ȏ�͒��ׂ�K�v������
		}

		//�v���C���[���N���̑��Ŏ���
		if (
			obj[i].name == "spiderweb" &&
			PLAYER.name == "winged" &&
			PLAYER.touchjust(obj[i])
			)
		{
			on_death();
			continue;
		}

		if (PLAYER.touch(obj[i])) { // �v���C���[���G��Ă��邩�H

			if (//�����U���Ŏ���
				(obj[i].name == "flame" && PLAYER.name != "red") ||
				(obj[i].name == "electricity" && PLAYER.name != "yellow" && PLAYER.name != "yellowb") ||
				(obj[i].name == "drop" && PLAYER.name != "blue") ||
				(obj[i].name == "poison" && PLAYER.name != "white")
				)
			{
				on_death();
				continue;
			}

			//�G�┚���ɐG���Ǝ���
			if (obj[i].isEnemy || obj[i].name == "exp" || obj[i].name == "tntexp") {

				//���̓A���{�E�Y�ɐG��Ă����ȂȂ�
				if (obj[i].name == "waterwraith" && PLAYER.name == "purple");
				else on_death();
			}

			//���܂��Ă���т�����e�ɐG���
			if (obj[i].name == "nisekiku" && obj[i].wait == -1) {
				obj[i].wait = KIKU_WAIT;
				continue;
			}

			//�N���X�^����j�󂷂�
			if (
				PLAYER.name == "rock" &&
				obj[i].name == "crystal"
				)
			{
				Play(sound["crystalbroken"]);
				obj.erase(obj.begin() + i--);
				continue;
			}

			//����̎擾
			if (obj[i].name == "treasure") {

				if ( // �H�s�N�~���͐[�����̒��̂�������Ȃ�
					PLAYER.name == "winged" &&
					getID(obj[i].x, obj[i].y, "deepwater") != -1
					)
				{
					continue;
				}
				getTreasure(i--);
				continue;
			}

			//�_���x���A�^�P�m�R�̎擾
			if (
				(PLAYER.name == "purple" && obj[i].name == "dumbbell") ||
				(PLAYER.name == "white" && obj[i].name == "takenoko")
				)
			{
				getTreasure(i--);
				continue;
			}

			//�|���K�V�O�T
			if (obj[i].PlayerCanUsePon(obj)) {
				Play(sound["changecolor"]);
				int n = obj[i].name.size();
				string newcolor = obj[i].name.substr(0, n - 3);
				PLAYER = Object(PLAYER.x, PLAYER.y, newcolor);
				obj.erase(obj.begin() + i--);
				continue;
			}

			//P�X�C�b�`
			if (obj[i].isPswitch) {
				string name = "pblock_" + obj[i].name.substr(8, obj[i].name.size() - 8);
				Play(sound["pswitch"]);
				for (int j = WALLS + 1; j < obj.size(); j++) {
					if (obj[j].name == name) {
						obj[j].erased = true;
					}
				}
				obj.erase(obj.begin() + i--);
				continue;
			}
		}

		//���e�̃J�E���g
		if (obj[i].BeforeExp > 0) {
			obj[i].BeforeExp--;
		}

		//�����̃J�E���g
		if (obj[i].ExpLasts > 0) {
			if (--obj[i].ExpLasts == 0) {
				obj.erase(obj.begin() + i--);
				continue;
			}
		}

		//�������邩�H
		if (obj[i].name == "bomb"&&obj[i].BeforeExp == 0) {

			//����
			Play(sound["exp"]);
			Explodes(obj[i].x, obj[i].y, i, "exp");
		}

		//���e�������Ɋ������܂��
		if (obj[i].name == "tnt" || obj[i].name == "bomb") {
			for (int j = 0; j < obj.size(); j++) {
				if ((obj[j].name == "exp" || obj[j].name == "tntexp") && obj[i].touch(obj[j])) {
					Explodes(obj[i].x, obj[i].y, i, obj[i].name == "bomb" ? "exp" : "tntexp");
				}
			}
		}

		//�т�����e���N����
		if (obj[i].name == "nisekiku" && obj[i].wait > 0) {
			if (--obj[i].wait == 0) {
				Play(sound["kiku"]);
				bool hasTreasure = obj[i].hasTreasure;
				obj[i] = Object(obj[i].x, obj[i].y, "bikkurikiku");
				obj[i].hasTreasure = hasTreasure;
			}
			continue; //����ȏ�͒��ׂ�K�v������
		}

		//�G
		if (obj[i].isEnemy) {

			//�G�̍s��
			if ((t - i) % obj[i].interval == 0) {
				obj[i].Action(obj, PLAYER);
				if (obj[i].name == "queen") {
					obj.push_back(Object(obj[i].adjX() + 2 * IMAGE_SIZE, obj[i].adjY() + IMAGE_SIZE / 2, "baby"));
				}
			}

			//�J�G���̃W�����v��
			if (!frogjumped){
				if (
					(obj[i].name == "frog" && obj[i].FrogJump == FROG_JUMP) ||
					(obj[i].name == "maro" && obj[i].FrogJump == MARO_JUMP)
					) {
					Play(sound["frogjump"]);
					frogjumped = true;
				}
			}

			//�A���{�E�Y�ϐ����������
			if (!obj[i].wwResistance) {
				bool flg = false;

				for (int j = WALLS + 1; j < obj.size(); j++) {

					//�A���{�E�Y��瀂����
					if (
						obj[j].name == "waterwraith" &&
						obj[i].touch(obj[j])
						)
					{
						Play(sound["don"]);
						i = injured(i);
						flg = true;
						break;
					}
				}
				if (flg) continue;
			}
			

			if (obj[i].invincibleTime > 0) { //���G��ԂȂ��
				obj[i].invincibleTime--; //���G���Ԃ����炷
				continue;
			}

			//�A���{�E�Y�����s�N�~���ɓ�����
			if (
				obj[i].name == "waterwraith" &&
				PLAYER.name == "purple" &&
				obj[i].touch(PLAYER)
				)
			{
				i = injured(i);
				continue;
			}

			bool flag = false;
			for (int j = WALLS + 1; j < obj.size(); j++) {
				
				//�����Ɋ������܂��
				if (
					!obj[i].BombResistance &&
					(obj[j].name == "exp" || obj[j].name == "tntexp") &&
					obj[i].touch(obj[j]) 
					
					)
				{
					i = injured(i);
					flag = true;
					break;
				}
			}
			if (flag) continue;
		}

		//�ǂ������ŉ���
		if (obj[i].name == "block") {
			
			for (int j = WALLS + 1; j < obj.size(); j++) {
				if (
					(obj[j].name == "exp" || obj[j].name == "tntexp") &&
					obj[i].touch(obj[j])
					)
				{
					obj.erase(obj.begin() + i--);
					break;
				}
			}
		}
	}
}

void check_keys() {
	KeyState now = KeyState();

	int step = 4;
	if (PLAYER.name == "purple") step = 3;
	else if (PLAYER.name == "white") step = 5;

	//�����̈ړ�
	for (int j = 0; j < step; j++) {
		if (now.d&&PLAYER.CanMove(obj, R)) PLAYER.Move(R);
		if (now.a&&PLAYER.CanMove(obj, L)) PLAYER.Move(L);
		if (now.s&&PLAYER.CanMove(obj, D)) PLAYER.Move(D);
		if (now.w&&PLAYER.CanMove(obj, U)) PLAYER.Move(U);
		
		for (int i = WALLS + 1; i < obj.size(); i++) {
			dir d;
			if (//���ɐڂ��Ă�����
				(obj[i].name == "1box" || (obj[i].name == "10box" && PLAYER.name == "purple")) &&
				(d = PLAYER.touchjust(obj[i]))
				)
			{
				if (obj[i].CanMove(obj, d, i)) obj[i].Move(d);
			}
		}
	}

	// Enter�������ꂽ��
	if (!old.enter && now.enter) {

		//���e�����Ă邩�H
		if (PLAYER.name == "yellow") {
			for (int i = WALLS + 1; i < obj.size(); i++) {
				if (//���e
					obj[i].name == "bomb" &&
					obj[i].BeforeExp == -1 &&
					PLAYER.touch(obj[i])
					)
				{
					//����
					Play(sound["holdbomb"]);
					PLAYER.name = "yellowb";
					obj.erase(obj.begin() + i);
					goto save_key_state;
				}
				else if (
					obj[i].name == "infbomb" &&
					PLAYER.touch(obj[i])
					)
				{
					//����
					Play(sound["holdbomb"]);
					PLAYER.name = "yellowb";
					goto save_key_state;
				}
			}
		}

		//���e��u���邩�H
		else if (PLAYER.name == "yellowb") {
			//�u��
			Play(sound["putbomb"]);
			PLAYER.name = "yellow";
			obj.push_back(Object(PLAYER.x, PLAYER.y, "bomb"));
			obj[obj.size() - 1].BeforeExp = BEFORE_EXP;
			goto save_key_state;
		}

		//������������
		else if (PLAYER.name == "winged") {
			for (int i = WALLS + 1; i < obj.size(); i++) {
				if (
					obj[i].name == "pickedgrass" &&
					PLAYER.touch(obj[i])
					)
				{
					//����������
					Play(sound["don"]);
					obj[i] = Object(obj[i].x, obj[i].y, obj[i].content);
					goto save_key_state;
				}
			}
		}

		//����
		else if (PLAYER.name == "blue") {
			for (int i = WALLS + 1; i < obj.size(); i++) {
				if (
					obj[i].name == "plug" &&
					PLAYER.touch(obj[i])
					)
				{
					//���𔲂�
					Play(sound["don"]);
					int x = obj[i].x, y = obj[i].y;
					obj.erase(obj.begin() + i);
					DrainWater(x, y);
					for (int j = WALLS + 1; j < obj.size(); j++) obj[j].done = false;
					goto save_key_state;
				}
			}
		}

		//��
		for (int i = WALLS + 1; i < obj.size(); i++) {
			dir d = PLAYER.touchjust(obj[i]);
			if (
				(obj[i].name == "bridgeh" && (d == L || d == R)) ||
				(obj[i].name == "bridgev" && (d == U || d == D))
				)
			{
				int x = obj[i].x, y = obj[i].y;

				bool built = false;
				string hv = obj[i].name == "bridgeh" ? "h" : "v";
				while (true) {
					tie(x, y) = Moved(x, y, d, IMAGE_SIZE);

					int ID;
					if ((ID = getID(x, y, "water")) != -1) {
						obj[ID] = Object(x, y, "builtbridge" + hv);
						built = true;
					}
					else if ((ID = getID(x, y, "deepwater")) != -1) {
						obj[ID] = Object(x, y, "builtbridge" + hv);
						built = true;
					}
					else {
						if (built) {
							Play(sound["don"]);
							obj.erase(obj.begin() + i);
						}
						goto save_key_state;
					}
				}
			}
		}
	}

	save_key_state: old = now;
}

void DrainWater(const int x, const int y) {
	int ID;
	if ((ID = getID(x, y, "water")) != -1) { //�����ɐ��������
		obj.erase(obj.begin() + ID);
	}
	else if ((ID = getID(x, y, "deepwater")) != -1) { //�����ɐ[�����������
		obj[ID].name = "water";
		obj[ID].done = true;
	}
	else return;

	//4�����̐��𔲂�
	DrainWater(x + IMAGE_SIZE, y);
	DrainWater(x - IMAGE_SIZE, y);
	DrainWater(x, y + IMAGE_SIZE);
	DrainWater(x, y - IMAGE_SIZE);
}

void Explodes(const int x, const int y, const int ID, const string name) {

	//���e�𔚕��ɕύX
	obj[ID]= Object(x, y, name);

	int size = obj[ID].size;

	//�A��
	for (int i = WALLS + 1; i < obj.size(); i++) {
		if (obj[i].touch(x, y, size)) {
			if (obj[i].name == "bomb") {
				Explodes(obj[i].x, obj[i].y, i, "exp");
			}
			else if (obj[i].name == "tnt") {
				Explodes(obj[i].x, obj[i].y, i, "tntexp");
			}
		}
	}
}

int getID(const int x, const int y, const string name) {
	for (int i = WALLS + 1; i < obj.size(); i++) {
		if (
			!obj[i].done &&
			obj[i].x == x &&
			obj[i].y == y &&
			obj[i].name == name
			)
		{
			return i;
		}
	}
	return -1;
}

int getID(const string name) {
	for (int i = WALLS + 1; i < obj.size(); i++) {
		if (
			!obj[i].done &&
			obj[i].name == name
			)
		{
			return i;
		}
	}
	return -1;
}

void getTreasure(const int ID) {
	Play(sound["gettreasure"]);
	obj.erase(obj.begin() + ID);
	if (++TreasuresYouHave == Treasures) cleared = true;
}

int injured(int ID) {
	if (--obj[ID].HP == 0) { //HP��0�ɂȂ����玀��
		if (obj[ID].hasTreasure) {
			obj.push_back(Object(obj[ID].x, obj[ID].y, "treasure"));
		}

		if (obj[ID].name == "bread" || obj[ID].name == "bread2") {
			int nestID;
			string name = obj[ID].name == "bread" ? "breadnest" : "breadnest2";
			if ((nestID = getID(name)) != -1) {
				if (obj[nestID].hasTreasure) {
					obj[nestID] = Object(obj[nestID].x, obj[nestID].y, "treasure");
				}
				else {
					obj.erase(obj.begin() + nestID);
					if (nestID < ID) ID--;
				}
			}
		}
		
		obj.erase(obj.begin() + ID--);
	}
	else {
		obj[ID].invincibleTime = INVINCIBLE_TIME;
	}
	return ID;
}

void on_death() {
	if (PLAYER.name == "dead") return;
	PLAYER.name = "dead";
	Play(sound["dead"]);
	draw_objects();
	int key;
	while (key = WaitKey()) {
		if (key == KEY_INPUT_RETURN) {
			DxLib_End();
			exit(0);
		}
		else if (key == KEY_INPUT_C) {
			break;
		}
	}
}

void draw_objects() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	Draw(0, 0, ground);
	
	for (int j = MAX_DEPTH; j >= 0; j--) {
		for (int i = WALLS; i < obj.size(); i++) {
			if (obj[i].depth != j) continue;
			if (obj[i].invincibleTime & 2) continue; //���G���Ԃ̈ꕔ�͕`�悵�Ȃ�

			//����
			if (obj[i].name == "treasure") {
				Draw(obj[i].x, obj[i].y, image["treasure" + to_string(obj[i].treasure_ID)]);
				continue;
			}

			//�������O�̔��e
			if (obj[i].name == "bomb" &&
				0 < obj[i].BeforeExp&&obj[i].BeforeExp < BEFORE_EXP / 2) {
				Draw(obj[i].x, obj[i].y, image["bombf"]);
				continue;
			}

			//�h�h���̗�
			if (obj[i].name == "dodoro"&&obj[i].HP == DODORO_HP) {
				Draw(obj[i].x, obj[i].y, image["dodoroegg"]);
				continue;
			}

			//�t
			if (obj[i].isLeaf&&obj[i].Distance2Mass(PLAYER) < 25) {
				if(obj[i].turn) DrawTurn(obj[i].x, obj[i].y, image[obj[i].name + "bug"]);
				else Draw(obj[i].x, obj[i].y, image[obj[i].name + "bug"]);
				continue;
			}

			int X = obj[i].x - (obj[i].size - 1)*IMAGE_SIZE / 2;
			int Y = obj[i].y - (obj[i].size - 1)*IMAGE_SIZE / 2;

			if (obj[i].turn && !obj[i].unturned) DrawTurn(X, Y, image[obj[i].name]);
			else Draw(X, Y, image[obj[i].name]);
		}
	}

	Draw(0, 0, outerwall);

	DrawFormatString(300, 980, BLACK, "%d / %d", TreasuresYouHave, Treasures);
	DrawFormatString(900, 980, BLACK, "%d - %d", (CurrentStage - 1) / 4 + 1, (CurrentStage - 1) % 4 + 1);

	if (PLAYER.name == "dead") {
		SetFontSize(36);
		DrawFormatString(1280, 966, BLACK, "Press Enter to end the game");
		DrawFormatString(1280, 1006, BLACK, "or press C to continue.");
		SetFontSize(52);
	}
	else if (cleared) {
		SetFontSize(36);
		if (CurrentStage != STAGE_NUM) {
			DrawFormatString(1280, 966, BLACK, "Press Enter to");
			DrawFormatString(1280, 1006, BLACK, "go to the next stage.");
		}
		else {
			DrawFormatString(1280, 966, BLACK, "Press Enter to");
			DrawFormatString(1280, 1006, BLACK, "end the game.");
		}
		SetFontSize(52);
	}
	else {
		DrawFormatString(1420, 980, BLACK, "%d", T - t / FRAMES_PER_SECOND);
	}

#ifdef DEBUG
	SetFontSize(36);
	for (int i = 1; i < SIZE_X - 1; i++) {
		for (int j = 1; j < SIZE_Y - 1; j++) {
			DrawFormatString(i*IMAGE_SIZE-4, j*IMAGE_SIZE+10, BLACK, "(");
		}
	}
	SetFontSize(16);
	for (int i = 1; i < SIZE_X - 1; i++) {
		for (int j = 1; j < SIZE_Y - 1; j++) {
			DrawFormatString(i*IMAGE_SIZE + 16, j*IMAGE_SIZE + IMAGE_SIZE / 6, BLACK, "%2d", i);
			DrawFormatString(i*IMAGE_SIZE + 16, j*IMAGE_SIZE + 2 * IMAGE_SIZE / 3, BLACK, "%2d", j);
		}
	}
	SetFontSize(52);
#endif

	ScreenFlip();
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WindowSettings();

	// �c�w���C�u�������������� �G���[���N�����璼���ɏI��
	if (DxLib_Init() == -1) return -1;

	NowLoading();
	
	init_genrand(time(NULL));

	ground = LoadGraph("image/ground.png");
	outerwall = LoadGraph("image/outerwall.png");

	LoadImages();
	LoadSounds();
	ClearDrawScreen();
	
	SetFontSize(96);
	DrawString(210, 300, WINDOW_MESSAGE, WHITE);
	DrawString(440, 700, "PRESS ENTER TO START", WHITE);
	
	WaitEnter();
	Play(sound["start"]);
	ClearDrawScreen();
	NowLoading();

	if (ReadSaveData() == -1) return -1;
	
	while (ProcessMessage() == 0) {
		start:
		t = 0;
		obj.clear();
		SetOuterWalls();
		if (ReadStageData() == -1) return -1;

		ClearDrawScreen();
		overtime = 0;
		SetFontSize(52);
		while (true) {
			t++;
			if (t == T * FRAMES_PER_SECOND) {
				on_death();
			}
			clock_t start_t = clock();
			per_frame();
			if (PLAYER.name == "dead") goto start;
			clock_t end_t = clock();
			draw_objects();
			if (cleared) break;
			clock_t dt = end_t - start_t;
#ifdef DEBUG
			printfDx("obj.size() = %4d, time = %4d\n", obj.size(), dt);
#endif
		}
		cleared = false;
		CurrentStage++;
		if (Save() == -1) return -1;
		WaitEnter();
		if (CurrentStage > STAGE_NUM) break;
	}
	
	DxLib_End();				// �c�w���C�u�����g�p�̏I������
	return 0;				// �\�t�g�̏I�� 
}

void LoadImages() {
	vector<string> filenames = get_all_files("./image/obj");
	for (int i = 0; i < filenames.size(); i++) {
		string name = filenames[i].substr(0, filenames[i].size() - 4);
		string file = "./image/obj/" + name + ".png";
		image[name] = LoadGraph(file.c_str());
	}

	for (int i = 1; i <= KIND_OF_TREASURE; i++) {
		string file = "./image/treasure/treasure" + to_string(i) + ".png";
		image["treasure" + to_string(i)] = LoadGraph(file.c_str());
	}
}

void LoadSounds() {
	vector<string> filenames = get_all_files("./sound");
	for (int i = 0; i < filenames.size(); i++) {
		string name = filenames[i].substr(0, filenames[i].size() - 4);
		string file = "./sound/" + name + ".wav";
		sound[name] = LoadSoundMem(file.c_str());
	}
}

int ReadSaveData() {
	string buf;
	ifstream savedata("textdata/save.txt");
	if (!getline(savedata, buf)) return -1;
	CurrentStage = stoi(buf);
	if (CurrentStage > STAGE_NUM) CurrentStage = 1;
	savedata.close();
	return 0;
}

int ReadStageData() {
	Treasures = TreasuresYouHave = 0;
	string line;
	ifstream stage("textdata/stage/" + to_string(CurrentStage) + ".csv");

	if (!getline(stage, line)) return -1;
	T = stoi(line);

	while (getline(stage, line)) {

		//�󔒍s��#�Ŏn�܂�s�͓ǂݔ�΂�
		if (line == "" || line[0] == '#') {
			continue;
		}

		vector<string> cells = split_string(line, ',');

		string name = cells[0];
		int x = stoi(cells[1]) * IMAGE_SIZE;
		int y = stoi(cells[2]) * IMAGE_SIZE;
		int lx = 1, ly = 1;
		string content = "";
		bool rotated = false;
		int centerX = x, centerY = y;
		int radius = 0;
		dir phase = 0;
		float frequency = 0.25;
		
		for (int i = 3; i < cells.size(); i++) {

			vector<string> words = split_string(cells[i], '=');

			if (words[0] == "lx") lx = stoi(words[1]);
			else if (words[0] == "ly") ly = stoi(words[1]);
			else if (words[0] == "content") content = words[1];
			else if (words[0] == "rot") rotated = true;
			else if (words[0] == "rotl") {
				rotated = true;
				centerX = centerY = -1;
			}
			else if (words[0] == "r") radius = stoi(words[1]) * IMAGE_SIZE;
			else if (words[0] == "p") phase = (dir)stoi(words[1]);
			else if (words[0] == "f") frequency = stod(words[1]);
		}

		if (rotated) tie(x, y) = Moved(x, y, phase, radius);

		for (int i = 0; i < lx; i++) {
			for (int j = 0; j < ly; j++) {
				Object object(x + i * IMAGE_SIZE, y + j * IMAGE_SIZE, name);

				if (object.rotated = rotated) {
					object.centerX = centerX;
					object.centerY = centerY;
					object.radius = radius;
					object.phase = phase;
					object.frequency = frequency;
				}

				if (
					name == "treasure" ||
					name.compare(0, 8, "treasure") == 0 ||
					name == "dumbbell" ||
					name == "takenoko"
					)
				{
					Treasures++;
				}

				if (content == "tr") {
					object.content = "treasure";
					object.hasTreasure = true;
					Treasures++;
				}
				else if (
					content == "bomb" ||
					content == "infbomb"
					) {
					object.content = content;
				}
				else if (
					content == "takenoko" ||
					content == "dumbbell"
					) {
					object.content = content;
					Treasures++;
				}

				obj.push_back(object);
			}
		}
	}
	stage.close();
	return 0;
}

int Save() {
	ofstream savedata("textdata/save.txt");
	if (savedata.fail()) return -1;
	savedata << CurrentStage;
	savedata.close();
	return 0;
}

void SetOuterWalls() {
	obj.push_back(Object(
		-(SIZE_Y - 1) * IMAGE_SIZE / 2, 
		(SIZE_Y - 1) * IMAGE_SIZE / 2,
		"wall"));
	obj[obj.size() - 1].size = SIZE_Y;
	obj.push_back(Object(
		(SIZE_X - 1) * IMAGE_SIZE + (SIZE_Y - 1) * IMAGE_SIZE / 2,
		(SIZE_Y - 1) * IMAGE_SIZE / 2,
		"wall"));
	obj[obj.size() - 1].size = SIZE_Y;
	obj.push_back(Object(
		(SIZE_X - 1) * IMAGE_SIZE / 2, 
		-(SIZE_X - 1) * IMAGE_SIZE / 2, 
		"wall"));
	obj[obj.size() - 1].size = SIZE_X;
	obj.push_back(Object(
		(SIZE_X - 1) * IMAGE_SIZE / 2, 
		(SIZE_Y - 1) * IMAGE_SIZE + (SIZE_X - 1) * IMAGE_SIZE / 2, 
		"wall"));
	obj[obj.size() - 1].size = SIZE_X;
}