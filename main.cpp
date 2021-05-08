#include <iostream>
#include <queue>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

typedef struct {
    int id;
    char processName;
}Process;

typedef struct {
    int count;
    queue<Process> Queue;
}Semaphore;

Semaphore  empty, full, mutex;

vector<int> buffer(8 , -1);

int N = int(buffer.size());
int pId = 1;
int cId = 1;

random_device rd;
default_random_engine gen = default_random_engine(rd());
uniform_int_distribution<int> dis(0, 7);

void printInformation();
void producerOperation(Process, int &, int &);
void consumerOperation();

void P(Semaphore &s, Process process, int &flag) {
    --s.count;
    if (s.count < 0) {
        s.Queue.push(process);
        flag = 0;
//        TODO:阻塞调用进程
    } else if (s.count >= 8) {
        flag = 2;
    }
}

void V(Semaphore &s, Process process) {
    ++s.count;
    if (s.count <= 0) {
//        TODO:
        Process q = s.Queue.front();
        s.Queue.pop();
//        consumerOperation();
    }
}

void producerOperation(Process process, int &flag1, int &flag2) {
    if (flag1 == 1) {
        if (flag2 == 1) {
            for (int & i : buffer) {
                if (i == -1) {
                    i = process.id;
                    break;
                }
            }
        } else {
//            TODO: mutex
            printf("Current mutex is 0, p is hung.");
        }
    } else if (flag1 == 2) {
        if (flag2 == 0) {
//            TODO: mutex
        } else if (flag2 == 1) {

        }
    }
}

void Producer(Process process) {
    int flag1 = 1;
    int flag2 = 1;
    P(empty, process, flag1);
    P(mutex, process, flag2);

    producerOperation(process, flag1, flag2);

    V(mutex, process);
    V(full, process);
    cout<<empty.count<<endl;
}

void consumerOperation(int& flag1, int& flag2) {
    if (flag1 != 0) {
        if (empty.Queue.empty()) {
            while (true) {
                int randomIndex = dis(gen);
                if (buffer[randomIndex] != -1) {
                    buffer[randomIndex] = -1;
                    break;
                }
            }
        } else {
            int randomIndex = dis(gen);
            buffer[randomIndex] = empty.Queue.front().id;
        }
    }
}

void Consumer(Process process) {
    int flag1 = 1;
    int flag2 = 1;
    P(full, process, flag1);
    P(mutex, process, flag2);
//    TODO:
    consumerOperation(flag1, flag2);

    V(mutex, process);
    V(empty, process);
}

void operation(char a) {
    if (a == 'P' || a == 'p') {
        Process p;
        p.id = pId;
        p.processName = 'P';
        ++pId;
        Producer(p);
        printInformation();
    } else if (a == 'C' || a == 'c') {
        Process c;
        c.id = cId;
        c.processName = 'C';
        ++cId;
        Consumer(c);
        printInformation();
    } 
}

void printInformation() {
    printf("\n*************** Information ***************\n");
    printf("Current Memory Buffer: \n");
    for (int i : buffer) {
        printf("%d\t", i);
    }
    printf("\n");

    printf("Current Empty Queue: \n");
    if (!empty.Queue.empty()) {
        queue<Process> q = empty.Queue;
        for (int i = 0; i < empty.Queue.size(); ++i) {
            int j = q.front().id;
            q.pop();
            printf("P%d\t", j);
        }
        printf("\n");
    } else {
        printf("NULL\n");
    }

    printf("Current Full Queue: \n");
    if (!full.Queue.empty()) {
        queue<Process> q = full.Queue;
        for (int i = 0; i < full.Queue.size(); ++i) {
            int j = q.front().id;
            q.pop();
            printf("C%d\t", j);
        }
        printf("\n");
    } else {
        printf("NULL\n");
    }

    printf("Current Mutex Queue: \n");
    if (!mutex.Queue.empty()) {
        queue<Process> q = mutex.Queue;
        int j = q.front().id;
        printf("M%d\t", j);
    } else {
        printf("NULL\n");
    }

    while (true){
        char c;
        printf("Do you want to quit? (y/n)\n");
        cin>>c;
        if (c == 'y' || c == 'Y') {
            exit(0);
        }
        else if (c == 'n' || c == 'N') {
            break;
        } else{
            continue;
        }
    }


    while (true) {
        printf("Please enter a process(P/p/C/c/E/e):\n");
        char c;
        cin>>c;
        printf("\n");
        if (c != 'P' && c != 'p' && c != 'C' && c != 'c' && c != 'E' && c != 'e') {
            continue;
        } else {
            operation(c);
            break;
        }
    }
}

void initial() {
    queue<Process> p, c, m;
    empty.count = N;
    empty.Queue = p;
    full.count = 0;
    full.Queue = c;
    mutex.count = 1;
    mutex.Queue = m;
}

int main() {
    initial();
    printInformation();
    return 0;
}
