#include "def.hpp"

class Object {
public:

	//敵
	bool isEnemy = false;
	int HP = 1;
	int interval;
	int invincibleTime = 0;
	bool BombResistance = false;
	bool RandomWalker = false;
	bool BearWalker = false;
	bool UjimWalker = false;
	bool KikuWalker = false;
	bool isHebi = false;
	bool WWWalker = false;
	bool isFrog = false;
	bool isLeaf = false;
	int FrogJump = 0;
	dir FrogJumpDir = NONE;
	int wait = 0; //カエル、ケメクジ、菊、ヘビ、アカヘビ、アメボウズの待ち時間に使う
	bool wwResistance = false;

	//爆弾
	int BeforeExp = -1;
	int ExpLasts = -1;

	//ひっこぬけ草
	string content;

	//属性
	bool enterable = false;
	bool isPlayer = false;
	bool isPon = false;
	bool isWater = false;
	bool isOneWayGate = false;
	bool isPswitch = false;
	bool hasTreasure = false;

	//フラグ
	bool turn = false;
	bool unturned = false;
	bool done = false;
	bool erased = false;

	//回転
	bool rotated = false;
	int centerX, centerY;
	int radius;
	dir phase;
	float frequency;

	//基本
	int depth;
	int dist;
	int size = 1;
	int treasure_ID;
	int x, y;
	string name;

	Object(const int, const int, const string);
	void Move(const dir);
	void Action(const vector<Object> &, const Object &);
	bool CanMove(const vector<Object> &, const dir) const;
	bool CanMove(const vector<Object> &, const dir, const int) const;
	bool touch(const int, const int) const;
	bool touch(const int, const int, const int) const;
	bool touch(const Object &) const;
	bool touch(const Object &, const float) const;
	dir touchjust(const Object &) const;
	int adjX() const;
	int adjY() const;
	int Distance2(const int, const int) const;
	int Distance2(const Object &) const;
	int Distance2Mass(const int, const int) const;
	int Distance2Mass(const Object &) const;
	bool PlayerCanUsePon(const vector<Object> &) const;
private:

	//以下、ピクミンは全てtrue
	bool CanSwim = false;
	bool CanPassFlame = false;
	bool CanPassElc = false;
	bool CanPassPoison = false;
	bool CanPassCrystal = false;

	bool CanMoveto(const vector<Object> &, const int, const int) const;
	bool CanMoveto(const vector<Object> &, const int, const int, const int) const;
	void RandomWalk(const vector<Object> &);
	void BearWalk(const vector<Object> &, const Object &);
	dir ApproachDir(const vector<Object> &, const Object &) const;
	dir LeaveDir(const vector<Object> &, const Object &) const;
	void UjimWalk(const vector<Object> &, const Object &);
	void FrogWalk(const vector<Object> &, const Object &);
	void Keme2Walk(const vector<Object> &, const Object &);
	void HebiWalk(const vector<Object> &, const Object &);
	void EnemySet();
};

Object::Object(const int X, const int Y, const string Name) {
	x = X;
	y = Y;
	name = Name;

	if (
		name == "red" ||
		name == "yellow" ||
		name == "blue" ||
		name == "purple" ||
		name == "white" ||
		name == "rock" ||
		name == "winged"
		) 
	{
		isPlayer = true;
		enterable = true;
		CanSwim = true;
		CanPassFlame = true;
		CanPassElc = true;
		CanPassPoison = true;
		dist = 1;
		depth = 0;

		if (name == "rock") CanPassCrystal = true;
	}
	else if (
		name == "redpon" ||
		name == "yellowpon" ||
		name == "bluepon" ||
		name == "purplepon" ||
		name == "whitepon" ||
		name == "rockpon" ||
		name == "wingedpon"
		)
	{
		isPon = true;
		depth = 1;
	}
	else if (
		name == "flame" ||
		name == "electricity" ||
		name == "poison"
		)
	{
		depth = 3;
	}
	else if (
		name == "water" ||
		name == "deepwater" ||
		name == "drop"
		)
	{
		isWater = true;
		depth = 4;
		if (name == "drop") depth = 3;
	}
	else if (
		name == "dumbbell" ||
		name == "takenoko"
		) {
		enterable = true;
		depth = 2;
	}
	else if (name == "crystal") {
		depth = 1;
	}
	else if (name == "pickedgrass") {
		depth = 2;
		enterable = true;
	}
	else if (name == "plug") {
		depth = 3;
		enterable = true;
	}
	else if (name == "spiderweb") {
		depth = 0;
	}
	else if (
		name == "bridgeh" ||
		name == "bridgev"
		) {
		depth = 0;
	}
	else if (
		name == "builtbridgeh" ||
		name == "builtbridgev"
		) {
		depth = 4;
		enterable = true;
	}
	else if (
		name == "1box" ||
		name == "10box"
		) {
		depth = 0;
		dist = 3;
		//CanSwim = true;
		unturned = true;
	}
	else if (
		name == "block" ||
		name == "wall" ||
		name == "breadnest" ||
		name == "tnt" ||
		name == "pblock_blue" ||
		name == "pblock_red" ||
		name == "pblock_yellow"
		) {
		depth = 0;
	}
	else if (name == "breadnest2") {
		depth = 0;
		size = 2;
	}
	else if (name == "outerwallX") {
		size = SIZE_Y;
		depth = 0;
	}
	else if (name == "outerwallY") {
		size = SIZE_X;
		depth = 0;
	}
	else if (
		name == "bomb" ||
		name == "infbomb"
		) {
		enterable = true;
		depth = 1;
	}
	else if (
		name == "right" ||
		name == "left" ||
		name == "up" ||
		name == "down"
		) 
	{
		isOneWayGate = true;
		depth = 4;
	}
	else if (
		name == "pswitch_blue" ||
		name == "pswitch_red" ||
		name == "pswitch_yellow"
		) 
	{
		isPswitch = true;
		depth = 4;
	}
	else if (name == "chap") {
		EnemySet();
		CanSwim = true;
		interval = 30;
		RandomWalker = true;
		dist = 16;
	}
	else if (name == "chap2") {
		EnemySet();
		CanSwim = true;
		interval = 40;
		RandomWalker = true;
		dist = 24;
		size = 2;
		HP = 2;
	}
	else if (name == "bchap") {
		EnemySet();
		CanSwim = true;
		interval = 40;
		BearWalker = true;
		dist = 16;
		HP = 2;
	}
	else if (name == "bchap2") {
		EnemySet();
		CanSwim = true;
		interval = 45;
		BearWalker = true;
		dist = 24;
		size = 2;
		HP = 3;
	}
	else if (name == "baby") {
		EnemySet();
		CanSwim = true;
		interval = 21;
		BearWalker = true;
		dist = 16;
	}
	else if (name == "ujif") {
		EnemySet();
		RandomWalker = true;
		interval = 30;
		dist = 12;
	}
	else if (name == "ujim") {
		EnemySet();
		interval = 35;
		UjimWalker = true;
		dist = 12;
	}
	else if (name == "hebi") {
		EnemySet();
		interval = 25;
		dist = 24;
		HP = 3;
		isHebi = true;
		wwResistance = true;
		wait = HEBI_WAIT;
	}
	else if (name == "akahebi") {
		EnemySet();
		interval = 13;
		dist = 24;
		HP = 5;
		isHebi = true;
		wwResistance = true;
		wait = HEBI_WAIT;
	}
	else if (name == "frog") {
		EnemySet();
		CanSwim = true;
		interval = 40;
		dist = 16;
		isFrog = true;
		HP = 2;
	}
	else if (name == "maro") {
		EnemySet();
		CanSwim = true;
		interval = 32;
		dist = 16;
		isFrog = true;
		HP = 2;
	}
	else if (name == "kiku") {
		depth = 4;
		enterable = true;
	}
	else if (name == "nisekiku") {
		depth = 4;
		enterable = true;
		wait = -1;
	}
	else if (name == "bikkurikiku") {
		EnemySet();
		CanSwim = true;
		interval = 30;
		dist = 12;
		KikuWalker = true;
		HP = 2;
	}
	else if (name == "bread") {
		EnemySet();
		CanSwim = true;
		interval = 15;
		RandomWalker = true;
		dist = 16;
		HP = 1;
	}
	else if (name == "bread2") {
		EnemySet();
		CanSwim = true;
		interval = 11;
		RandomWalker = true;
		dist = 24;
		HP = 3;
	}
	else if (
		name == "leaf" ||
		name == "bleaf"
		) 
	{
		EnemySet();
		CanSwim = true;
		interval = 25;
		isLeaf = true;
		dist = 18;
	}
	else if (name == "keme") {
		EnemySet();
		CanSwim = true;
		interval = 50;
		RandomWalker = true;
		dist = 12;
		HP = 2;
	}
	else if (name == "keme2") {
		EnemySet();
		CanSwim = true;
		interval = 11;
		dist = 18;
		HP = 5;
		wwResistance = true;
		size = 2;
		wait = 0;
	}
	else if (name == "waterwraith") {
		EnemySet();
		CanSwim = true;
		interval = 6;
		dist = 8;
		WWWalker = true;
		HP = 3;
		wwResistance = true;
		BombResistance = true;
		wait = 0;
		CanPassElc = true;
	}
	else if (name == "dodoro") {
		EnemySet();
		CanSwim = true;
		interval = 2;
		WWWalker = true;
		dist = 3;
		HP = DODORO_HP;
		wwResistance = true;
	}
	else if (name == "queen") {
		EnemySet();
		interval = 300;
		HP = 10;
		wwResistance = true;
	}
	else if (name == "last") {
		EnemySet();
		interval = 8;
		HP = 20;
		dist = 16;
		size = 3;
		wait = 0;
		CanSwim = true;
		CanPassFlame = true;
		CanPassElc = true;
		CanPassPoison = true;
	}
	else if (name == "exp") {
		enterable = true;
		depth = 3;
		size = 5;
		ExpLasts = EXP_LASTS;
	}
	else if (name == "tntexp") {
		enterable = true;
		depth = 3;
		size = 9;
		ExpLasts = EXP_LASTS;
	}
	else if (name == "treasure") {
		enterable = true;
		depth = 2;
		treasure_ID = genrand_int32() % KIND_OF_TREASURE + 1;
	}
	else if (name.compare(0, 8, "treasure") == 0) {
		treasure_ID = stoi(name.substr(8, name.size() - 8));
		name = "treasure";
		enterable = true;
		depth = 2;
	}
}

void Object::Action(const vector<Object> &obj, const Object &target) {

	if (name == "dodoro" && HP == DODORO_HP) return;

	if (RandomWalker) RandomWalk(obj);
	else if (BearWalker) BearWalk(obj, target);
	else if (UjimWalker) UjimWalk(obj, target);
	else if (isFrog) FrogWalk(obj, target);
	else if (isLeaf) {
		if (Distance2Mass(target) <= 25) {
			dir d = (name == "leaf") ? LeaveDir(obj, target) : ApproachDir(obj, target);
			d == RANDOM ? RandomWalk(obj) : Move(d);
		}
	}
	else if (KikuWalker) {
		RandomWalk(obj);
		UjimWalk(obj, target);
	}
	else if (name == "keme2"){
		Keme2Walk(obj, target);
	}
	else if (isHebi) HebiWalk(obj, target);
	else if (WWWalker) {
		if (10 <= wait && wait < 20) {
			RandomWalk(obj);
			UjimWalk(obj, target);
			wait++;
			if (wait == 20) wait = 0;
			return;
		}
		int X = x, Y = y;
		BearWalk(obj, target);
		BearWalk(obj, target);
		if (X == x && Y == y) wait++;
		else wait = 0;
	}
	else if (name == "last") {
		interval = 2 * HP / 3 + 8;
		wait++;
		int n = wait % 40;
		if (0 <= n && n <= 19) {
			dir d = (10 <= n && n <= 19) ? LeaveDir(obj, target) : ApproachDir(obj, target);
			d == RANDOM ? RandomWalk(obj) : Move(d);
		}
		else {
			if (n % 3 == 0) UjimWalk(obj, target);
			else RandomWalk(obj);
			BearWalk(obj, target);
		}
	}
}

void Object::RandomWalk(const vector<Object> &obj) {
	bool c[DIR_NUM];
	c[R] = CanMove(obj, R);
	c[L] = CanMove(obj, L);
	c[D] = CanMove(obj, D);
	c[U] = CanMove(obj, U);
	if (!c[R] && !c[L] && !c[D] && !c[U]) return;

	bool done[DIR_NUM] = { false };
	for (int i = 0; i < DIR_NUM; i++) done[i] = false;

	while (true) {
		int r = genrand_int32() % (DIR_NUM - 1) + 1;
		if (r == R && !done[R]) {
			if (c[R]) { Move(R); return; }
			else done[R] = true;
		}
		if (r == L && !done[L]) {
			if (c[L]) { Move(L); return; }
			else done[L] = true;
		}
		if (r == D && !done[D]) {
			if (c[D]) { Move(D); return; }
			else done[D] = true;
		}
		if (r == U && !done[U]) {
			if (c[U]) { Move(U); return; }
			else done[U] = true;
		}
	}
}

void Object::BearWalk(const vector<Object> &obj, const Object &target) {
	dir d = ApproachDir(obj, target);
	if (d == RANDOM) RandomWalk(obj);
	else Move(d);
}

dir Object::ApproachDir(const vector<Object> &obj, const Object &target) const {
	bool c[DIR_NUM];
	c[R] = CanMove(obj, R);
	c[L] = CanMove(obj, L);
	c[D] = CanMove(obj, D);
	c[U] = CanMove(obj, U);
	if (!c[R] && !c[L] && !c[D] && !c[U]) return 0;

	int px = target.x, py = target.y;

	int r = genrand_int32() % 2;
	auto dcd = [&c, &r](int d1, int d2) {
		if (c[d1] && c[d2]) return r ? d1 : d2;
		else if (c[d1]) return d1;
		else if (c[d2]) return d2;
		else return RANDOM;
	};
	if (px < x && py < y) return dcd(U, L);
	else if (px > x && py < y) return dcd(U, R);
	else if (px > x && py > y) return dcd(D, R);
	else if (px < x && py > y) return dcd(D, L);
	else if (px == x && py < y) return c[U] ? U : RANDOM;
	else if (px > x && py == y) return c[R] ? R : RANDOM;
	else if (px == x && py > y) return c[D] ? D : RANDOM;
	else if (px < x && py == y) return c[L] ? L : RANDOM;
	else return RANDOM;
}

dir Object::LeaveDir(const vector<Object> &obj, const Object &target) const {
	bool c[DIR_NUM];
	c[R] = CanMove(obj, R);
	c[L] = CanMove(obj, L);
	c[D] = CanMove(obj, D);
	c[U] = CanMove(obj, U);
	if (!c[R] && !c[L] && !c[D] && !c[U]) return NONE;

	int px = target.x, py = target.y;

	int r = genrand_int32() % 2;
	auto dcd = [&c, &r](int d1, int d2) {
		if (c[d1] && c[d2]) return r ? d1 : d2;
		else if (c[d1]) return d1;
		else if (c[d2]) return d2;
		else return RANDOM;
	};
	if (px > x && py > y) return dcd(U, L);
	else if (px < x && py > y) return dcd(U, R);
	else if (px < x && py < y) return dcd(D, R);
	else if (px > x && py < y) return dcd(D, L);
	else if (px == x && py > y) return c[U] ? U : RANDOM;
	else if (px < x && py == y) return c[R] ? R : RANDOM;
	else if (px == x && py < y) return c[D] ? D : RANDOM;
	else if (px > x && py == y) return c[L] ? L : RANDOM;
	else return RANDOM;
}

void Object::UjimWalk(const vector<Object> &obj, const Object &target) {
	if (Distance2Mass(target) > 25) RandomWalk(obj);
	else BearWalk(obj, target);
}

void Object::FrogWalk(const vector<Object> &obj, const Object &target) {
	if (wait > 0) {
		wait--;
		RandomWalk(obj);
		return;
	}
	if (FrogJump > 0) {
		FrogJump--;
		if (FrogJump != 0 && CanMove(obj, FrogJumpDir)) {
			Move(FrogJumpDir);
		}
		else {
			FrogJump = 0; // ジャンプ終了
			wait = name == "frog" ? FROG_WAIT : MARO_WAIT;
			interval = name == "frog" ? 40 : 30;
		}
	}
	else if (Distance2Mass(target) > 36) RandomWalk(obj);
	else {
		interval = name == "frog" ? 5 : 4;
		FrogJump = name == "frog" ? FROG_JUMP : MARO_JUMP;
		FrogJumpDir = ApproachDir(obj, target);
	}
}

void Object::Keme2Walk(const vector<Object> &obj, const Object &target) {
	if (wait > 0) {
		wait--;
		return;
	}
	dir d = ApproachDir(obj, target);
	if (!turn && d == R) {
		turn = true;
		wait = 3;
	}
	else if (turn && d == L) {
		turn = false;
		wait = 3;
	}
	else BearWalk(obj, target);
}

void Object::HebiWalk(const vector<Object> &obj, const Object &target) {
	if (--wait == 0) {
		wait = HEBI_WAIT;

		bool cant = true;
		bool can[SIZE_X - 2][SIZE_Y - 2] = { false };
		for (int i = 0; i < SIZE_X - 2; i++) {
			for (int j = 0; j < SIZE_Y - 2; j++) {
				int X = (i + 1)*IMAGE_SIZE;
				int Y = (j + 1)*IMAGE_SIZE;
				int d = target.Distance2Mass(X, Y);
				//printfDx("X=%4d,Y=%4d,d=%4d")
				if (
					8 <= d && d <= 13 &&
					CanMoveto(obj, X, Y)
					)
				{
					cant = false;
					can[i][j] = true;
				}
			}
		}

		if (cant) {
			BearWalk(obj, target);
			return;
		}

		while (true) {
			int X = genrand_int32() % (SIZE_X - 2);
			int Y = genrand_int32() % (SIZE_Y - 2);
			if (can[X][Y]) {
				x = (X + 1)*IMAGE_SIZE;
				y = (Y + 1)*IMAGE_SIZE;
				return;
			}
		}
	}
	else {
		BearWalk(obj, target);
	}
}

void Object::Move(const dir d) {
	tie(x, y) = Moved(x, y, d, dist);
	if (d == R) turn = true;
	else if (d == L) turn = false;
}

bool Object::CanMove(const vector<Object> & obj, const dir d) const {
	return CanMove(obj, d, -1);
}

bool Object::CanMove(const vector<Object> & obj, const dir d, const int exc) const {
	if (d == NONE) return true;
	int X, Y;
	tie(X, Y) = Moved(x, y, d, dist);
	return CanMoveto(obj, X, Y, exc);
}

bool Object::CanMoveto(const vector<Object> & obj, const int X, const int Y) const {
	return CanMoveto(obj, X, Y, -1);
}

bool Object::CanMoveto(const vector<Object> & obj, const int X, const int Y, const int exc) const {
	for (int i = 0; i < obj.size(); i++) {

		if (i == exc) continue;

		if (
			obj[i].touch(X, Y, size) &&
			!obj[i].enterable
			) {

			if (
				(obj[i].name == "crystal" && CanPassCrystal) || //水晶を通れるか
				(obj[i].isWater && CanSwim) || //水の中に入れるか
				(obj[i].name == "flame" && CanPassFlame) || //火の中に入れるか
				(obj[i].name == "electricity" && CanPassElc) || //電気の中に入れるか
				(obj[i].name == "poison" && CanPassPoison) //毒の中に入れるか
				)
			{
				continue;
			}

			//ピクミンはポンガシの上を通れる
			if (
				isPlayer &&
				name != "yellowb" &&
				obj[i].isPon
				)
			{
				continue;
			}

			//Pスイッチが押せるならば通れる
			if (
				isPlayer &&
				obj[i].isPswitch
				)
			{
				continue;
			}
			
			//一方通行ゲート
			if (obj[i].isOneWayGate) {
				int Dir;
				if (obj[i].name == "right") Dir = R;
				else if (obj[i].name == "left") Dir = L;
				else if (obj[i].name == "up") Dir = U;
				else Dir = D;

				if (touch(obj[i])) continue; //既に触れている場合は通れる
				else if (touchjust(obj[i]) == Dir) continue;
			}

			return false;
		}
	}
	return true;
}

bool Object::touch(const int X, const int Y) const {
	return touch(X, Y, 1);
}

bool Object::touch(const int X, const int Y, const int Size) const {

	return (
		abs(x - X) < (size / 2.0 + Size / 2.0)*IMAGE_SIZE &&
		abs(y - Y) < (size / 2.0 + Size / 2.0)*IMAGE_SIZE
		);
}

bool Object::touch(const Object &obj) const {
	return (
		abs(x - obj.x) < (size / 2.0 + obj.size / 2.0)*IMAGE_SIZE &&
		abs(y - obj.y) < (size / 2.0 + obj.size / 2.0)*IMAGE_SIZE
		);
}

bool Object::touch(const Object &obj, const float d) const {
	return (
		abs(x - obj.x) < (size / 2.0 + obj.size / 2.0 + d)*IMAGE_SIZE &&
		abs(y - obj.y) < (size / 2.0 + obj.size / 2.0 + d)*IMAGE_SIZE
		);
}

dir Object::touchjust(const Object &obj) const {
	if (abs(x - obj.x) == IMAGE_SIZE && abs(y - obj.y) < IMAGE_SIZE) {
		return x < obj.x ? R : L;
	}
	if (abs(x - obj.x) < IMAGE_SIZE&&abs(y - obj.y) == IMAGE_SIZE) {
		return y < obj.y ? D : U;
	}
	return NONE;
}

int Object::adjX() const {
	return (x + IMAGE_SIZE / 2) / IMAGE_SIZE * IMAGE_SIZE;
}

int Object::adjY() const {
	return (y + IMAGE_SIZE / 2) / IMAGE_SIZE * IMAGE_SIZE;
}

int Object::Distance2(const int X, const int Y) const {
	return (x - X)*(x - X) + (y - Y)*(y - Y);
}

int Object::Distance2(const Object &obj) const {
	return Distance2(obj.x, obj.y);
}

int Object::Distance2Mass(const int X, const int Y) const {
	return Distance2(X, Y) / (IMAGE_SIZE*IMAGE_SIZE);
}

int Object::Distance2Mass(const Object &obj) const {
	return Distance2Mass(obj.x, obj.y);
}

void Object::EnemySet() {
	enterable = true;
	depth = 0;
	isEnemy = true;
}

bool Object::PlayerCanUsePon(const vector<Object> &obj) const {
	return (
		isPon &&
		name != "yellowb" &&
		name.compare(0, name.size() - 3, PLAYER.name) != 0
		);
}