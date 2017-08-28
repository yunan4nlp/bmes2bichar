#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

const static string no_tag = "NOTAG";
const static string no_char = "NOCHAR";

struct Instance {
	vector<string> m_chars;
	vector<string> m_tags;
	int m_size;

	void clear() {
		m_chars.clear();
		m_tags.clear();
		m_size = 0;
	}

	void resize(int size) {
		m_chars.resize(size);
		m_tags.resize(size);
		m_size = size;
	}

	void save(ofstream &out) const {
		for (int idx = 0; idx < m_size; idx++) {
			if (idx + 1 < m_size)
				out << m_chars[idx] << "@$" << m_chars[idx + 1] << "###" << m_tags[idx] << "@$" << m_tags[idx + 1] << " ";
			else
				out << m_chars[idx] << "@$" << no_char << "###" << m_tags[idx] << "@$" << no_tag;
		}
		out << endl;
	}
};

void split_bychar(const string& str, vector<string>& vec, const char separator);
void getInstance(const string& line, Instance& inst);

int main(int argc, char* argv[]) {
	ifstream in(argv[1]);
	ofstream out(argv[2]);
	if (in.is_open() && out.is_open()) {
		string line;
		Instance inst;
		while(getline(in, line)){
			getInstance(line, inst);
			inst.save(out);
		}
		cout << "transfer ok!" << endl;
		in.close();
		out.close();
	}
	return 0;
}

void split_bychar(const string& str, vector<string>& vec, const char separator = ' ') {
	//assert(vec.empty());
	vec.clear();
	string::size_type pos1 = 0, pos2 = 0;
	string word;
	while ((pos2 = str.find_first_of(separator, pos1)) != string::npos) {
		word = str.substr(pos1, pos2 - pos1);
		pos1 = pos2 + 1;
		if (!word.empty())
			vec.push_back(word);
	}
	word = str.substr(pos1);
	if (!word.empty())
		vec.push_back(word);
}

void getInstance(const string& line, Instance& inst) {
	inst.clear();
	vector<string> info;
	split_bychar(line, info, ' ');
	int pos, char_len = info.size();
	inst.resize(char_len);
	for (int idx = 0; idx < char_len; idx++) {
		const string& cur_info = info[idx];
		pos = cur_info.find_last_of('_');
		pos = cur_info.find_last_of('_', pos - 1);
		inst.m_chars[idx] = cur_info.substr(0, pos);
		inst.m_tags[idx] = cur_info.substr(pos + 1, -1);
	}
}
