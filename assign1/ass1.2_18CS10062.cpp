
/*NAME : RADHIKA PATWARI
ROLL NO. : 18CS10062
ASSIGNMENT NO. : Assignment 1 -> Program 2 -> program for decryption of playfair cipher*/

#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

//To check if the given character of ciphertext is a lowercase alphabet
bool isalpha(char ch){
	if(ch>='a' && ch<='z')
		return true;
	return false;
}

//changing characters of string to lower case
void changetoLowerCase(string &str){
	int n=str.size();

	for(int i=0;i<n;i++)
		str[i] = str[i]|(32);
}

//Calculating the cipher text from plain text using playfair cipher
//encrypted 5*5 matrix of keyword
string PlayFairDeCrypt(string ciphertxt,string keystr){

	//Maintaing a vector of size 26 where alpha[i] is such that i referes to some alphabet
	//encoding of alphabet and integer is done such that 
	// a->0
	// b->1
	//.....
	// z->25

	//alpha[i] = {r,c} such that character encoding for i is present at rth row and
	//cth column of the encrypted matrix

	//this ensures checking the row and column of a alphabet in O(1) time
	vector<pair<int,int>> alpha(26);

	//Encrypted matrix of (5*5) size containing 25 alphabets
	//Assuming 'i' and 'j' are treated as same alphabets
	vector<vector<char>> matrix(5,vector<char>(5));

	//final plain text obtained after decryption using playfair cipher 
	string plaintxt = "";

	//i,j : loop variables
	//len : store the length of encrypted keyword
	//txt_len : store the length of cipher text
	//ch : store the encoding of characters of encrypted key
	//r,c : empty cell of matrix going rowise
	//ch1,ch2 : the first 2 characters of the diagram(pair of 2) of cipher text
	int i,j,check_i,len,txt_len,ch,r = 0,c = 0;
	char ch1,ch2;

	//initialising the row and column for each 26 letter
	for(i=0;i<26;i++){
		alpha[i] = {-1,-1};
	}

	//storing the encrypted keyword length
	len = keystr.size();

	//filling the matrix using the non-repeated alphabets of encrypted keyword
	for(i=0;i<len;i++){

		//extracting encoding of alphabets of keyword
		ch = keystr[i]-'a';

		//Case 1 : 
		//if the alphabet is 'i' and 'j' is already present in excrypted matrix
		//then there is no need to insert 'i' in it
		//Case 2 :
		//if the alphabet is 'i' and 'j' is not present in encrypted matrix
		//then we insert 'j' in it (we give more preference to 'j')
		if(keystr[i] == 'i'){
			if(alpha['j'-'a'] == make_pair(-1,-1)){	
				alpha[ch] = {r,c};
				matrix[r][c] = keystr[i];
				c = (c+1)%5;
				if(c==0)       //changing row for every new 0th column
					r++;
			}
		}	
		else if(alpha[keystr[i]-'a'] == make_pair(-1,-1)){
			alpha[ch] = {r,c};
			matrix[r][c] = keystr[i];
			c = (c+1)%5;
			if(c==0)          //changing row for every new 0th column
				r++;
		}	
	}

	//filling remaining lower case alphabets in the encrypted matrix
	//If the alphabet is already present,then do not insert it again
	for(i=0;i<26;i++){
		check_i = (i==8)?1:0;
		if(alpha[i] == make_pair(-1,-1)){
			alpha[i+check_i] = {r,c};
			matrix[r][c] = (char)(i+check_i+97);
			c = (c+1)%5;
			if(c==0)        //changing row for every new 0th column
				r++;
		}
	}

	//printing the Encrypted matrix of the playfair cipher
	/*for(i=0;i<5;i++){
		for(j=0;j<5;j++)
			cout<<matrix[i][j]<<" ";
		cout<<endl;
	}*/

	//changing all alphabetical characters of cipher text to lower case
	changetoLowerCase(ciphertxt);

	//storing the plain text length
	txt_len = ciphertxt.size();

	//extracting 2 alphabetic characters from cipher text and applying playfair cipher
	//to generate the decrypted cipher code for the cipher text and concatenate the 
	//characters in plaintxt
	for(i=0;i<txt_len;){

		//Cipher text do not contain special characters 
		//hence we can directly take the values for the diagram formation
		//storing 1st alphabetic character of diagram in ch1 
		ch1 = ciphertxt[i];

		//extracting 2nd alphabetic character of the diagram from the ciphertext
		i++; 

		//Cipher text has been formed using diagrams,hence it possess the following properties : 
		//contain even number of characters
		//no same adjacent characters as no 2 cell elements in the encryption matrix can have same characters
		//padding is not required
		ch2 = ciphertxt[i];
		i++;

		//Since we give preference to 'j' over 'i' in the encrypted matrix
		//we change it to 'j' in case any character in diagram is 'i'
		if(ch1 == 'i')
			ch1 = 'j';
		if(ch2 == 'i')
			ch2 = 'j';

		//Case 1:
		//if both the characters of diagram belong to same row
		//use the following character on left circularly
		if(alpha[ch1-'a'].first == alpha[ch2-'a'].first){
			plaintxt = plaintxt + matrix[alpha[ch1-'a'].first][(alpha[ch1-'a'].second-1+5)%5] + matrix[alpha[ch1-'a'].first][(alpha[ch2-'a'].second-1+5)%5];
		}
		//Case 2:
		//if both the characters of diagram belong to same column
		//use the following character above circularly
		else if(alpha[ch1-'a'].second == alpha[ch2-'a'].second){
			plaintxt = plaintxt + matrix[(alpha[ch1-'a'].first-1+5)%5][alpha[ch1-'a'].second] + matrix[(alpha[ch2-'a'].first-1+5)%5][alpha[ch2-'a'].second];
		}
		//Case 3:
		//if the characters do not have common row or column,then
		//use the letter that lies in its own row and the column occupied by the other
		//character of the diagram
		else{
			plaintxt = plaintxt + matrix[alpha[ch1-'a'].first][alpha[ch2-'a'].second] + matrix[alpha[ch2-'a'].first][alpha[ch1-'a'].second];
		}
	}

	alpha.clear();
	matrix.clear();

	return plaintxt;
}
int main(){

	//plaintxt = Plain text in lower case obtained from cipher text after applying playfair cipher
	//ciphertxt = Cipher text in upper case
	string plaintxt,ciphertxt;

	//keystr = encryption keyword for obtaining playfair encrypted matrix
	//and use it for decryption of cipher text
	string keystr;

	//USING INPUT/OUTPUT FILE SYSTEM ------------------------------

	//creation of ifstream class object
	ifstream fin;
	fin.open("ass1.2_input.txt");
	// FILE FORMAT
	//first line contain keystr
	//second line contain number of cipher texts -> count
	//Then a series of count number of cipher texts follow

	//creation of ofstream class object
	ofstream fout;
	fout.open("ass1.2_output.txt");

	//Reading keyword from the input file
	getline(fin,keystr);

	//Reading # of cipher texts from the input file as string in ctr
	//and the convert it into integer using stoi function and store it in count
	string ctr;
	getline(fin,ctr);
	int count = stoi(ctr);

	//reading count # of cipher texts one by one
	while(count--){

		//reading a ciphertext from input file
		getline(fin,ciphertxt);

		//decrypting cipher text into plain text 
		plaintxt = PlayFairDeCrypt(ciphertxt,keystr);

		//writing the plain text in the output file
		fout<<"Cipher text : "<<ciphertxt<<endl;
		fout<<"Plain text : "<<plaintxt<<endl;
		fout<<endl;

		//printing plain text in the console as well
		cout<<plaintxt<<endl;
	}

	fin.close();
	fout.close();

	//USING CONSOLE INPUT/OUTPUT -------------------------------------

	// keystr and cipher text can be taken as input from the console

	// uncomment the below statement
	// cin>>keystr;

	// User defined test-case:

	// uncomment the 3 statements below
	/*cin>>ciphertxt;
	plaintxt = PlayFairDeCrypt(ciphertxt,keystr);
	cout<<plaintxt<<endl;*/

}