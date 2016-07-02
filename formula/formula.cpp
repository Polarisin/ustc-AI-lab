#include <iostream>
#include <ctime>
using namespace std;
int N;
int *queen;
void initial(){
	queen = (int *)calloc(N, sizeof(int));
}
void show(){
	for (int i = 0; i < N; ++i){
		for (int j = 0; j < N; ++j){
			if (queen[i] == j){
				cout << "£Ñ";
			}
			else{
				cout << "¡¤";
			}
		}
		cout << endl;
	}
	cout << endl;
}
void formula(){
	if (N % 6 != 2 && N % 6 != 3){
		int x = 0, y = 1;
		while (y < N){
			queen[x] = y;
			x += 1;
			y += 2;
		}
		y = 0;
		while (x < N){
			queen[x] = y;
			x += 1;
			y += 2;
		}
	}
	else{
		if (N % 2 == 0){
			int k = N / 2;
			if (k % 2 == 0){
				int x = 0, y = k - 1;
				while (y < N){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = 1;
				while (y < k - 2){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = k + 2;
				while (y < N - 1){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = 0;
				while (y < k + 1){
					queen[x] = y;
					x += 1;
					y += 2;
				}
			}
			else{
				int x = 0, y = k - 1;
				while (y < N - 1){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = 0;
				while (y < k - 2){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = k + 2;
				while (y < N){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = 1;
				while (y < k + 1){
					queen[x] = y;
					x += 1;
					y += 2;
				}
			}
		}
		else{
			int k = (N - 1) / 2;
			if (k % 2 == 0){
				int x = 0, y = k - 1;
				while (y < N - 1){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = 1;
				while (y < k - 2){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = k - 2;
				while (y < N - 2){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = 0;
				while (y < k + 1){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				queen[x] = N - 1;
			}
			else{
				int x = 0, y = k - 1;
				while (y < N - 2){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = 0;
				while (y < k - 2){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = k + 2;
				while (y < N - 1){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				y = 1;
				while (y < k + 1){
					queen[x] = y;
					x += 1;
					y += 2;
				}
				queen[x] = N - 1;
			}
		}
	}
}
int main(){
	cin >> N;
	initial();

	clock_t start, finish;
	double totaltime;
	start = clock();

	formula();

	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
//	show();
	cout << totaltime << endl;
	return 0;
}