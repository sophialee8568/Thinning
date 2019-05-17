
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void initializeArray(int** ary, int numRows, int numCols){
	for (int i = 0; i < numRows+2; ++i){
		ary[i] = new int[numCols+2];
	}
}

void zeroFramed(int** ary, int numRows, int numCols){
	for ( int i = 0; i < numRows+2; i++){
		for (int j = 0; j < numCols+2; j++){
			ary[i][j] = 0;
		}
	}
}

void changeFlags(bool& flag){
	flag = true;
}

void doThinning(int** firstAry, int** secondAry, int r, int c, bool& flag){
	int nonZeroCount = -1;
	bool moreThanTwoCC = false;

	for ( int i = -1; i<= 1; i++){
		for (int j = -1; j <= 1; j++){
			if(firstAry[r+i][c+j] != 0) nonZeroCount++;
		}
	}



	int a = firstAry[r-1][c-1];
	int b = firstAry[r-1][c];
	int cc = firstAry[r-1][c+1];
	int d = firstAry[r][c-1];
	int f = firstAry[r][c+1];
	int g = firstAry[r+1][c-1];
	int h = firstAry[r+1][c];
	int i = firstAry[r+1][c+1];


	if ( b == 0 && h == 0) moreThanTwoCC = true;
	if ( d == 0 && f == 0) moreThanTwoCC = true;

	if ( a == 1 && b == 0 && d == 0) moreThanTwoCC = true;
	if ( f == 0 && h == 0 && i == 1) moreThanTwoCC = true;

	if ( a == 0 && i == 0 && ((f != 1 && h != 1) || (b!=1 && d !=1 ))) moreThanTwoCC = true;
	if ( cc == 0 && g == 0 && ((b != 1 && d != 1) || (f != 1 && h != 1))) moreThanTwoCC = true;

	if ( b == 0 && cc == 1 && f == 0) moreThanTwoCC = true;
	if ( d == 0 && g == 1 && h == 0) moreThanTwoCC = true;

	if (nonZeroCount >= 4 && moreThanTwoCC == false){
		secondAry[r][c] = 0;
		changeFlags(flag);
	}
	else { secondAry[r][c] = 1; }

}

void northThinning(int** firstAry, int** secondAry, int i, int j, bool& flag){
	if ((firstAry[i][j] > 0) && (firstAry[i-1][j] == 0)){
		doThinning(firstAry, secondAry, i, j, flag);
	}
}

void southThinning(int** firstAry, int** secondAry, int i, int j, bool& flag){
	if ((firstAry[i][j] > 0) && (firstAry[i+1][j] == 0)){
		doThinning(firstAry, secondAry, i, j,flag);
	}
}

void westThinning(int** firstAry, int** secondAry, int i, int j, bool& flag){
	if ((firstAry[i][j] > 0) && (firstAry[i][j-1] == 0)){
		doThinning(firstAry, secondAry, i, j, flag);
	}
}

void eastThinning(int** firstAry, int** secondAry, int i, int j, bool& flag){
	if ((firstAry[i][j] > 0) && (firstAry[i][j+1] == 0)){
		doThinning(firstAry, secondAry, i, j, flag);
	}
}

void loadImage(int** ary,int** ary2, int numRows, int numCols, ifstream& inFile){
	int next;
	for ( int i = 1; i < numRows+1; i++){
		for (int j = 1; j < numCols+1; j++){
			inFile >> next;
			ary[i][j] = next;
			ary2[i][j] = next;
		}
	}
}

void copyAry(int** firstAry, int** secondAry, int numRows, int numCols){
	for (int i = 1; i < numRows + 1; i++){
		for (int j = 1; j < numCols + 1; j++){
			firstAry[i][j] = secondAry[i][j];
		}
	}
}

void prettyPrint(int** firstAry, ofstream& outFile2, int numRows, int numCols){
	for ( int i = 1; i < numRows+1; i++){
		for (int j = 1; j < numCols+1; j++){
			if (firstAry[i][j] > 0) outFile2 << firstAry[i][j] << " ";
			else { outFile2 << "  ";}
		}
		outFile2 << endl;
	}
}

int main(int argc, char** argv) {
	ifstream inFile;
	ofstream outFile1;
	ofstream outFile2;
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	bool changeFlag = true;
	int cycleCount;
	int** firstAry;
	int** secondAry;
	int next;

	inFile.open(argv[1]);
	inFile >> next;
	numRows = next;
	inFile >> next;
	numCols = next;
	inFile >> next;
	minVal = next;
	inFile >> next;
	maxVal = next;

	firstAry = new int*[numRows+2];
	secondAry = new int*[numRows+2];
	initializeArray(firstAry, numRows, numCols);
	initializeArray(secondAry, numRows, numCols);
	zeroFramed(firstAry, numRows, numCols);
	loadImage(firstAry,secondAry, numRows, numCols, inFile);
	cycleCount = 0;
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);

	while (changeFlag != false){
		changeFlag = false;
			for (int i = 1; i < numRows+1; i++){
				for (int j = 1; j < numCols+1; j++){
					northThinning(firstAry, secondAry, i, j, changeFlag);

				}
			}
			copyAry(firstAry, secondAry, numRows, numCols);
			if (cycleCount == 0 || cycleCount == 2 || cycleCount == 4){

				outFile2 << "result of cycle " << cycleCount << " with north thinning" << endl << endl;
				prettyPrint(firstAry, outFile2, numRows, numCols);
			}

			for (int i = 1; i < numRows+1; i++){
				for (int j = 1; j < numCols+1; j++){
					southThinning(firstAry, secondAry, i, j, changeFlag);

				}
			}
			copyAry(firstAry, secondAry, numRows, numCols);

			if (cycleCount == 0 || cycleCount == 2 || cycleCount == 4){

				outFile2 << "result of cycle " << cycleCount << " with south thinning" << endl << endl;
				prettyPrint(firstAry, outFile2, numRows, numCols);
			}

			for (int i = 1; i < numRows+1; i++){
				for (int j = 1; j < numCols+1; j++){
					westThinning(firstAry, secondAry, i, j,changeFlag);

				}
			}
			copyAry(firstAry, secondAry, numRows, numCols);

			if (cycleCount == 0 || cycleCount == 2 || cycleCount == 4){

				outFile2 << "result of cycle " << cycleCount << " with west thinning" << endl << endl;
				prettyPrint(firstAry, outFile2, numRows, numCols);
			}

			for (int i = 1; i < numRows+1; i++){
				for (int j = 1; j < numCols+1; j++){
					eastThinning(firstAry, secondAry, i, j,changeFlag);
				}
			}
			copyAry(firstAry, secondAry, numRows, numCols);
			if (cycleCount == 0 || cycleCount == 2 || cycleCount == 4){

				outFile2 << "result of cycle " << cycleCount << " with east thinning" << endl << endl;
				prettyPrint(firstAry, outFile2, numRows, numCols);
			}
			cycleCount++;

	}

	outFile1 << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
	outFile2 << endl << "Final Result of Thinning" << endl;
	for ( int i = 1; i < numRows+1; i++){
		for (int j = 1; j < numCols+1; j++){
			outFile1 << firstAry[i][j] << " ";
		}
		outFile1 << endl;
	}
	prettyPrint(firstAry, outFile2, numRows, numCols);

	inFile.close();
	outFile1.close();
	outFile2.close();

	return 0;
}
