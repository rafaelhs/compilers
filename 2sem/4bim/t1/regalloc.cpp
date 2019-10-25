#include <iostream>
#include <string>
#include <vector>

using namespace std;


class Node {
    public:
        int id;
        int color;
        vector<int> interf;
        vector<int> ogInterf;

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


vector<Node> IN_LIST, STACK, CURRENT_GRAPH; //Grafo de entrada, pilha, grafo atual
vector<int> CURRENT_NODES;   //Nos no grafo atual
int K, CURRENT_K, FILEEND = 0; //Registradores fisicos, valor atual de K
string G_NAME;  //Nome do grafo



int getNode(vector<Node> l, int id) {
    int i = 0, size = l.size();
    for(i; i < size; i++) {
        if(l[i].id == id) {
            return i;
        }
    } 
    return -1;
};

string readInput() {
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

vector<string> split(string str) {
    vector<string> tokens;
    std::string delimiter = " ";

    size_t pos = 0;
    string token;
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
    string line;
    vector<string> lineVector;
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
    int i, j;
    Node n = CURRENT_GRAPH[ix];
    STACK.push_back(n);
    CURRENT_GRAPH.erase(CURRENT_GRAPH.begin() + ix);
    


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
        cout << "Push: " << CURRENT_GRAPH[ret].id << "\n";
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
        
    cout << "Push: " << CURRENT_GRAPH[ret].id << " *\n";
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
            cout << "Pop: " << n.id << " -> " << n.color << "\n";
            CURRENT_GRAPH.push_back(n);
            return true;
        }
    }
    cout << "Pop: " << n.id << " -> NO COLOR AVAILABLE\n";
    CURRENT_GRAPH.push_back(n);
    return false;
}

void gambiarra(int n) {
    string s1 = std::to_string(n), s2 = std::to_string(K);
    int i = s1.length(), j = s2.length();
    for(i; i < j; i++) {
        cout << " ";
    }
}

int main() {
    treatInput();
    int r[K] = {0};
    
    cout << "Graph " << G_NAME << " -> Physical Registers: " << K << "\n";
    cout << "----------------------------------------\n";
    cout << "----------------------------------------\n";

    CURRENT_K = K;
    while(CURRENT_K >= 2) {
        copyGraph();
        bool flag = true;
        cout << "K = " << CURRENT_K << "\n\n";
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
        cout << "----------------------------------------\n";
        CURRENT_K--;
    }
    cout << "----------------------------------------";
    int i;
    for(i = K-1; i > 0; i--) {
        if(i > 0) {
            cout << "\n";
        }
        cout << "Graph " << G_NAME << " -> K = ";
        gambiarra(i+1);
        cout << i+1 << ": ";
        
        if(r[i] != 0) {
            cout << "SPILL";
        } else {
            cout << "Successful Allocation";
        }
    }
}