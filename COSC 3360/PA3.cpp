#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
#include <deque>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

struct Address{                             // Struct to hold address
    int segment;                            // Declare segment number
    int page;                               // Declare page number
    int displacement;                       // Declare displacement
};

struct Page{                                // Struct to hold page data
    int pageNum;                            // Declare page number
    string value;
    vector<int> times;                      // Declare access times
};

struct Segment{
    int segNum;                             // Declare segment number
    deque<Page> pageTable;                  // Declare page table
};

struct Process{                             // Struct to hold process data
    int pid;                                // Declare process id
    int size;                               // Declare total number of pages on disk
    vector<Segment> segTable;               // Declare segment table
};

// Read file and assign variables
void fileRead(string filename, int &tp, int &sl, int &ps, int &r, int &x, int &minimum, int &maximum, int &k, vector<Process> &processes, vector<string> &requests){
    fstream file(filename);                     // Open file
    string line;                                // Declare line to hold line from file

    getline(file,line);
    tp = stoi(line);

    getline(file,line);
    sl = stoi(line);

    getline(file,line);
    ps = stoi(line);

    getline(file,line);
    r = stoi(line);

    getline(file,line);
    x = stoi(line);

    getline(file,line);
    minimum = stoi(line);

    getline(file,line);
    maximum = stoi(line);

    getline(file,line);
    k = stoi(line);

    for(int i = 0; i < k; i++) {
        getline(file, line);
        stringstream ss(line);
        string token;

        processes.push_back(*(new Process));
        getline(ss, token, ' ');
        processes[i].pid = stoi(token);
        getline(ss, token, ' ');
        processes[i].size = stoi(token);

    }

    while(getline(file,line)){
        requests.push_back(line);
    }

    file.close();
}

// Convert hexidecimal address to decimal and store segment number, page number, and displacement.
Address hexToAddrData(string line, int pageSize, int segLength){
    Address data;
    string hex = line.substr(line.find(' ')+1);
    int deci = strtoul(hex.c_str(),NULL,0);
    data.displacement = deci & ((1 << (int)log2(pageSize)) - 1);
    data.page = (deci >> (int)log2(pageSize)) & ((1 << (int)log2(segLength)) - 1);
    data.segment = deci >> ((int)log2(pageSize) + (int)log2(segLength));
    return data;
}

// Clears all tables in preparation for next page replacement algorithm
void clearTables(vector<Process> &processes, int k){
    for(int i = 0; i < k; i++){
        for(int j = 0; j < processes[i].segTable.size(); j++){
            processes[i].segTable[j].pageTable.clear();
        }
        processes[i].segTable.clear();
    }
}

// Performs FIFO page replacement
void FIFO(int sl, int ps, int r, int k, vector<Process> processes, vector<string> requests){
    int pageFaults[k] = { 0 };
    for(int i = 0; i < requests.size(); i++){                                                   // Iterates through requests
        int currentP;
        int currentS = 0;
        int id = stoi(requests[i].substr(0, requests[i].find(' ')));
        for(int j = 0; j < k; j++){                                                             // Match request to process
            if(id == processes[j].pid){
                currentP = j;
                break;
            }
        }
        Address currAddr = hexToAddrData(requests[i], ps, sl);
        if(processes[currentP].segTable.empty()) {                                              // Match request to segment
            Segment *seg  = new Segment;
            seg->segNum = currAddr.segment;
            processes[currentP].segTable.push_back(*seg);
        }
        else{
            for(int j = 0; j < processes[currentP].segTable.size(); j++){
                if(currAddr.segment == processes[currentP].segTable[j].segNum){
                    currentS = j;
                    break;
                }
                else if((j == processes[currentP].segTable.size() - 1) && (currentS == 0)){
                    Segment *seg  = new Segment;
                    seg->segNum = currAddr.segment;
                    processes[currentP].segTable.push_back(*seg);
                    currentS = j + 1;
                }
            }
        }
        if(processes[currentP].segTable[currentS].pageTable.empty()){                           // Add page to table
            pageFaults[currentP] += 2;                                                          // 2 Page faults: allocate page, add page
            Page *pg = new Page;
            pg->pageNum = currAddr.page;
            pg->value = requests[i].substr(requests[i].find(' '));
            processes[currentP].segTable[currentS].pageTable.push_back(*pg);
        }
        else{
            bool inTable = false;
            for(int j = 0; j< processes[currentP].segTable[currentS].pageTable.size(); j++){
                if(requests[i].substr(requests[i].find(' ')) == processes[currentP].segTable[currentS].pageTable[j].value){
                    inTable = true;
                    break;
                }
            }
            if(inTable){
                continue;
            }
            else if(processes[currentP].segTable[currentS].pageTable.size() < r){
                pageFaults[currentP] += 2;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
            else{
                pageFaults[currentP] += 1;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                processes[currentP].segTable[currentS].pageTable.pop_front();
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
        }
    }
    int total = 0;
    cout << "FIFO page replacement: \n";
    for(int i = 0; i < k; i++){
        cout << processes[i].pid << ": " << pageFaults[i] << endl;
        total += pageFaults[i];
    }
    cout << "Total: " << total << "\n\n";
}

// Performs LIFO page replacement
void LIFO(int sl, int ps, int r, int k, vector<Process> processes, vector<string> requests){
    int pageFaults[k] = { 0 };
    for(int i = 0; i < requests.size(); i++){                                                   // Iterates through requests
        int currentP;
        int currentS = 0;
        int id = stoi(requests[i].substr(0, requests[i].find(' ')));
        for(int j = 0; j < k; j++){                                                             // Match request to process
            if(id == processes[j].pid){
                currentP = j;
                break;
            }
        }
        Address currAddr = hexToAddrData(requests[i], ps, sl);
        if(processes[currentP].segTable.empty()) {                                              // Match request to segment
            Segment *seg  = new Segment;
            seg->segNum = currAddr.segment;
            processes[currentP].segTable.push_back(*seg);
        }
        else{
            for(int j = 0; j < processes[currentP].segTable.size(); j++){
                if(currAddr.segment == processes[currentP].segTable[j].segNum){
                    currentS = j;
                    break;
                }
                else if((j == processes[currentP].segTable.size() - 1) && (currentS == 0)){
                    Segment *seg  = new Segment;
                    seg->segNum = currAddr.segment;
                    processes[currentP].segTable.push_back(*seg);
                    currentS = j + 1;
                }
            }
        }
        if(processes[currentP].segTable[currentS].pageTable.empty()){                           // Add page to table
            pageFaults[currentP] += 2;                                                          // 2 Page faults: allocate page, add page
            Page *pg = new Page;
            pg->pageNum = currAddr.page;
            pg->value = requests[i].substr(requests[i].find(' '));
            processes[currentP].segTable[currentS].pageTable.push_back(*pg);
        }
        else{
            bool inTable = false;
            for(int j = 0; j< processes[currentP].segTable[currentS].pageTable.size(); j++){
                if(requests[i].substr(requests[i].find(' ')) == processes[currentP].segTable[currentS].pageTable[j].value){
                    inTable = true;
                    break;
                }
            }
            if(inTable){
                continue;
            }
            else if(processes[currentP].segTable[currentS].pageTable.size() < r){
                pageFaults[currentP] += 2;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
            else{
                pageFaults[currentP] += 1;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                processes[currentP].segTable[currentS].pageTable.pop_back();
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
        }
    }
    int total = 0;
    cout << "LIFO page replacement: \n";
    for(int i = 0; i < k; i++){
        cout << processes[i].pid << ": " << pageFaults[i] << endl;
        total += pageFaults[i];
    }
    cout << "Total: " << total << "\n\n";
}

// Performs LRU-X page replacement
void LRUX(int sl, int ps, int r, int x, int k, vector<Process> processes, vector<string> requests){
    int pageFaults[k] = { 0 };
    for(int i = 0; i < requests.size(); i++){                                                   // Iterates through requests
        int currentP;
        int currentS = 0;
        int id = stoi(requests[i].substr(0, requests[i].find(' ')));
        for(int j = 0; j < k; j++){                                                             // Match request to process
            if(id == processes[j].pid){
                currentP = j;
                break;
            }
        }
        Address currAddr = hexToAddrData(requests[i], ps, sl);
        if(processes[currentP].segTable.empty()) {                                              // Match request to segment
            Segment *seg  = new Segment;
            seg->segNum = currAddr.segment;
            processes[currentP].segTable.push_back(*seg);
        }
        else{
            for(int j = 0; j < processes[currentP].segTable.size(); j++){
                if(currAddr.segment == processes[currentP].segTable[j].segNum){
                    currentS = j;
                    break;
                }
                else if((j == processes[currentP].segTable.size() - 1) && (currentS == 0)){
                    Segment *seg  = new Segment;
                    seg->segNum = currAddr.segment;
                    processes[currentP].segTable.push_back(*seg);
                    currentS = j + 1;
                }
            }
        }
        if(processes[currentP].segTable[currentS].pageTable.empty()){                           // Add page to table
            pageFaults[currentP] += 2;                                                          // 2 Page faults: allocate page, add page
            Page *pg = new Page;
            pg->pageNum = currAddr.page;
            pg->value = requests[i].substr(requests[i].find(' '));
            pg->times.push_back(i);
            processes[currentP].segTable[currentS].pageTable.push_back(*pg);
        }
        else{
            bool inTable = false;
            for(int j = 0; j< processes[currentP].segTable[currentS].pageTable.size(); j++){
                if(requests[i].substr(requests[i].find(' ')) == processes[currentP].segTable[currentS].pageTable[j].value){
                    inTable = true;
                    break;
                }
            }
            if(inTable){
                continue;
            }
            else if(processes[currentP].segTable[currentS].pageTable.size() < r){
                pageFaults[currentP] += 2;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                pg->times.push_back(i);
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
            else{
                pageFaults[currentP] += 1;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                pg->times.push_back(i);
                int remove = -1;
                int recentX;
                for(int j = 0; j < r; j++){
                    if(remove == -1){
                        if(processes[currentP].segTable[currentS].pageTable[j].times.size() < x){
                            remove = j;
                            recentX = processes[currentP].segTable[currentS].pageTable[j].times.back();
                        }
                        else{
                            remove = j;
                            recentX = processes[currentP].segTable[currentS].pageTable[j].times[processes[currentP].segTable[currentS].pageTable[j].times.size() - x];
                        }
                    }
                    else{
                        if(processes[currentP].segTable[currentS].pageTable[j].times.size() < x){
                            if(processes[currentP].segTable[currentS].pageTable[j].times.back() < recentX){
                                remove = j;
                                recentX = processes[currentP].segTable[currentS].pageTable[j].times.back();
                            }
                        }
                        else{
                            if(processes[currentP].segTable[currentS].pageTable[j].times[processes[currentP].segTable[currentS].pageTable[j].times.size() - x] < recentX){
                                remove = j;
                                recentX = processes[currentP].segTable[currentS].pageTable[j].times[processes[currentP].segTable[currentS].pageTable[j].times.size() - x];
                            }
                        }
                    }
                }
                processes[currentP].segTable[currentS].pageTable.erase(processes[currentP].segTable[currentS].pageTable.begin()+remove);
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
        }
    }
    int total = 0;
    cout << "LRU-" << x << " page replacement: \n";
    for(int i = 0; i < k; i++){
        cout << processes[i].pid << ": " << pageFaults[i] << endl;
        total += pageFaults[i];
    }
    cout << "Total: " << total << "\n\n";
}

// Performs LDF page replacement
void LDF(int sl, int ps, int r, int k, vector<Process> processes, vector<string> requests){
    int pageFaults[k] = { 0 };
    for(int i = 0; i < requests.size(); i++){                                                   // Iterates through requests
        int currentP;
        int currentS = 0;
        int id = stoi(requests[i].substr(0, requests[i].find(' ')));
        for(int j = 0; j < k; j++){                                                             // Match request to process
            if(id == processes[j].pid){
                currentP = j;
                break;
            }
        }
        Address currAddr = hexToAddrData(requests[i], ps, sl);
        if(processes[currentP].segTable.empty()) {                                              // Match request to segment
            Segment *seg  = new Segment;
            seg->segNum = currAddr.segment;
            processes[currentP].segTable.push_back(*seg);
        }
        else{
            for(int j = 0; j < processes[currentP].segTable.size(); j++){
                if(currAddr.segment == processes[currentP].segTable[j].segNum){
                    currentS = j;
                    break;
                }
                else if((j == processes[currentP].segTable.size() - 1) && (currentS == 0)){
                    Segment *seg  = new Segment;
                    seg->segNum = currAddr.segment;
                    processes[currentP].segTable.push_back(*seg);
                    currentS = j + 1;
                }
            }
        }
        if(processes[currentP].segTable[currentS].pageTable.empty()){                           // Add page to table
            pageFaults[currentP] += 2;                                                          // 2 Page faults: allocate page, add page
            Page *pg = new Page;
            pg->pageNum = currAddr.page;
            pg->value = requests[i].substr(requests[i].find(' '));
            processes[currentP].segTable[currentS].pageTable.push_back(*pg);
        }
        else{
            bool inTable = false;
            for(int j = 0; j< processes[currentP].segTable[currentS].pageTable.size(); j++){
                if(requests[i].substr(requests[i].find(' ')) == processes[currentP].segTable[currentS].pageTable[j].value){
                    inTable = true;
                    break;
                }
            }
            if(inTable){
                continue;
            }
            else if(processes[currentP].segTable[currentS].pageTable.size() < r){
                pageFaults[currentP] += 2;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
            else{
                pageFaults[currentP] += 1;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                int remove = 0;
                for(int j = 1; j < r; j++){
                    if(abs(currAddr.page - processes[currentP].segTable[currentS].pageTable[remove].pageNum) > abs(currAddr.page - processes[currentP].segTable[currentS].pageTable[j].pageNum)){
                        remove = j;
                    }
                    else if(abs(currAddr.page - processes[currentP].segTable[currentS].pageTable[remove].pageNum) == abs(currAddr.page - processes[currentP].segTable[currentS].pageTable[j].pageNum)){
                        if(processes[currentP].segTable[currentS].pageTable[remove].pageNum > processes[currentP].segTable[currentS].pageTable[j].pageNum){
                            remove = j;
                        }
                    }
                }
                processes[currentP].segTable[currentS].pageTable.erase(processes[currentP].segTable[currentS].pageTable.begin()+remove);
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
        }
    }
    int total = 0;
    cout << "LDF page replacement: \n";
    for(int i = 0; i < k; i++){
        cout << processes[i].pid << ": " << pageFaults[i] << endl;
        total += pageFaults[i];
    }
    cout << "Total: " << total << "\n\n";
}

//Performs OPT-X page replacement
void OPTX(int sl, int ps, int r, int x, int k, vector<Process> processes, vector<string> requests){
    int pageFaults[k] = { 0 };
    for(int i = 0; i < requests.size(); i++){                                                   // Iterates through requests
        int currentP;
        int currentS = 0;
        int id = stoi(requests[i].substr(0, requests[i].find(' ')));
        for(int j = 0; j < k; j++){                                                             // Match request to process
            if(id == processes[j].pid){
                currentP = j;
                break;
            }
        }
        Address currAddr = hexToAddrData(requests[i], ps, sl);
        if(processes[currentP].segTable.empty()) {                                              // Match request to segment
            Segment *seg  = new Segment;
            seg->segNum = currAddr.segment;
            processes[currentP].segTable.push_back(*seg);
        }
        else{
            for(int j = 0; j < processes[currentP].segTable.size(); j++){
                if(currAddr.segment == processes[currentP].segTable[j].segNum){
                    currentS = j;
                    break;
                }
                else if((j == processes[currentP].segTable.size() - 1) && (currentS == 0)){
                    Segment *seg  = new Segment;
                    seg->segNum = currAddr.segment;
                    processes[currentP].segTable.push_back(*seg);
                    currentS = j + 1;
                }
            }
        }
        if(processes[currentP].segTable[currentS].pageTable.empty()){                           // Add page to table
            pageFaults[currentP] += 2;                                                          // 2 Page faults: allocate page, add page
            Page *pg = new Page;
            pg->pageNum = currAddr.page;
            pg->value = requests[i].substr(requests[i].find(' '));
            processes[currentP].segTable[currentS].pageTable.push_back(*pg);
        }
        else{
            bool inTable = false;
            for(int j = 0; j< processes[currentP].segTable[currentS].pageTable.size(); j++){
                if(requests[i].substr(requests[i].find(' ')) == processes[currentP].segTable[currentS].pageTable[j].value){
                    inTable = true;
                    break;
                }
            }
            if(inTable){
                continue;
            }
            else if(processes[currentP].segTable[currentS].pageTable.size() < r){
                pageFaults[currentP] += 2;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
            else{
                pageFaults[currentP] += 1;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                int farthest = 0;
                int remove = 0;
                for(int j = 0; j < r; j++){
                    for(int z = 1; z < x; z++){
                        if((i + z) > requests.size() - 1) { break; }
                        else if(processes[currentP].segTable[currentS].pageTable[j].pageNum == hexToAddrData(requests[i + z],ps,sl).page){
                            if(z > farthest) {
                                farthest = z;
                                remove = j;
                            }
                            break;
                        }
                        else if(z == x - 1){
                            farthest = -1;
                            remove = j;
                            break;
                        }
                    }
                    if(farthest == -1){ break; }
                }
                processes[currentP].segTable[currentS].pageTable.erase(processes[currentP].segTable[currentS].pageTable.begin()+remove);
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
            }
        }
    }
    int total = 0;
    cout << "OPT-" << x << " page replacement: \n";
    for(int i = 0; i < k; i++){
        cout << processes[i].pid << ": " << pageFaults[i] << endl;
        total += pageFaults[i];
    }
    cout << "Total: " << total << "\n\n";
}

//Performs Working Set page replacement
void WS(int sl, int ps, int r, int minimum, int maximum, int k, vector<Process> processes, vector<string> requests){
    int pageFaults[k] = { 0 };
    for(int i = 0; i < requests.size(); i++){                                                   // Iterates through requests
        int currentP;
        int currentS = 0;
        int id = stoi(requests[i].substr(0, requests[i].find(' ')));
        for(int j = 0; j < k; j++){                                                             // Match request to process
            if(id == processes[j].pid){
                currentP = j;
                break;
            }
        }
        Address currAddr = hexToAddrData(requests[i], ps, sl);
        if(processes[currentP].segTable.empty()) {                                              // Match request to segment
            Segment *seg  = new Segment;
            seg->segNum = currAddr.segment;
            processes[currentP].segTable.push_back(*seg);
        }
        else{
            for(int j = 0; j < processes[currentP].segTable.size(); j++){
                if(currAddr.segment == processes[currentP].segTable[j].segNum){
                    currentS = j;
                    break;
                }
                else if((j == processes[currentP].segTable.size() - 1) && (currentS == 0)){
                    Segment *seg  = new Segment;
                    seg->segNum = currAddr.segment;
                    processes[currentP].segTable.push_back(*seg);
                    currentS = j + 1;
                }
            }
        }
        if(processes[currentP].segTable[currentS].pageTable.empty()){                           // Add page to table
            pageFaults[currentP] += 2;                                                          // 2 Page faults: allocate page, add page
            Page *pg = new Page;
            pg->pageNum = currAddr.page;
            pg->value = requests[i].substr(requests[i].find(' '));
            pg->times.push_back(i);
            processes[currentP].segTable[currentS].pageTable.push_back(*pg);
        }
        else{
            bool inTable = false;
            for(int j = 0; j< processes[currentP].segTable[currentS].pageTable.size(); j++){
                if(requests[i].substr(requests[i].find(' ')) == processes[currentP].segTable[currentS].pageTable[j].value){
                    inTable = true;
                    break;
                }
            }
            if(inTable){
                continue;
            }
            else{
                pageFaults[currentP] += 2;
                Page *pg = new Page;
                pg->pageNum = currAddr.page;
                pg->value = requests[i].substr(requests[i].find(' '));
                pg->times.push_back(i);
                processes[currentP].segTable[currentS].pageTable.push_back(*pg);
                for(int j = processes[currentP].segTable[currentS].pageTable.size() - 1; j >= 0 ; j--){
                    if(abs(processes[currentP].segTable[currentS].pageTable[j].times.back() - i) == r){
                        processes[currentP].segTable[currentS].pageTable.erase(processes[currentP].segTable[currentS].pageTable.begin()+j);
                    }
                }
                if(processes[currentP].segTable[currentS].pageTable.size() > maximum){
                    int remove = 0;
                    int delta = abs(processes[currentP].segTable[currentS].pageTable[0].times.back() - i);
                    for(int j = 1; j > processes[currentP].segTable[currentS].pageTable.size(); j++){
                        if(abs(processes[currentP].segTable[currentS].pageTable[j].times.back() - i) > delta){
                            remove = j;
                            delta = abs(processes[currentP].segTable[currentS].pageTable[j].times.back() - i);
                        }
                    }
                    processes[currentP].segTable[currentS].pageTable.erase(processes[currentP].segTable[currentS].pageTable.begin()+remove);
                }

            }
        }
    }
    int total = 0;
    cout << "Working Set page replacement: \n";
    for(int i = 0; i < k; i++){
        cout << processes[i].pid << ": " << pageFaults[i] << endl;
        total += pageFaults[i];
    }
    cout << "Total: " << total << endl;
}

int main(int argc, char* argv[]) {
    if(argc != 2){								// Check if there are enough arguments
        perror("Invalid number of arguments.");	// Print error message
        exit(1);								// Exit the program
    }
    string filename = argv[1];          		// Text file argument

    int tp;                                     // Declare total number of page frames
    int sl;                                     // Declare segment length
    int ps;                                     // Declare page size
    int r;                                      // Declare page frames per process/delta
    int x;                                      // X value for LRU-X and OPT-X
    int minimum;                                // Declare min frame
    int maximum;                                // Declare max frame
    int k;                                      // Declare total number of processes
    vector<Process> processes;                  // Declare process vector
    vector<string> requests;
    fileRead(filename, tp, sl, ps, r, x, minimum, maximum, k, processes, requests);

    FIFO(sl, ps, r, k, processes, requests);
    clearTables(processes, k);

    LIFO(sl, ps, r, k, processes, requests);
    clearTables(processes, k);

    LRUX(sl, ps, r, x, k, processes, requests);
    clearTables(processes, k);

    LDF(sl, ps, r, k, processes, requests);
    clearTables(processes, k);

    OPTX(sl, ps, r, x, k, processes, requests);
    clearTables(processes, k);

    WS(sl, ps, r, minimum, maximum, k, processes, requests);
    clearTables(processes, k);

}