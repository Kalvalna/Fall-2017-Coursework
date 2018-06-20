#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <deque>
#include <sstream>
#include <sys/wait.h>

using namespace std;

// Struct that holds process details
struct Process{
    int deadline;					// Declare deadline variable
    int comTime;					// Declare computation time variable
    int remainingComp;              // Declare remaining computation time variable
    deque<int> reqComTime;	        // Declare variable that holds time to complete each request
    deque<int> request;             // Declare variable that holds requested resources
    deque<int> release;             // Declare variable that holds resources to release
    int requestsCompleted = 0;      // Initialize number of requests completed to 0
    int maxRequests;                // Declare variable that holds maximum number of requests in a process
    int laxity;						// Declare laxity variable
    bool complete = false;          // Initialize variable that marks completion to false
};

// Function that waits semaphore
int waitSem(int semid){
    struct sembuf swait;
    swait.sem_num = 0;
    swait.sem_op = -1;
    swait.sem_flg = 0;
    semop(semid, &swait, 1);
}

// Function that signals semaphore
int sigSem(int semid){
    struct sembuf ssignal;
    ssignal.sem_num = 0;
    ssignal.sem_op = 1;
    ssignal.sem_flg = 0;
    semop(semid, &ssignal, 1);
}

// Function that splits string by commas and stores it in a deque
deque<int> splitString(string line){
    deque<int> storage;					        // Declare deque to store words
    stringstream ss(line);					    // Insert the line into a stream
    string token;							    // Declare string variable to get words from line

    while(getline(ss, token, ',')){			    // Find numbers by using a comma as a delimiter
        storage.push_back(atoi(token.c_str()));	// Stores the word into a deque
    }

    return storage;							    // return the deque
}

// Function that determines if there are enough available resources
bool isSafe(int m, vector<int> available, Process check){
    for (int i = 0; i < m; i++){
        if(check.request[i] > available[i]){
            return false;
        }
    }
    return true;
}

// Function that performs the Banker's algorithm and selects the next process
int selectNext(int m, int n, int type, int* currentProcess, vector<int> available, Process processes[]){
    int next = 5000;                                       // Declare next variable to return
    int lowestRequest = 1000;                              // Initialize variable to check the lowest amount of requests a process has completed

    // Find lowest request completed
    for (int i = 0; i < n; i++){
        if(processes[i].requestsCompleted < lowestRequest){
            lowestRequest = processes[i].requestsCompleted;
        }
    }

    // Find starting point for comparison (assign next to current process if all processes with lowest request are unsafe)
    for (int i = 0; i < n; i++){
        if ((processes[i].complete == false) && (processes[i].requestsCompleted == lowestRequest)){
            if (isSafe(m, available, processes[i])){
                next = i;
                break;
            }
        }
        if (next == 5000){
            next = *currentProcess;
        }

    }
    for (int i = 0; i < n; i++){
        if(i == *currentProcess) { continue; }
        // SJF scheduling
        else if(type == 0){
            if((processes[i].remainingComp < processes[next].remainingComp) && (processes[i].complete == false) && isSafe(m, available, processes[i])){
                next = i;
            }
            // EDF tie-breaker
            else if((processes[next].remainingComp == processes[i].remainingComp) && (processes[i].complete == false) && isSafe(m, available, processes[i])){
                next = i;
            }
        }
        // LLF scheduling
        else if(type == 1){
            if((processes[i].laxity < processes[next].laxity) && (processes[i].complete == false) && isSafe(m, available, processes[i])){
                next = i;
            }
            // EDF tie-breaker
            else if((processes[next].laxity == processes[i].laxity) && (processes[i].complete == false) && isSafe(m, available, processes[i])){
                if(processes[i].deadline < processes[next].deadline){
                    next = i;
                }
            }
        }
    }
    return next;

}

// Perform child functions and returns elapsed time
int childFun(int m, int* currentProcess, int* currentTime, vector<int> available, vector<vector<int>> &allocation, vector<vector<int>> &need, Process processes[], vector<int> &deadlineMisses){
    int elapsed = *currentTime + processes[*currentProcess].reqComTime.front();
    processes[*currentProcess].reqComTime.pop_front();

    for (int i = 0; i < m; i++){
        available[i] -= (processes[*currentProcess].request.front() - processes[*currentProcess].release.front());
        allocation[*currentProcess][i] += (processes[*currentProcess].request.front() - processes[*currentProcess].release.front());
        need[*currentProcess][i] -= processes[*currentProcess].request.front();
        processes[*currentProcess].request.pop_front();
        processes[*currentProcess].release.pop_front();
    }

    processes[*currentProcess].requestsCompleted += 1;
    if (processes[*currentProcess].requestsCompleted == processes[*currentProcess].maxRequests) {
        processes[*currentProcess].complete = true;
        if (elapsed > processes[*currentProcess].deadline){
            deadlineMisses.push_back(*currentProcess);
        }
        for (int i = 0; i < m; i++){
            available[i] += allocation[*currentProcess][i];
            allocation[*currentProcess][i] = 0;
        }
    }
    return elapsed - *currentTime;
}

// Function that prints the details of the program after each request is complete
void printDetails(int n, int m, vector<int> available, vector<vector<int>> const& allocation, vector<vector<int>> const&  need, vector<int> deadlineMisses){
    cout << "Current state: \n" << endl;
    cout << "Available: " << endl;
    for (int i = 0; i < m; i++){
        cout << "Resource "<< i + 1 << ": " << available[i] << endl;
    }
    cout << "\nAllocation: \n";
    cout << "Resource\t";
    for (int i = 1; i <= m; i++) {
        cout << i << "\t";
    }
    cout << endl;
    for (int i = 0; i < n; i++){
        cout << "Process " << i + 1 << "\t";
        for (int j = 0; j < m; j++){
            cout << allocation[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\nNeed: " << endl;
    cout << "Resource\t";
    for (int i = 1; i <= m; i++) {
        cout << i << "\t";
    }
    cout << endl;
    for (int i = 0; i < n; i++){
        cout << "Process " << i + 1 << "\t";
        for (int j = 0; j < m; j++){
            cout << need[i][j] << "\t";
        }
        cout << "\n";
    }
    cout << "\nDeadline misses: ";
    for (int i = 0; i < deadlineMisses.size(); i++){
        cout << deadlineMisses[i] + 1<< " ";
    }
    cout << "\n\n";
}

int main(int argc, char *argv[]) {

    if(argc != 2){								                // Check if there are enough arguments
        perror("Invalid number of arguments.");	                // Print error message
        exit(1);								                // Exit the program
    }

    string filename = argv[1];                                  // Text file argument
    fstream file(filename);			                            // Opens the file for reading/writing
    string line;                                                // Declare variable to hold line from file

    getline(file,line);
    int m = atoi(line.c_str());                                 // Initialize resource variable
    getline(file,line);
    int n = atoi(line.c_str());                                 // Initialize process variable
    vector<int> available(0);                                   // Initialize available array
    vector<vector<int>> max(n, vector<int>(m, 0));              // Initialize max vector
    vector<vector<int>> allocated(n, vector<int>(m, 0));        // Initialize allocation vector
    vector<vector<int>> need(n, vector<int>(m, 0));             // Initialize need vector
    vector<int> deadlineMisses;                                 // Declare deadline misses vector
    int currentProcess = 0;                                     // Initialize current process
    int type;                                                   // Declare variable for type of scheduling (0 for SJF, 1 for LLF)
    Process processes[n];				                        // Declare an array of Processes

    // While loop that initializes variables based on what's read
    while(getline(file,line)){
        if(line.find("available") != string::npos){                         // If string contains "available"
            for(int i = 0; i < m; i++){                                     // Loop and fill available array
                string temp = line.substr(line.find('=') + 2);              // by searching for number after equal
                available.push_back(atoi(temp.c_str()));
                if(i == m - 1){ break; }                                    // Break when last resource added so that getline doesn't skip a line
                getline(file, line);
            }
        }
        else if(line.find("max") != string::npos){                          // If string contains "max", do same as above
            for(int i = 0; i < n; i++){
                for(int j = 0; j < m; j++){
                    string temp = line.substr(line.find('=') + 2);
                    max[i][j] = atoi(temp.c_str());
                    if((i == n - 1) && (j == m - 1)){ break; }
                    getline(file, line);
                }
            }
        }
        else if(line.find("process") != string::npos){                          // If line contains process, modify process variable to contain relevant details
            int requestNum = 0;                                                 // Declare variable to keep track of requests in a process
            int calcTime = 0;                                                   // Declare calculation time variable to track calculation time per request
            getline(file,line);
            processes[currentProcess].deadline = atoi(line.c_str());            // Assign deadline as first line after process
            getline(file,line);
            processes[currentProcess].comTime = atoi(line.c_str());             // Assign computation time as 2nd line after process
            processes[currentProcess].remainingComp = atoi(line.c_str());       // Assign remaining computation time to 2nd line after process
            processes[currentProcess].laxity = processes[currentProcess].deadline - processes[currentProcess].remainingComp;    // Calculate laxity
            getline(file,line);
            while(line.find("end") == string::npos){                                                        // Loop until end of process details
                if((line.find("calculate") != string::npos)){                                               // Add required time for calculate
                    string temp = line.substr(line.find('(') + 1,line.find(')') - 1);
                    calcTime += atoi(temp.c_str());
                    getline(file, line);
                }
                else if(line.find("request") != string::npos){                                              // Add requests to process and fill in need array
                    calcTime += 1;
                    deque<int> requests = splitString(line.substr(line.find('(') + 1,line.find(')') - 1));
                    for(int i = 0; i < m; i++){
                        int temp = requests.front();
                        processes[currentProcess].request.push_back(temp);
                        need[currentProcess][i] += temp;
                        requests.pop_front();
                    }
                    getline(file, line);
                    requestNum += 1;
                }
                else if((line.find("useresources") != string::npos)){                                       // Same as calculate except check for next request or end
                    string temp = line.substr(line.find('(') + 1,line.find(')') - 1);
                    calcTime += atoi(temp.c_str());
                    if(file.peek() == 'c'){                                                                 // Start next request's details if next line is calculate
                        processes[currentProcess].reqComTime.push_back(calcTime);
                        requestNum += 1;
                        calcTime = 0;
                        for(int i = 0; i < m; i++){
                            processes[currentProcess].release.push_back(0);
                        }
                    }
                    else if (file.peek() == 'e'){                                                           // Add process details if end of process details
                        processes[currentProcess].reqComTime.push_back(calcTime);
                        processes[currentProcess].maxRequests = requestNum;
                    }
                    getline(file,line);
                }
                else if((line.find("release") != string::npos)){                                            // Add release integers and start next request's details
                    calcTime += 1;
                    deque<int> release =  splitString(line.substr(line.find('(') + 1,line.find(')') - 1));
                    for(int i = 0; i < m; i++){
                        int temp = release.front();
                        processes[currentProcess].release.push_back(temp);
                        release.pop_front();
                    }
                    processes[currentProcess].reqComTime.push_back(calcTime);
                    calcTime = 0;
                    getline(file, line);
                }
                else{
                    getline(file, line);
                }
            }
            processes[currentProcess].maxRequests = requestNum;         // Assign max request
            currentProcess += 1;
        }
    }

    cout << "Please enter either 0 or 1" << endl;       // Take in user input
    cout << "0: Shortest Job First (SJF)\n";            // to determine scheduling
    cout << "1: Least Laxity First (LLF)\n";            // type
    cin >> type;
    cout << endl;

    int key = 867616;                   // Initialize key for shared memory
    int totalRequests = 0;
    for (int i = 0; i < n; i++){
        totalRequests += processes[i].maxRequests;
    }

    // Create semaphores
    int csem, psem;

    csem = semget(key/2, n, 0666 | IPC_CREAT);
    psem = semget(key*2, n, 0666 | IPC_CREAT);

    // Declare variables for shared memory and attach to memory space
    int shmid;                          // Declare shared memory id
    int *pmem;							// Declare address space to attach shared memory

    if((shmid = shmget(key, 50, 0666 | IPC_CREAT)) < 0) {
        cout << "Shared memory creation failed." << endl;
        exit(-1);
    }
    pmem = (int *)shmat(shmid, NULL, 0);
    if(pmem == (int *)(-1)){
        cout << "Shared memory attachment failed." << endl;
    }

    int pid;									// Declare process id
    int parentId = getpid();                    // Initialize parent id
    int* time = pmem + 1;
    int* next = pmem + 2;
    int* requestsComp = pmem + 3;
    *time = 0;
    *next = 0;
    *requestsComp = 0;
    sigSem(csem);

    // Create and execute processes
    pid = fork();
    if(pid < 0) {                                           // Check if fork failed
        perror("Error creating child process");             // Print error
        exit(1);                                            // Exit
    }
    else if (pid == 0){                                     // Child process
        while(*requestsComp <= totalRequests){
            waitSem(psem);
            cout << "Performing request from Process " << *next + 1 << "\n\n";
            *time += childFun(m, next, time, available, allocated, need, processes, deadlineMisses);
            printDetails(n, m, available, allocated, need, deadlineMisses);
            *requestsComp += 1;
            sigSem(csem);
        }
    }
    else{                                                   // Parent Process
        while(*requestsComp <= totalRequests){
            waitSem(csem);
            *next = selectNext(m, n, type, next, available, processes);
            sigSem(psem);
        }
    }

    int status;							// Current status of waiting for child
    for (int i = 0; i < n; i++){		// Iterate through each child and wait for them to stop
        wait(&status);
    }

    shmdt(pmem);                        // Detach Shared memory
    semctl(shmid, 0, IPC_RMID, 0);      // Destroy shared memory
}