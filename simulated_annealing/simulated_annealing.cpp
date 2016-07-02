#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <cmath>

using namespace std;
int N;
int hazardx, hazardy;
int valueState;
int *rightDown, *leftDown;
int *queen;
double origialTemperature;
int triesPT;
double coldRate;
double temperature;

//随机数产生器
random_device rd;   // non-deterministic generator
mt19937 gen(rd());  // to seed mersenne twister.
uniform_int_distribution<> dist(0, 0x7fffffff); // distribute results between 0 and 0x7fffffff inclusive.

void SWAP(int &a, int &b) {
	if (&a == &b)	return;
	a ^= b ^= a ^= b;
}
void initial(){
	rightDown = (int *)calloc(2 * N - 1, sizeof(int));
	leftDown = (int *)calloc(2 * N - 1, sizeof(int));
	queen = (int *)calloc(N, sizeof(int));
	origialTemperature = 50;
	triesPT = 300;
	coldRate = 0.8;
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
void initialState(){
	int i, Part = 30;
	i = 10;
	while ((i *= 10) < N){
		Part += 20;
	}
	for (i = 0; i < 2 * N - 1; ++i){
		leftDown[i] = 0;
		rightDown[i] = 0;
	}
	for (i = 0; i < N; ++i){
		queen[i] = i;
	}
	for (i = 0; i < N - Part; ++i) {
		do {
			SWAP(queen[i], queen[dist(gen) % (N - i) + i]);
		} while (leftDown[i + queen[i]] != 0 || rightDown[i + N - queen[i] - 1] != 0);
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
void nextStep(int cq){
	int sq = dist(gen) % N;
	int oldValue = valueState;
	int minValue = 2147483647;
	int minLine = cq;
	for (int sq = 0; sq < N; ++sq){
		if (sq == cq){
			continue;
		}
		trySwap(cq, sq);
		if (valueState <= minValue){
			minValue = valueState;
			minLine = sq;
		}
		backSwap(cq, sq);
		valueState = oldValue;
	}
	if (minValue <= valueState || (minValue > oldValue && ((double)(dist(gen) % 1000) / 1000.0 < exp((minValue - oldValue) / temperature)))){
		trySwap(cq, minLine);
	}
}
int findMaxValue(){
	int value = 0, index, temp;
	for (int i = 0; i < N; i++){
		temp = leftDown[i + queen[i]] + rightDown[i + N - queen[i] - 1];
		if (temp > value){
			value = temp;
			index = i;
		}
	}
	return index;
}
void simulated_annealing(){
	int cq, step;
	initialState();
	step = 0;
	temperature = origialTemperature;
	while (valueState > 0){
		cq = findMaxValue();
		nextStep(cq);
		step++;
		if (step >= triesPT){
			step = 0;
			temperature *= coldRate;
		}
	}
	return;
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
	initial();
	clock_t start, finish;
	double totaltime;
	start = clock();

	simulated_annealing();

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	//	show();
	cout << totaltime << endl;
	system("pause");
	return 0;
}