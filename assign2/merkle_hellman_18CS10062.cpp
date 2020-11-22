/*
NAME : RADHIKA PATWARI
ROLL NO. : 18CS10062
COURSE : CRYPTOGRAPHY AND NETWORK SECURITY
ASSIGNMENT NO. : Assignment 2 -> Program for MERKLE HELLMAN ENCRYPTION 
*/

#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

// Function for extended Euclidean Algorithm  
long long int ExtendedEuclidean(long long int a, long long int b, long long int *x, long long int *y)  
{  
    // Base Case  
    if (a == 0)  
    {  
        *x = 0;  
        *y = 1;  
        return b;  
    }  
  	
  	// To store results of recursive call  
    long long int x1, y1; 
    long long int gcd = ExtendedEuclidean(b%a, a, &x1, &y1);  
  
    // Update x and y using results of recursive call  
    *x = y1 - (b/a) * x1;  
    *y = x1;  
  
    return gcd;  
}  

//function for verifying if m is greater than sum of all value of
//super increasing sequence
bool verify_m(vector<long long int> a, long long int m){

	long long int n = a.size();
	long long int i,sum = 0;

	for(i=0;i<n;i++){
		sum += a[i];
	}

	if(m <= sum)
		return false;
	return true;
}

//function to check if gcd(w,m) == 1 and w inverse exits
//funtion stores the inverse value of w globally in wInverse
bool verify_w(long long int m, long long int w, long long int *wInverse){

	long long int x, y, gcd;

	gcd = ExtendedEuclidean(w, m, &x, &y);

	if(gcd != 1)
		return false;

	*wInverse = x;
	return true;
}

//Computing public key of Merkle Hellman cryptosystem
vector<long long int> computePublicWeights(vector<long long int> a, long long int w, long long int m){

	long long int n = a.size();
	long long int i;

	vector<long long int> PublicWeights(n);

	for(i=0;i<n;i++){
		PublicWeights[i] = (1ll*w*a[i])%m;
	}

	return PublicWeights;
}

//using merkle hellman encryption to compute ciphertext
long long int MerkleHellmanEncrypt(vector<long long int> PublicWeights, long long xPlaintext){

	long long int i, rem, n = PublicWeights.size();
	long long int Ciphertext = 0;

	vector<int> x(n);

	i = n-1;
	while(xPlaintext > 0){      //extract bits of plaintext
		rem = xPlaintext%10;
		x[i--] = rem; 
		xPlaintext /= 10;
	}

	for(i=0;i<n;i++){           //compute ciphertext as (plaintext*publicKey)
		Ciphertext += (1ll*PublicWeights[i]*x[i]);
	}

	return Ciphertext;
}

//using Merkle hellman decryption to decryption to obtain plaintext
long long int MerkleHellmanDecrypt(long long int Ciphertext, long long int wInverse, long long int &m, vector<long long int> &a){
	
	long long int n = a.size();
	long long int xPlaintext = 0, i, knapsackSum;
	vector<long long int> x(n,0);

	wInverse = (wInverse + m)%m;
	Ciphertext = (Ciphertext + m)%m;
	knapsackSum = (1ll*wInverse*Ciphertext)%m;  //finding knapsack sum

	i = n-1;
	while(knapsackSum){							//using knapsack problem
		if(a[i] <= knapsackSum){
			knapsackSum -= a[i];
			x[i] = 1;
		}
		i--;
	}

	for(i=0;i<n;i++){
		xPlaintext = xPlaintext*10 + x[i];
	}

	return xPlaintext;
}

int main(){

	vector<long long int> a = {1, 2, 6, 10, 25, 55, 105, 205, 505};
	long long int m, w, wInverse, Ciphertext, xPlaintext;

	m = 999;
	w = 334;

	//verifying m and w before encrypting
	if(!verify_m(a, m) || !verify_w(m, w, &wInverse)){
		cout << "Merkle Hellman Encryption not possible" << endl;
		return 0;
	}
	
	//computing public key
	vector<long long int> PublicWeights = computePublicWeights(a,w,m);

	ofstream fout;
	fout.open("output_18CS10062.txt");

	xPlaintext = 111000111;
	Ciphertext = MerkleHellmanEncrypt(PublicWeights, xPlaintext);
	fout<<"Plain text : "<<xPlaintext<<"   Cipher text : "<<Ciphertext<<endl;
	//cout << MerkleHellmanDecrypt(Ciphertext, wInverse, m, a) << endl;

	xPlaintext = 101010101;
	Ciphertext = MerkleHellmanEncrypt(PublicWeights, xPlaintext);
	fout<<"Plain text : "<<xPlaintext<<"   Cipher text : "<<Ciphertext<<endl;
	//cout << MerkleHellmanDecrypt(Ciphertext, wInverse, m, a) << endl;

	xPlaintext = 110011001;
	Ciphertext = MerkleHellmanEncrypt(PublicWeights, xPlaintext);
	fout<<"Plain text : "<<xPlaintext<<"   Cipher text : "<<Ciphertext<<endl;
	//cout << MerkleHellmanDecrypt(Ciphertext, wInverse, m, a) << endl;

	fout.close();

	return 0;
}