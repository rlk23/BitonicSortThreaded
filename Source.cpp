
#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace literals::chrono_literals;
class Timer {
public:
	system_clock::time_point start;
	Timer() {
		start = system_clock::now();
	}

	~Timer() {
		cout << "runtime: " << duration_cast<milliseconds>(system_clock::now() - start).count() << " ms" << endl;
	}
};

void bitonic(vector<int>& A, int n1, int n2, bool Inc);
void combine(vector<int>& A, int n1, int n2, bool Inc);
void preCombine(vector<int>& A, int n1, int n2, bool Inc, int Size);

int main() {

	int n{ int(pow(2,22)) }, m{ 100 };
	vector<int> A(n);
	for (auto& i : A) i = rand() % m;

	{
		Timer tm1;
		bitonic(A, 0, n - 1, true);
	}
	//for (auto& i : A) cout << i << " ";

	for (auto& i : A) i = rand() % m;
	{
		Timer tm1;
		thread T1{ bitonic, ref(A), 0, n / 4 - 1,  true };
		thread T2{ bitonic, ref(A), n / 4,n / 2 - 1,  false };
		thread T3{ bitonic, ref(A),n / 2, n / 4 * 3 - 1,  true };
		bitonic(A, n / 4 * 3, n - 1, false);
		T1.join();
		T2.join();
		T3.join();
		//cout << "111111111111111111111" << endl;
		T1 = thread{ preCombine, ref(A), 0, n / 8 - 1, true, n / 4 };
		T2 = thread{ preCombine, ref(A), n / 8, n / 4 - 1, true, n / 4 };

		T3 = thread{ preCombine, ref(A),n / 2, n / 2 + n / 8 - 1, false, n / 4 };
		preCombine(A, n / 2 + n / 8, n / 4 * 3 - 1, false, n / 4);
		T1.join();
		T2.join();
		T3.join();
		//cout << "2222222222222222222" << endl;
		T1 = thread(combine, ref(A), 0, n / 4 - 1, true);
		T2 = thread(combine, ref(A), n / 4, n / 2 - 1, true);
		T3 = thread(combine, ref(A), n / 2, n / 4 * 3 - 1, false);
		combine(A, n / 4 * 3, n - 1, false);
		T1.join();
		T2.join();
		T3.join();
		//cout << "333333333333333" << endl;
		T1 = thread(preCombine, ref(A), 0, n / 8 - 1, true, n / 2);
		T2 = thread(preCombine, ref(A), n / 8, n / 4 - 1, true, n / 2);
		T3 = thread(preCombine, ref(A), n / 4, n / 4 + n / 8 - 1, true, n / 2);
		preCombine(A, n / 8 * 3, n / 2 - 1, true, n / 2);
		T1.join();
		T2.join();
		T3.join();
		//cout << "44444444444444" << endl;

		T1 = thread{ preCombine, ref(A), 0, n / 8 - 1, true, n / 4 };
		T2 = thread{ preCombine, ref(A), n / 8, n / 4 - 1, true, n / 4 };
		T3 = thread{ preCombine, ref(A),n / 2, n / 2 + n / 8 - 1, true, n / 4 };
		preCombine(A, n / 2 + n / 8, n / 4 * 3 - 1, true, n / 4);
		T1.join();
		T2.join();
		T3.join();
		//cout << "55555555555555555555" << endl;
		T1 = thread(combine, ref(A), 0, n / 4 - 1, true);
		T2 = thread(combine, ref(A), n / 4, n / 2 - 1, true);
		T3 = thread(combine, ref(A), n / 2, n / 4 * 3 - 1, true);
		combine(A, n / 4 * 3, n - 1, true);
		T1.join();
		T2.join();
		T3.join();
		//cout << "666666666666666666" << endl;

	}



	////T1 = thread(Combine)
	////ofstream out("Output1.txt");
	//for (auto& i : A) { cout << i << " "; }


	////out.close();

	return 0;
}

void preCombine(vector<int>& A, int n1, int n2, bool Inc, int Size) {
	for (int i = n1; i <= n2; ++i) {
		if (Inc) { if (A[i] > A[i + Size]) swap(A[i], A[i + Size]); }
		else { if (A[i] < A[i + Size]) swap(A[i], A[i + Size]); }
	}
}

void combine(vector<int>& A, int n1, int n2, bool Inc) {

	if (n2 - n1 == 1) {
		if (Inc) {
			if (A[n1] > A[n2]) swap(A[n1], A[n2]);
		}
		else {
			if (A[n1] < A[n2]) swap(A[n1], A[n2]);

		}
		return;
	}
	if (Inc) {

		for (int i = n1; i <= (n1 + n2) / 2; ++i) {
			if (A[i] > A[i + (n2 - n1 + 1) / 2]) swap(A[i], A[i + (n2 - n1 + 1) / 2]);
		}
		combine(A, n1, (n2 + n1) / 2, Inc);
		combine(A, (n2 + n1) / 2 + 1, n2, Inc);
		return;
	}
	else {


		for (int i = n1; i <= (n1 + n2) / 2; ++i) {
			if (A[i] < A[i + (n2 - n1 + 1) / 2]) swap(A[i], A[i + (n2 - n1 + 1) / 2]);

		}
		combine(A, n1, (n2 + n1) / 2, Inc);
		combine(A, (n2 + n1) / 2 + 1, n2, Inc);
	}
}




void bitonic(vector<int>& A, int n1, int n2, bool Inc) {

	if (n2 - n1 == 1) {
		if (Inc) {
			if (A[n1] > A[n2]) swap(A[n1], A[n2]);
		}
		else {
			if (A[n1] < A[n2]) swap(A[n1], A[n2]);

		}
		return;
	}
	bitonic(A, n1, (n1 + n2) / 2, true);
	bitonic(A, (n1 + n2) / 2 + 1, n2, false);
	combine(A, n1, n2, Inc);
}