#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
using namespace std;
static int N;
static int hazardx, hazardy;
static int valueState;
static int C, C2;
static double C1 = 0.45;
static int *rightDown, *leftDown, *lineValue;
static int *queen;

static int *fightQueen;
static int fightQueenNum;

//随机数产生器
random_device rd;   // non-deterministic generator
mt19937 gen(rd());  // to seed mersenne twister.
uniform_int_distribution<> dist(0, 0x7fffffff); // distribute results between 0 and 0x7fffffff inclusive.

void SWAP(int &a, int &b) {
	if (&a == &b)	return;
	a ^= b ^= a ^= b;
}
void init(){
	srand((unsigned int)time(NULL));
	rightDown = (int *)calloc(2 * N - 1, sizeof(int));
	leftDown = (int *)calloc(2 * N - 1, sizeof(int));
	lineValue = (int *)calloc(N, sizeof(int));
	queen = (int *)calloc(N, sizeof(int));
	fightQueen = (int *)calloc(N, sizeof(int));
	fightQueenNum = 0;
	if (N >= 1000000){
		C2 = 1;
		C = 100;
	}
	else if (N >= 100000){
		C2 = 1;
		C = 80;
	}
	else if (N >= 10000){
		C2 = 1;
		C = 50;
	}
	else if (N >= 1000){
		C2 = 16;
		C = 50;
	}
	else if (N >= 100){
		C2 = 32;
		C = 50;
	}
	else if (N >= 10){
		C2 = 32;
		C = 30;
	}
	else{
		C2 = 32;
		C = 8;
	}
}
void show(){
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < N; ++j){
			if (queen[i] == j){
				cout << "Ｑ";
			}
			else{
				cout << "·";
			}
		}
		cout << endl;
	}
	cout << endl;
}
bool hasFight(){
	for (int i = 0; i < 2 * N - 1; ++i){
		if (leftDown[i] > 1 || rightDown[i]>1){
			return true;
		}
	}
	return false;
}
void initialState(){
	int i;
	for (i = 0; i < 2 * N - 1; ++i){
		leftDown[i] = 0;
		rightDown[i] = 0;
	}
	for (i = 0; i < N; ++i){
		queen[i] = i;
	}
	for (i = 0; i < N - C; ++i) {
		do {
			SWAP(queen[i], queen[dist(gen) % (N - i) + i]);
		} while (leftDown[i + queen[i]] != 0 || rightDown[i + N - queen[i]- 1] != 0);
		++leftDown[i + queen[i]];
		++rightDown[i + N - queen[i] - 1];
	}
	while (i < N) {
		SWAP(queen[i], queen[dist(gen) % (N - i) + i]);
		++leftDown[i + queen[i]];
		++rightDown[i + N - queen[i] - 1];
		++i;
	}
	valueState = 0;
	for (int i = 0; i < 2 * N - 1; ++i){
		valueState += (leftDown[i] * (leftDown[i] - 1) + rightDown[i] * (rightDown[i] - 1)) / 2;
	}
}
void setFight(){
	fightQueenNum = 0;
	for (int i = 0; i < N; ++i){
		if (rightDown[i + N - queen[i] - 1] > 1 || leftDown[i + queen[i]] > 1){
			fightQueen[fightQueenNum++] = i;
		}
	}
}
void trySwap(int a, int b){
	valueState -= leftDown[a + queen[a]] + leftDown[b + queen[b]] + rightDown[a + N - queen[a] - 1] + rightDown[b + N - queen[b] - 1];
	--leftDown[a + queen[a]];
	--leftDown[b + queen[b]];
	++leftDown[a + queen[b]];
	++leftDown[b + queen[a]];
	--rightDown[a + N - queen[a] - 1];
	--rightDown[b + N - queen[b] - 1];
	++rightDown[a + N - queen[b] - 1];
	++rightDown[b + N - queen[a] - 1];
	SWAP(queen[a], queen[b]);
	valueState += leftDown[a + queen[a]] + leftDown[b + queen[b]] + rightDown[a + N - queen[a] - 1] + rightDown[b + N - queen[b] - 1];
}
void backSwap(int a, int b){
	--leftDown[a + queen[a]];
	--leftDown[b + queen[b]];
	++leftDown[a + queen[b]];
	++leftDown[b + queen[a]];
	--rightDown[a + N - queen[a] - 1];
	--rightDown[b + N - queen[b] - 1];
	++rightDown[a + N - queen[b] - 1];
	++rightDown[b + N - queen[a] - 1];
	SWAP(queen[a], queen[b]);
}
void QS4(){
	int loopStep, cq, sq, limit, oldValue;
	do{
		valueState = 0;
		initialState();
		setFight();
		limit = C1 * valueState;
		loopStep = 0;
		while (loopStep <= C2 * N && valueState > 0){
			for (int i = 0; i < fightQueenNum; ++i){
				oldValue = valueState;
				cq = fightQueen[i];
				sq = dist(gen) % N;
				if (sq == cq){
					++sq;
					sq = sq % N;
				}
				trySwap(cq, sq);
				if (valueState==0)
					break;
				if (oldValue <= valueState){
					backSwap(cq, sq);
					valueState = oldValue;
				}
				else if (valueState < limit){
					limit *= C1;
					setFight();
				}
			}
			loopStep += fightQueenNum;
		}
	} while (valueState > 0);
	//////////////////////////////////////////////////////////////////////////
	if (N % 2 == 1 && queen[hazardx] == hazardy&&hazardx * 2 + 1 == N&&hazardy * 2 + 1 == N){
		QS4();
	}
	return;
}
void outputFile(double time){
	ofstream file("output_QS4.txt");
	if (queen[hazardx] == hazardy){
		if (hazardx * 2 + 1 == N){
			for (int i = 0; i < N; ++i){
				file << N - 1 - queen[i] << endl;
			}
		}
		else{
			for (int i = N - 1; i >= 0; --i){
				file << queen[i] << endl;
			}
		}
	}
	else{
		for (int i = 0; i < N; ++i){
			file << queen[i] << endl;
		}
	}
	file << time << endl;
	file.close();
}
int main(){
	/*
	ifstream input("input.txt");
	input >> N;
	input >> hazardx >> hazardy;
	input.close();
	*/
	cin >> N;
//	cin >> hazardx >> hazardy;
	init();
	clock_t start, finish;
	double totaltime;
	start = clock();

	QS4();

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
//	show();
	cout << totaltime << endl;
	outputFile(totaltime);
	system("pause");
	return 0;
}