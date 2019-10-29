#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Node {
    public:
        int id;
        int color;
        std::vector<int> interf;
        std::vector<int> ogInterf;

        Node(int pId) {
            id = pId;
        }
        
        Node () {id = 0;}

        ~Node () {}

        Node copy() {
            Node n;
            n.id = this->id;
            n.color = this->color;
            int i;
            for(i = 0; i < this->interf.size(); i++) {
                n.interf.push_back(this->interf[i]);
                n.ogInterf.push_back(this->interf[i]);
            }
            return n;
        }


};


std::vector<Node> IN_LIST, STACK, CURRENT_GRAPH; //Grafo de entrada, pilha, grafo atual
std::vector<int> CURRENT_NODES;   //Nos no grafo atual
int K, CURRENT_K, FILEEND = 0; //Registradores fisicos, valor atual de K
std::string G_NAME;  //Nome do grafo



int getNode(std::vector<Node> l, int id) {

    int i = 0, size = l.size();
    for(i; i < size; i++) {
        if(l[i].id == id) {
            return i;
        }
    } 
    return -1;
};

std::string readInput() {
    char c;
    std::string str;
    int n = 0;

    while(scanf("%c", &c)!=EOF){
        if(c == '\n'){n = 1; break;}
        str.append(1, tolower(c));
    }

    if(!n){FILEEND = 1;}

    return str;
}

std::vector<std::string> split(std::string str) {
    std::vector<std::string> tokens;
    std::string delimiter = " ";

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    token = str.substr(0, pos);
    tokens.push_back(token);
    return tokens;
}



void copyGraph() {
    int i;
    Node n;
    CURRENT_GRAPH.clear();
    CURRENT_NODES.clear();
    for(i = 0; i < IN_LIST.size(); i++) {
        n = *(new Node(IN_LIST[i].id));
        int j;
        for(j = 0; j < IN_LIST[i].interf.size(); j++) {
            n.interf.push_back(IN_LIST[i].interf[j]);
            n.ogInterf.push_back(IN_LIST[i].interf[j]);
        }
        CURRENT_GRAPH.push_back(n);
        CURRENT_NODES.push_back(n.id);
    }
}

void treatInput() {
    G_NAME = split(readInput())[1];
    G_NAME = G_NAME.substr(0, G_NAME.length()-1);
    K = stoi(readInput().substr(2));
    std::string line;
    std::vector<std::string> lineVector;
    Node gNode;
    int i;

    line = readInput();
    while(true) {
        lineVector = split(line);
        gNode = *(new Node(stoi(lineVector[0])));
        for(i = 2; i < lineVector.size(); i++) {
            gNode.interf.push_back(stoi(lineVector[i]));
            gNode.ogInterf.push_back(stoi(lineVector[i]));
        }
        IN_LIST.push_back(gNode);
        CURRENT_NODES.push_back(gNode.id);

        if(FILEEND){
            break;
        }
        line = readInput();
    }
}


void push(int ix) {
    int i, j, k, l;
    Node n = CURRENT_GRAPH[ix];
    STACK.push_back(n);
    CURRENT_GRAPH.erase(CURRENT_GRAPH.begin() + ix);
    

/*
    for(i = 0; i < n.interf.size(); i++) {
        if(n.interf[i] > K-1) {
            int ix2 = getNode(CURRENT_GRAPH, n.interf[i]);
            for(j = 0; j < CURRENT_GRAPH[ix2].interf.size(); j++) {
                if(CURRENT_GRAPH[ix2].interf[j] == n.id) {
                    CURRENT_GRAPH[ix2].interf.erase(CURRENT_GRAPH[ix2].interf.begin() + j);
                    break;
                }
            }
        }
    }*/
    l = 0;
    for(i = 0; i < CURRENT_GRAPH.size(); i++) {
        /*if(l >= n.interf.size()){
            return;
        }*/
        for(j = 0; j < n.interf.size(); j++) {
            if(n.interf[j] > K-1){
                if(CURRENT_GRAPH[i].id == n.interf[j]) {
                    for(k = 0; k < CURRENT_GRAPH[i].interf.size(); k++) {
                        if(CURRENT_GRAPH[i].interf[k] == n.id) {
                            CURRENT_GRAPH[i].interf.erase(CURRENT_GRAPH[i].interf.begin() + k);
                            break;
                        }   
                    }
                }
            }
        }
        l++;
    }   
}

bool simplify() {
    //CURRENT_NODES.clear();
    int i, min = CURRENT_K  , ret = -1;
    for(i = 0; i < CURRENT_GRAPH.size(); i++) {
        if(ret == -1 && CURRENT_GRAPH[i].interf.size() < min){
            min = CURRENT_GRAPH[i].interf.size();
            ret = i;
        }else if (CURRENT_GRAPH[i].interf.size() < min) {
            min = CURRENT_GRAPH[i].interf.size();
            ret = i;
        }else if(ret != -1 && CURRENT_GRAPH[i].interf.size() == min) {
            if(CURRENT_GRAPH[i].id < CURRENT_GRAPH[ret].id) {
                min = CURRENT_GRAPH[i].interf.size();
                ret = i;
            }
        }
    }
        
    if(ret != -1){
        std::cout << "Push: " << CURRENT_GRAPH[ret].id << "\n";
        push(ret);
        return true;
    }else{
        return false;
    }
}

void potentialSpill() {
    int i, max = CURRENT_K, ret = -1;
    for(i = 0; i < CURRENT_GRAPH.size(); i++) {
        if(ret == -1){
            max = CURRENT_GRAPH[i].interf.size();
            ret = i;
        }else if (CURRENT_GRAPH[i].interf.size() > max) {
            max = CURRENT_GRAPH[i].interf.size();
            ret = i;
        }else if(CURRENT_GRAPH[i].interf.size() == max) {
            if(CURRENT_GRAPH[i].id < CURRENT_GRAPH[ret].id) {
                max = CURRENT_GRAPH[i].interf.size();
                ret = i;
            }
        }
    }
        
    std::cout << "Push: " << CURRENT_GRAPH[ret].id << " *\n";
    push(ret);
}


bool assign() {
    Node n = STACK.back();
    STACK.pop_back();
    int colors [CURRENT_K] = {0};

    int i, j;
    for(i = 0; i < n.ogInterf.size(); i++) {
        if(n.ogInterf[i] < CURRENT_K) {
            colors[n.ogInterf[i]]++;
        }else{
            for(j = 0; j < CURRENT_GRAPH.size(); j++) {
                if(n.ogInterf[i] == CURRENT_GRAPH[j].id){
                    colors[CURRENT_GRAPH[j].color]++;
                }
            }
        }
    }
    
    for(i = 0; i < CURRENT_K; i++) {
        if(colors[i] == 0){
            n.color = i;
            std::cout << "Pop: " << n.id << " -> " << n.color << "\n";
            CURRENT_GRAPH.push_back(n);
            return true;
        }
    }
    std::cout << "Pop: " << n.id << " -> NO COLOR AVAILABLE\n";
    CURRENT_GRAPH.push_back(n);
    return false;
}

void gambiarra(int n) {
    std::string s1 = to_string(n), s2 = to_string(K);
    int i = s1.length(), j = s2.length();
    for(i; i < j; i++) {
        std::cout << " ";
    }
}

int main() {
    treatInput();
    int r[K] = {0};
    
    std::cout << "Graph " << G_NAME << " -> Physical Registers: " << K << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "----------------------------------------\n";

    CURRENT_K = K;
    while(CURRENT_K >= 2) {
        copyGraph();
        bool flag = true;
        std::cout << "K = " << CURRENT_K << "\n\n";
        while(CURRENT_GRAPH.size() > 0) {
            flag = simplify();
            if(!flag) {
                potentialSpill();
            }
        }
        CURRENT_NODES.clear();
        while(STACK.size() > 0) {
            flag = assign();
            if(!flag) {
                r[CURRENT_K-1]++;
                break;
            }
        }
        std::cout << "----------------------------------------\n";
        CURRENT_K--;
    }
    std::cout << "----------------------------------------";
    int i;
    for(i = K-1; i > 0; i--) {
        if(i > 0) {
            std::cout << "\n";
        }
        std::cout << "Graph " << G_NAME << " -> K = ";
        gambiarra(i+1);
        std::cout << i+1 << ": ";
        
        if(r[i] != 0) {
            std::cout << "SPILL";
        } else {
            std::cout << "Successful Allocation";
        }
    }
}