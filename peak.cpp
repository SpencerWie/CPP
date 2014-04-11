/*
Notes:

This program is used to find and O(log n) algorithm for the peak finding problem. Such that we go from always 
increasing, to a peak, then always decreasing. And excepts an input file where it's a single number per line
called "peak.txt".

---peak.txt---
1
5
10
8
7
5
3
2
--------------

Would output:

---peak.cpp---
>> getPeak(0,7) - at mid 3
>> getPeak(0,2) - at mid 1
>> getPeak(2,2) - at mid 2
The peak is at index 2 with value 10
--------------
*/

#include <iostream>
#include <fstream>
#include <math.h> 
#include <vector>
#include <string>

using namespace std;

int findPeak(int start, int end);
void readFile();
vector<int> A; // Dynamic vector to use as array.

int main() 
{
	readFile();
	int last = A.size()-1; // last element in array
	int peak = findPeak(0,last);
	if(peak != -1) cout << "The peak is at index " << peak << " with value " << A[peak] << endl;
	else cout << "No peek value (array might be invalid)" << endl;
	return 0;
}

int findPeak(int start, int end)
//	A Divide-and-Concor algothim to find a peek of a single mountain of int item arrays. We pick an element, starting with the middle of the array.
//	Then check it's neighboor values, if "i" is higher then both neighboors then we are at the peak (base), if i-1 is higher then do getPeak(0 to i-1) since
//	the peek must be in that sub-array, otherwise if i+1 is higher getPeek(i+1 to n-1) and it must be in that subarray. Doing this we have O(log n).
{
	int mid = floor((start+end)/2);
	cout << ">> getPeak("<< start << ", " << end << ") - at mid " << mid << endl; // Tell the user what functions and parms are being called

	// If we are at the peek return it's index.
	if(A[mid] > A[mid+1] && A[mid] > A[mid-1]) return mid;

	// If the left value is larger, consider to find the peek of the left array.
	else if(A[mid] < A[mid-1]) findPeak(start, mid-1);

	// If the right value is larger, consider to find the peek of the right array.
	else if(A[mid] < A[mid+1]) findPeak(mid+1, end);

	else return -1; // If the array is not correct

}

// Get the input file, per line and place into array A.
void readFile()
{
	string line;
	ifstream file("peak.txt");
	if(file.is_open())
	{
		int i, number = 0;
		while(file >> number)
		{
			A.push_back(number);
		}
		file.close();
	} else {
		cout << "Can't open file (make sure file is named 'peak.txt' )..." << endl;
	}
}
