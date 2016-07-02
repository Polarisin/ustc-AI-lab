//A* h2
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <unordered_set>
#include <queue>
using namespace std;
typedef struct Node{
	char state[28];
	unsigned int f, g;
	char *path;
	unsigned char sp;
	bool lessthan (const Node* node) const {
		return g < node->g;
	}
}Node;
class CompareNode{
public:
	bool operator () (Node* node1,Node* node2){
		return node1->lessthan(node2);
	}
};
static size_t sizeofNode = sizeof(Node);
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
static int dircsp[] = { 9, -9, 1, -1, 3, -3 };
static char dircpt[] = { 'B', 'F', 'R', 'L', 'D', 'U' };
static char valueH2[27][27];
static priority_queue<Node*, vector<Node*>, CompareNode> leaf[2];
static priority_queue<Node*, vector<Node*>, CompareNode> tempList;
static unordered_set<string> innode;
static unordered_set<string>::iterator endit;
static Node *goal, *last;
char* ASH2(){
	int i;
	//initial root
	Node *root = (Node*)malloc(sizeofNode);
	memcpy(root->state, state0, 27);
	int h = 0;
	int len = 0;
	unsigned char opbyte, lastop = 0;
	root->state[27] = 0;
	root->g = 0;
	int t;
	for (i = 0; i < 27; i++){
		t = state0[i] ^ 0x20;
		if (t > 0){
			h += valueH2[t][i];
		}
		else if (t == 0){
			root->sp = i;
		}
	}
	root->f = h;
	root->path = (char *)calloc(1, 1);
	//astar
	goal = root;
	endit = innode.end();
	while (goal->f != len){
		if (len > 0){
			switch (goal->path[len - 1]){
			case 'U':lastop = 0x10; break;
			case 'D':lastop = 0x20; break;
			case 'L':lastop = 0x04; break;
			case 'R':lastop = 0x08; break;
			case 'F':lastop = 0x01; break;
			case 'B':lastop = 0x02; break;
			}
		}
		opbyte = oper[goal->sp] ^ lastop;
		for (i = 5; i >= 0; i--){
			if ((opbyte >> i) & 0x01){
				last = (Node*)malloc(sizeofNode);
				memcpy(last->state, goal->state, 28);
				last->path = (char *)malloc(len + 2);
				memcpy(last->path, goal->path, len);
				last->sp = goal->sp + dircsp[i];
				last->state[goal->sp] = last->state[last->sp];
				last->state[last->sp] = 0x20;
				last->path[len] = dircpt[i];
				last->path[len + 1] = 0;
				last->g = len + 1;
				h = valueH2[goal->state[last->sp] ^ 0x20][goal->sp] - valueH2[goal->state[last->sp] ^ 0x20][last->sp];
				last->f = goal->f + h + 1;
				leaf[(h+1)>>1].push(last);
				last = NULL;
			}
		}
		innode.insert(string(goal->state + 1));
		do{
			free(goal->path);
			free(goal);
			if (leaf[0].empty()){
				tempList = leaf[0];
				leaf[0] = leaf[1];
				leaf[1] = tempList;
			}
			goal = leaf[0].top();
			leaf[0].pop();
		} while (innode.find(string(goal->state + 1)) != endit);
		len = goal->g;
	}
	cout << leaf[0].size() << endl;
	cout << leaf[1].size() << endl;
	cout << innode.size() << endl;
	return goal->path;
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
void destory(){
	free(goal->path);
	free(goal);
	goal = NULL;
// 	leaf[0].clear();
// 	leaf[1].clear();
	innode.clear();
}
void print(char * state){
	for (int i = 0; i < 27;){
		cout << " " << (int)(state[i++] ^ 0x20);
		if (i % 3 == 0)	cout << endl;
		if (i % 9 == 0) cout << endl;
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
	state_initial_console();
	int select = 1;
//	cin >> select;
	if (select == 1){
		clock_t start, finish;
		double totaltime;
		start = clock();
		char *path = ASH2();
		finish = clock();
		cout << "step=" << strlen(path) << endl;
		cout << path << endl;
		totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << totaltime << endl;
		destory();
	}
	else{
		string path;
		cin >> path;
		check(path);
	}
	system("pause");
	return 0;
}