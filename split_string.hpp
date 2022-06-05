#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

vector<string> split_string(string target, const char c) {
	replace(target.begin(), target.end(), c, ' ');
	istringstream iss(target);

	vector<string> ret;
	while (true) {
		string buf;
		iss >> buf;
		if (buf == "") break;
		ret.push_back(buf);
	}
	return ret;
}