#include <iostream>
#include <fstream>
#include <math.h> 
#include <vector>
#include <string>

using namespace std;

void readFile(); 
void findSeq();
vector<int> X; // Dynamic vector to use as array. Starting seq

int main() 
{
	readFile();
	findSeq();
}



// Get the input file, per line and place into array A. Place two very small numbers on the edge to prevent out-of-bounds errors.
void readFile()
{
	string line;
	ifstream file("incseq.txt");
	if(file.is_open())
	{
		int number = 0;
		while(file >> number)
		{
			X.push_back(number);
		}
		file.close();
	} else {
		cout << "Can't open file (make sure file is named 'peak.txt' )..." << endl;
	}
}

void findSeq()
// Use algorithm desciribed to find the largest increasing sequence.
{
	vector<int> L; // Seq tracker
	vector<int> S; // Our actual seq
	cout << "X = ";

	// copy size and show org seq
	for(int i = 0; i < X.size(); i++){
		L.push_back(1); 
		cout << X[i] << " ";
	}

	cout << endl;
	// maxL = max length, maxI = last index of seq
	int maxL = 1; int maxI = 0; L[0] = 1; 

	for(int i = 1; i < L.size(); i++)
	{
		for(int j = i - 1; j >= 0; j--)
		{
			if(X[j] < X[i] && L[j] + 1 > L[i]){
				L[i] = L[j] + 1; 
			}
		}
		if(L[i] > maxL){
			maxL = L[i]; 
			maxI = i;
		}
	}

	cout << "The largest increasing sequence has a length of " << maxL << "." << endl;
	cout << "Table : | ";
	for(int i = 0; i < L.size(); i++) cout << L[i] << " | ";
	cout << endl;

	// To get the sequence we move back and get the values where the L value goes down by one from the highest.
	S.push_back(X[maxI]);
	for(int i = maxI; i >= 0 ; i--){
		if(L[i] == L[maxI] - 1){
			S.push_back(X[i]); maxI = i; 
		}
	}
	cout << "Seq : ";
	for(int i = 0; i < S.size() ; i++) cout << S[i] << " ";
	cout << endl;

}