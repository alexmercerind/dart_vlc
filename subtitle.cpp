#include "stdafx.h"
#include "iostream"
#include "cstdlib"
#include "fstream"
#include "string"

 using namespace std;

 int main(int argc, char *argv[]){
 ifstream input; //input

 char input_file[32]; //names of input and output


  cout << "Enter name of input fille: "; ///user gives names of input
  cin >> input_file;

 input.open(input_file);
 if (!input.good()){
cout << "File " << input_file << " dosen't exist." << endl;
return 1;
}


string row;
while (!input.eof()){    
getline(input, row);

cout << row << endl;
}

system("pause");
return 0;
}