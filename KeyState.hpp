#include "DxLib.h"

class KeyState {
public:
	bool d, a, s, w, c, enter;
	KeyState() {
		d = CheckHitKey(KEY_INPUT_D);
		a = CheckHitKey(KEY_INPUT_A);
		s = CheckHitKey(KEY_INPUT_S);
		w = CheckHitKey(KEY_INPUT_W);
		enter = CheckHitKey(KEY_INPUT_RETURN);
	}
	KeyState(bool n) {
		d = a = s = w = enter = n;
	}
};