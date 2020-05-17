#include<iostream>
#include<string>
#include<vector>
using namespace std;

string state[4] = { "SN","WN","WT","ST" };
int Reg[32];

struct pred {
	string n;
	int s[4];
	int mis;
};

int main() {
	int entries;
	cin >> entries;
	string buffer;
	string inst[1000];
	string str[1000];
	int k = 0;		//第幾個beq,entry
	int pc = 0;			//第幾行
	int entry[1000];
	int label[1000];
	pred p[1000];
	int lb = 1;
	typedef pair<string, int> par;
	vector<par> branch;

	while (getline(cin, buffer)) {  //init
		if (buffer == "-1")
			break;
		if (buffer == "")
			continue;
		str[pc] = buffer;
		int pos;
		pos = buffer.find(" ");
		string x = buffer.substr(0, pos);
		if (x == "beq") {
			branch.push_back(par(buffer, k));
			inst[k] = buffer;
			entry[k] = pc % entries;
			for (int i = 0; i <= 3;i++)
				p[entry[k]].s[i] = 0;
			p[entry[k]].n = "00";
			p[entry[k]].mis = 0;
			k++;
		}

		pos = buffer.find(":");
		if (pos != -1) {
			label[lb] = pc;
			lb++;
			continue;
		}

		pc++;
	}

	for (int j = 0; j < pc;j++) {
		int pos;
		buffer = str[j];
		pos = buffer.find(" ");
		string x = buffer.substr(0, pos);
		if (x == "beq") {
			pos = buffer.find("R");
			int pos2 = buffer.find(",");
			string m = buffer.substr(pos + 1, pos2 - pos - 1);
			int r1 = stoi(m);

			buffer.erase(0, pos2 + 1);
			pos = buffer.find("R");
			pos2 = buffer.find(",");
			m = buffer.substr(pos + 1, pos2 - pos - 1);
			int r2 = stoi(m);

			buffer.erase(0, pos2 + 1);
			m = buffer.substr(1, buffer.size());
			int la = stoi(m);

			int idx;
			for (int i = 0;i < branch.size();i++) {
				if (branch[i].first == str[j]) {
					idx = branch[i].second;
					cout << "entry: " << entry[idx] << "　　　" << inst[idx] << endl;
					cout << "(" << p[entry[idx]].n << ", " << state[p[entry[idx]].s[0]] << ", " << state[p[entry[idx]].s[1]] << ", " << state[p[entry[idx]].s[2]] << ", " << state[p[entry[idx]].s[3]] << ")";
				}
			}


		}
	}
}