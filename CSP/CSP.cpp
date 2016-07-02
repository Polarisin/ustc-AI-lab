#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;
int N;
int hazardx, hazardy;
int *rightDown, *leftDown, *updown, *lineValue;
int *queen;

void init(){
	srand((unsigned int)time(NULL));
	rightDown = (int *)calloc(2 * N - 1, sizeof(int));
	leftDown = (int *)calloc(2 * N - 1, sizeof(int));
	updown = (int *)calloc(N, sizeof(int));
	lineValue = (int *)calloc(N, sizeof(int));
	queen = (int *)calloc(N, sizeof(int));
}
void show(){
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++){
			if (queen[i] == j){
				cout << "£Ñ";
			}
			else if (hazardx == i&&hazardy == j){
				cout << "£Ø";
			}
			else{
				cout << "¡¤";
			}
		}
		cout << endl;
	}
	cout << endl;
}
bool hasFight(){
	for (int i = 0; i < N; i++){
		if (updown[i] != 1){
			return true;
		}
	}
	for (int i = 0; i < 2 * N - 1; i++){
		if (leftDown[i] > 1 || rightDown[i]>1){
			return true;
		}
	}
	return false;
}
void CSP(){
	for (int i = 0; i < N; i++){
		queen[i] = rand() % N;
		leftDown[i + queen[i]]++;
		rightDown[i + N - queen[i] - 1]++;
		updown[queen[i]]++;
	}
	//	show();
	int minValue, minValuePos;
	while (hasFight()){
		for (int i = 0; i < N; i++){
			updown[queen[i]]--;
			leftDown[i + queen[i]]--;
			rightDown[i + N - queen[i] - 1]--;
			for (int j = 0; j < N; j++){
				lineValue[j] = updown[j] + leftDown[i + j] + rightDown[i + N - j - 1];
			}
			minValue = 2147483647;
			minValuePos = -1;
			for (int j = 0; j < N; j++){
				if (j != queen[i] && lineValue[j] <= minValue){
					minValue = lineValue[j];
					minValuePos = j;
				}
			}
			if ((minValue < lineValue[queen[i]]) || (minValue == lineValue[queen[i]] && (rand() & 0x01))){
				queen[i] = minValuePos;
			}
			updown[queen[i]]++;
			leftDown[i + queen[i]]++;
			rightDown[i + N - queen[i] - 1]++;
			if (!hasFight())	break;
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
	init();
	clock_t start, finish;
	double totaltime;
	start = clock();

	CSP();

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	//	show();
	cout << totaltime << endl;
	system("pause");
	return 0;
}