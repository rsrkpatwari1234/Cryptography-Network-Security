
/*NAME : RADHIKA PATWARI
ROLL NO. : 18CS10062
ASSIGNMENT NO. : Assignment 1 -> Program 1 -> program for playfair cipher*/

#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

//To check if the given character of plaintext is a lowercase alphabet
bool isalpha(char ch){
	if(ch>='a' && ch<='z')
		return true;
	return false;
}

//changing characters of string to upper case
void changetoUpperCase(string &str){
	int n=str.size();

	for(int i=0;i<n;i++)
		str[i] = str[i]&(~32);
}

//Calculating the cipher text from plain text using playfair cipher
//encrypted 5*5 matrix of keyword
string PlayFairCrypt(string plaintxt,string keystr){

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

	//final cipher text obtained after encryption using playfair cipher 
	string ciphertxt = "";

	//i,j : loop variables
	//len : store the length of encrypted keyword
	//txt_len : store the length of plain text
	//ch : store the encoding of characters of encrypted key
	//r,c : empty cell of matrix going rowise
	//ch1,ch2 : the first 2 characters of the diagram(pair of 2) of plain text
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

	//storing the plain text length
	txt_len = plaintxt.size();

	//extracting 2 alphabetic characters from plain text and applying playfair cipher
	//to generate the encrypted cipher code for the plain text and concatenate the 
	//characters in ciphertxt
	for(i=0;i<txt_len;){

		//extracting 1st alphabetic character for the diagram
		while(i<txt_len && !isalpha(plaintxt[i]))
			i++;

		//Break the loop if no more alphabetic characters exist
		if(i == txt_len) break;

		//storing 1st alphabetic character of diagram in ch1 
		ch1 = plaintxt[i];

		//extracting 2nd alphabetic character of the diagram from the plaintext
		i++; 
		while(i<txt_len && !isalpha(plaintxt[i]))
			i++;

		//Case 1:
		//if no more alphabetic characters exist in plain text,
		//perform padding of 'x' in the diagram
		//Case 2:
		//if the 2nd character of plain text for the diagram is same as 1st 
		//character , then add a 'x' as 2nd character of diagram 
		//Case 3:
		//if the case if "ij" or "ji" , then also use filler letter 'x' as 'i' and 
		//'j' are considered same
		//Case 4:
		//if this character exists and is not equal to 1st character,then
		//add it to the diagram
		if(i == txt_len || plaintxt[i] == ch1 || (plaintxt[i] == 'i' && ch1 == 'j') || (plaintxt[i] == 'j' && ch1 == 'i')){
			ch2 = 'x';
		}
		else{
			ch2 = plaintxt[i];
			i++;
		}

		//Since we give preference to 'j' over 'i' in the encrypted matrix
		//we change it to 'j' in case any character in diagram is 'i'
		if(ch1 == 'i')
			ch1 = 'j';
		if(ch2 == 'i')
			ch2 = 'j';

		//Case 1:
		//if both the characters of diagram belong to same row
		//use the following character on right circularly
		if(alpha[ch1-'a'].first == alpha[ch2-'a'].first){
			ciphertxt = ciphertxt + matrix[alpha[ch1-'a'].first][(alpha[ch1-'a'].second+1)%5] + matrix[alpha[ch1-'a'].first][(alpha[ch2-'a'].second+1)%5];
		}
		//Case 2:
		//if both the characters of diagram belong to same column
		//use the following character beneath circularly
		else if(alpha[ch1-'a'].second == alpha[ch2-'a'].second){
			ciphertxt = ciphertxt + matrix[(alpha[ch1-'a'].first+1)%5][alpha[ch1-'a'].second] + matrix[(alpha[ch2-'a'].first+1)%5][alpha[ch2-'a'].second];
		}
		//Case 3:
		//if the characters do not have common row or column,then
		//use the letter that lies in its own row and the column occupied by the other
		//character of the diagram
		else{
			ciphertxt = ciphertxt + matrix[alpha[ch1-'a'].first][alpha[ch2-'a'].second] + matrix[alpha[ch2-'a'].first][alpha[ch1-'a'].second];
		}
	}

	changetoUpperCase(ciphertxt);

	alpha.clear();
	matrix.clear();

	return ciphertxt;
}
int main(){

	//plaintxt = Plain text in lower case
	//ciphertxt = Cipher text obtained from plain text after applying playfair cipher
	string plaintxt,ciphertxt;

	//keystr = encryption keyword for obtaining playfair cipher text
	string keystr;

	//USING INPUT/OUTPUT FILE SYSTEM ------------------------------

	//creation of ifstream class object
	ifstream fin;
	fin.open("ass1.1_input.txt");
	// FILE FORMAT
	//first line contain keystr
	//second line contain number of plain texts -> count
	//Then a series of count number of plain texts follow

	//creation of ofstream class object
	ofstream fout;
	fout.open("ass1.1_output.txt");

	//Reading keyword from the input file
	getline(fin,keystr);

	//Reading # of plain texts from the input file as string in ctr
	//and the convert it into integer using stoi function and store it in count
	string ctr;
	getline(fin,ctr);
	int count = stoi(ctr);

	//reading count # of plain texts one by one
	while(count--){

		//reading a plalintext from input file
		getline(fin,plaintxt);

		//encrypting plain text into cipher text 
		ciphertxt = PlayFairCrypt(plaintxt,keystr);

		//writing the cipher text in the output file
		fout<<"Plain text : "<<plaintxt<<endl;
		fout<<"Cipher text : "<<ciphertxt<<endl;
		fout<<endl;

		//printing cipher text in the console as well
		cout<<ciphertxt<<endl;
	}

	fin.close();
	fout.close();

	//USING CONSOLE INPUT/OUTPUT -------------------------------------

	// keystr and plain text can be taken as input from the console

	// uncomment the below statement
	// cin>>keystr;

	// User defined test-case:

	// uncomment the 3 statements below
	/*cin>>plaintxt;
	ciphertxt = PlayFairCrypt(plaintxt,keystr);
	cout<<ciphertxt<<endl;*/

}