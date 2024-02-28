//
//  main.cpp
//  Mini Project: Compressed Sentences
//
//  Created by Cristian Ramirez on 3/7/22.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <map>
using namespace std;

unordered_map<string, int> MAP;
unordered_set<string> DICT2;
map<int,string> RANKMAP;


// Define the character size
#define CHAR_SIZE 128
 
// A class to store a Trie node
class Trie
{
public:
    bool isLeaf;
    Trie* character[CHAR_SIZE];
 
    // Constructor
    Trie()
    {
        this->isLeaf = false;
 
        for (int i = 0; i < CHAR_SIZE; i++) {
            this->character[i] = nullptr;
        }
    }
 
    void insert(string);
    bool search(string);
    bool haveChildren(Trie const*);
};
 
// Iterative function to insert a key into a Trie
void Trie::insert(string key)
{
    // start from the root node
    Trie* curr = this;
    for (int i = 0; i < key.length(); i++)
    {
        // create a new node if the path doesn't exist
        if (curr->character[key[i]] == nullptr) {
            curr->character[key[i]] = new Trie();
        }
 
        // go to the next node
        curr = curr->character[key[i]];
    }
 
    // mark the current node as a leaf
    curr->isLeaf = true;
}
 
// Iterative function to search a key in a Trie. It returns true
// if the key is found in the Trie; otherwise, it returns false
bool Trie::search(string key)
{
    // return false if Trie is empty
    if (this == nullptr) {
        return false;
    }
 
    Trie* curr = this;
    for (int i = 0; i < key.length(); i++)
    {
        // go to the next node
        curr = curr->character[key[i]];
 
        // if the string is invalid (reached end of a path in the Trie)
        if (curr == nullptr) {
            return false;
        }
    }
 
    // return true if the current node is a leaf and the
    // end of the string is reached
    return curr->isLeaf;
}
 
// Returns true if a given node has any children
bool Trie::haveChildren(Trie const* curr)
{
    for (int i = 0; i < CHAR_SIZE; i++)
    {
        if (curr->character[i]) {
            return true;    // child found
        }
    }
 
    return false;
}

void readFile(string fileName){

    string line;
    fstream myFile;
    int n = 1;
    int mapCounter=0;
    
    myFile.open(fileName, ios::in); //reading from file

    if(myFile.is_open()){
        while (getline(myFile,line)) {
            
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            
            if (line.size() > 1 || line == "I" || line == "a" || line == "i" || line == "A") {
                if (line[0] != '#') {
                    if(line[0] != '!'){
                        if(line[0] != '.'){
                            if(line.substr(0,1) != "xx"){
                                if(line.substr(0,1) != "XX"){
                                    mapCounter++;
                                    if (fileName == "dictionary.txt") {
                                        MAP[line] = n;
                                        if(mapCounter == 100){
                                            n++;
                                            mapCounter=0;
                                        }
                                    }
                                    
                                    else
                                        DICT2.insert(line);
                                }
                            }
                        }
                    }
                }
            }
        }
            
        myFile.close();
    }
    
    return;
}

string readSentence(string fileName){
    
    string line , mySentence;
    fstream myFile;
    
    myFile.open(fileName,ios::in);
    
    if (myFile.is_open()) {
        while (getline(myFile,line)) {
            mySentence+=line;
        }
        myFile.close();
    }
    
    return mySentence;
}

void saveToFile (string sentence){
    
    fstream myFile;
    myFile.open("sentences.txt",ios::out);
    
    
    if (myFile.is_open()) {
        myFile << "Original Sentence: " << sentence << endl << endl;
        myFile << "Here Are The Best Possible Solutions From Best to Worst:" << endl << endl;
        int n = 1;
        for(auto i = RANKMAP.begin(); i != RANKMAP.end() ; i++){
            myFile << n << ": " << i->second << "\n";
            n++;
        }
        
        myFile.close();
    }
    
}


void closeGaps(string &result){
    
    int periodIndex;
    
    for (int i=0; i<result.length(); i++) {
        if (result[i]=='.'|| result[i]=='!' || result[i] == ',' || result[i] == '?'||result[i]=='\''||result[i]=='\"'||result[i] == ',') {
            periodIndex=i;
            result.erase(periodIndex-1,1);
        }
    }

    return;
}

int getNumber(vector<string> &myVect){
    
    int total = 0;
    
    for (auto i: myVect){
        total += MAP[i];
    }
    
    return total;
}


void getRanking(string result){
    
    vector<string> rankingVec;
    string temp = "";
    int size = result.size();
   // map<string,int> rankingMap;
    
    for (int i=0; i<size; i++) {
        temp+=result[i];
        if(result[i] == ' ' ){
            temp.erase(temp.size()-1 ,1);
            rankingVec.push_back(temp);
            temp = "";
        }
        
    }
    
    closeGaps(result);
    
    RANKMAP[getNumber(rankingVec)] = result;
    
}

 
//Prototype if compressedSentences
void compressedSentence(string str, long size , string result , Trie* head);

//Taken from main()
void compressedSentence(string str, Trie* head){
    
    compressedSentence(str, str.size(), "", head); // Passed into 3 parameter function
}

//Recursion function
//Scans string and places spaces in appropriate location based on our Trie Tree
//Outputs all possible sentenc
void compressedSentence (string str, long n, string result, Trie* head){
    
    
    for( int i = 1; i<=n ; i++){
        string prefix = str.substr(0,i);
        
        if ( head->search(prefix)){
            if (i==n){
                result += prefix;
                getRanking(result);
                //cout << result <<endl;
                return;
            }
            compressedSentence(str.substr(i,n-i), n-i, result + prefix + " ", head); //Recursion call
        }
    }
}


int main() {
    // insert code here...
    Trie* head = new Trie();
    readFile("dictionary.txt");
    readFile("dictionary2.txt");
    

    for(auto x: DICT2){
        head->insert(x);
    }
    
    head->insert(".");
    head->insert("!");
    head->insert("?");
    head->insert("\'");
    head->insert("\"");
    head->insert(",");
    head->insert(":");
    head->insert(";");
    
    MAP["."] = 0;
    MAP["!"] = 0;
    MAP["?"] = 0;
    MAP["\'"] = 0;
    MAP["\""] = 0;
    MAP[","] = 0;
    MAP[":"] = 0;
    MAP[";"] = 0;
    
    
    string mySentence = readSentence("stringfile.txt");
    compressedSentence(mySentence, head);
    saveToFile(mySentence);
    
    cout << "Original Sentence: " << mySentence << endl << endl;
    cout << "Here Are The Best Possible Solutions From Best to Worst:" << endl << endl;
    
    int n = 1;
    for(auto i = RANKMAP.begin(); i != RANKMAP.end() ; i++){
        cout << n << ": " << i->second << "\n";
        n++;
    }
    

    return 0;
}
