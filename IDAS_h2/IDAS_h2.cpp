#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#define limit 200
using namespace std;
typedef struct {
	char state[28];
	unsigned int f, g, h;
	unsigned char sp;
	char path[100];
}Node;
static char state0[27];
static char stateN[27];
static unsigned char oper[27] = {	//U,D,L,R,F,B
	0x15, 0x1d, 0x19,
	0x35, 0x3d, 0x39,
	0x25, 0x2d, 0x29,

	0x17, 0x1f, 0x1b,
	0x37, 0x3f, 0x3b,
	0x27, 0x2f, 0x2b,

	0x16, 0x1e, 0x1a,
	0x36, 0x3e, 0x3a,
	0x26, 0x2e, 0x2a
};
static char dircsp[] = { 9, -9, 1, -1, 3, -3 };
static char dircpt[] = { 'B', 'F', 'R', 'L', 'D', 'U' };
static char valueH2[27][27];
Node p;

void print(char * state){
	for (int i = 0; i < 27;){
		cout << " " << (int)(state[i++] ^ 0x20);
		if (i % 3 == 0)	cout << endl;
		if (i % 9 == 0) cout << endl;
	}
}
char* IDASH2(){
	memcpy(p.state, state0, 27);
	p.state[27] = 0;
	unsigned int rooth = 0, limitf;
	char t, opbyte, lastop, child, dh;
	unsigned char index;
	for (index = 0; index < 27; index++){
		t = state0[index] ^ 0x20;
		if (t > 0){
			rooth += valueH2[t][index];
		}
		else if (t == 0){
			p.sp = index;
		}
	}
	limitf = p.f = p.h = rooth;
	p.g = 0;
	p.path[0] = 0;
	while (limitf < limit){
		lastop = 0x00;
		child = 5;
		while (true){
			opbyte = oper[p.sp] ^ lastop;
			while (child >= 0){
				if (p.h == 0){
					for (unsigned int i = 1; i <= p.g; i++){
						p.path[i] = dircpt[p.path[i] - 1];
					}
					p.path[p.g + 1] = 0;
					return p.path + 1;
				}
				if ((opbyte >> child) & 0x01){
					//计算操作child的dh
					index = p.sp + dircsp[child];
					dh = valueH2[p.state[index] ^ 0x20][p.sp] - valueH2[p.state[index] ^ 0x20][index];
					if (p.f + dh + 1 <= limitf){//在当前阈值内
						//更新结点
						p.state[p.sp] = p.state[index];
						p.state[index] = 0x20;
						p.sp = index;
						p.g++;
						p.h += dh;
						p.f += dh + 1;
						p.path[p.g] = child + 1;
						//修改其他环境变量
						lastop = (0x01 << (child ^ 0x01));
						opbyte = oper[p.sp] ^ lastop;
						child = 5;
						//						print(p.state);
						continue;
					}
				}
				child--;
			}
			if (p.h == rooth&&p.g == 0){
				lastop = 0x00;
				break;
			}
			//回溯
			child = p.path[p.g] - 1;
			index = p.sp - dircsp[child];
			dh = valueH2[p.state[index] ^ 0x20][p.sp] - valueH2[p.state[index] ^ 0x20][index];
			p.g--;
			p.h += dh;
			p.f += (dh - 1);
			p.state[p.sp] = p.state[index];
			p.state[index] = 0x20;
			p.sp = index;
			child--;
			if (p.h == rooth&&p.g == 0){
				lastop = 0x00;
			}
			else{
				lastop = (0x01 << ((p.path[p.g] - 1) ^ 0x01));
			}
			//			print(p.state);
		}
		limitf += 2;
	}
	return NULL;
}
void state_initial_file(){
	ifstream source("source.txt"), target("target.txt");
	int t, i;
	for (i = 0; i < 27; i++){
		source >> t;
		state0[i] = (char)(t ^ 0x20);
		if (t == -1){
			if ((oper[i] >> 5) & 0x01){
				oper[i - 3] ^= 0x10;
			}
			if ((oper[i] >> 4) & 0x01){
				oper[i + 3] ^= 0x20;
			}
			if ((oper[i] >> 3) & 0x01){
				oper[i - 1] ^= 0x04;
			}
			if ((oper[i] >> 2) & 0x01){
				oper[i + 1] ^= 0x08;
			}
			if ((oper[i] >> 1) & 0x01){
				oper[i - 9] ^= 0x01;
			}
			if ((oper[i] >> 0) & 0x01){
				oper[i + 9] ^= 0x02;
			}
		}
		target >> t;
		stateN[i] = (char)t;
	}
	source.close();
	target.close();
	unsigned char apos, bpos, cpos;
	memset(valueH2, 0, 27 * 27);
	for (i = 0; i < 27; i++){
		if (stateN[i] > 0){
			apos = i / 9;
			bpos = i % 3;
			cpos = i / 3 % 3;
			for (int a = 0; a < 3; a++){
				for (int b = 0; b < 3; b++){
					for (int c = 0; c < 3; c++){
						valueH2[stateN[i]][a * 9 + b + c * 3] = abs(apos - a) + abs(bpos - b) + abs(cpos - c);
					}
				}
			}
		}
		stateN[i] ^= 0x20;
	}
}
void state_initial_console(){
	int t, i, j = 1;
	for (i = 0; i < 27; i++){
		cin >> t;
		state0[i] = (char)(t ^ 0x20);
		if (t == -1){
			stateN[i] = -1;
			//initial operate
			if ((oper[i] >> 5) & 0x01){
				oper[i - 3] ^= 0x10;
			}
			if ((oper[i] >> 4) & 0x01){
				oper[i + 3] ^= 0x20;
			}
			if ((oper[i] >> 3) & 0x01){
				oper[i - 1] ^= 0x04;
			}
			if ((oper[i] >> 2) & 0x01){
				oper[i + 1] ^= 0x08;
			}
			if ((oper[i] >> 1) & 0x01){
				oper[i - 9] ^= 0x01;
			}
			if ((oper[i] >> 0) & 0x01){
				oper[i + 9] ^= 0x02;
			}
			oper[i] = 0x00;
		}
		else {
			stateN[i] = j;
			j++;
		}
	}
	while (i > 0 && stateN[--i] < 0);
	stateN[i] = 0;
	unsigned char apos, bpos, cpos;
	memset(valueH2, 0, 27 * 27);
	for (i = 0; i < 27; i++){
		if (stateN[i] > 0){
			apos = i / 9;
			bpos = i % 3;
			cpos = i / 3 % 3;
			for (int a = 0; a < 3; a++){
				for (int b = 0; b < 3; b++){
					for (int c = 0; c < 3; c++){
						valueH2[stateN[i]][a * 9 + b + c * 3] = abs(apos - a) + abs(bpos - b) + abs(cpos - c);
					}
				}
			}
		}
		stateN[i] ^= 0x20;
	}
}
void check(string opra){
	int spindex, ni, step = opra.length();
	for (spindex = 0; spindex < 27; spindex++){
		if (state0[spindex] == 0x20){
			break;
		}
	}
	for (int i = 0; i < step; i++){
		switch (opra[i]){
		case 'U':ni = spindex - 3; break;
		case 'D':ni = spindex + 3; break;
		case 'L':ni = spindex - 1; break;
		case 'R':ni = spindex + 1; break;
		case 'F':ni = spindex - 9; break;
		case 'B':ni = spindex + 9; break;
		default:break;
		}
		state0[spindex] = state0[ni];
		state0[ni] = 0x20;
		spindex = ni;
	}
	cout << endl;
	print(state0);
}
void generate(int step){
	srand((unsigned int)time(NULL));
	char i, c, spindex;
	for (i = 0; i < 27; i++){
		state0[i] = stateN[i];
	}
	i = spindex = 26;
	while (step > 0){
		c = (rand() % 6);
		if ((oper[i] >> c) & 0x01){
			step--;
			switch (c){
			case 5:spindex -= 3; cout << "U"; break;
			case 4:spindex += 3; cout << "D"; break;
			case 3:spindex -= 1; cout << "L"; break;
			case 2:spindex += 1; cout << "R"; break;
			case 1:spindex -= 9; cout << "F"; break;
			case 0:spindex += 9; cout << "B"; break;
			default:break;
			}
			state0[i] = state0[spindex];
			state0[spindex] = 0;
			i = spindex;
		}
	}
	cout << endl;
	print(state0);
}
int main(int argc, char** argv){
	state_initial_file();
	int select = 1;
	//	cin >> select;
	if (select == 1){
		clock_t start, finish;
		double totaltime;
		start = clock();
		char *path = IDASH2();
		finish = clock();
		ofstream output("output_IDAh2.txt");
		totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
		output << totaltime << endl;
		if (path == NULL){
			output << "Can't find path" << endl;
		}
		else {
			output << path << endl;
		}
		output.close();
	}
	else{
		string path;
		cin >> path;
		check(path);
	}
	system("pause");
	return 0;
}