#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string xorProcess(string data) {
    char key = '*'; 
    string output = data;
    for (int i = 0; i < data.length(); i++) {
        output[i] = data[i] ^ key;
    }
    return output;
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    string command = argv[1];

    // CHECK IF SYSTEM IS INITIALIZED
    if (command == "check_init") {
        ifstream file("vault.txt");
        if (!file || file.peek() == EOF) cout << "NEED_SETUP";
        else cout << "READY";
        return 0;
    }

    // SETUP MASTER KEY
    if (command == "setup") {
        ofstream file("vault.txt");
        file << "MASTER_KEY:" << xorProcess(argv[2]) << endl;
        cout << "SETUP_SUCCESS";
        return 0;
    }

    // LOGIN
    if (command == "auth") {
        ifstream file("vault.txt");
        string line;
        getline(file, line); // Get first line
        string stored = line.substr(line.find(":") + 1);
        if (xorProcess(stored) == argv[2]) cout << "SUCCESS";
        else cout << "FAIL";
    }

    // ADD PASSWORD
    else if (command == "add") {
        ofstream file("vault.txt", ios::app);
        file << argv[2] << ":" << xorProcess(argv[3]) << endl;
        cout << "ADDED";
    }

    // VIEW ALL
    else if (command == "view") {
        ifstream file("vault.txt");
        string line;
        getline(file, line); // Skip master
        while (getline(file, line)) {
            if (!line.empty()) {
                string site = line.substr(0, line.find(":"));
                string enc = line.substr(line.find(":") + 1);
                cout << site << " -> " << xorProcess(enc) << "|";
            }
        }
    }

    // CHANGE MASTER
    else if (command == "change") {
        vector<string> data;
        string line;
        ifstream fileIn("vault.txt");
        getline(fileIn, line); // drop old master
        while (getline(fileIn, line)) data.push_back(line);
        fileIn.close();

        ofstream fileOut("vault.txt");
        fileOut << "MASTER_KEY:" << xorProcess(argv[2]) << endl;
        for (string s : data) fileOut << s << endl;
        cout << "CHANGED";
    }

    // RESET
    else if (command == "reset") {
        ofstream file("vault.txt", ios::trunc);
        cout << "CLEARED";
    }

    return 0;
}