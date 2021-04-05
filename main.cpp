#include <iostream>
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <cmath>

struct Node;
struct Edge;

struct Node{
    char id;
    bool visited = false;
    std::list<Edge *> edges;
};

struct Edge{
    int weight;
    Node *src, *dst;
};

struct Graph{
    std::list<Node *> nodes;
    std::list<Node *>::iterator utility(const char& src);
    bool inList(const char& label);
    bool haveEdge(const char& src, const char& dst);
    int edgeWeight(const char& src, const char& dst);
    void clear();
    void addNode(const char& label);
    void insertEdge(const char& src, const char& dst, const int& weight);
    void printNodes() const;
    void printEdges() const;
    void printEdges(const char& src);
    void deleteNode(const char& src);
    void deleteEdge(const char& src, const char& dst);
    void bfs(const char& src);
    void dfs(const char& src);
    void degree(const char& src);
    void jaccard(const char& param1, const char& param2);
    void cosine(const char& param1, const char& param2);
    void simrank(const char& param1, const char& param2);
};

std::list<Node *>::iterator Graph::utility(const char& src) {
    auto it = nodes.begin();
    for(auto i : nodes){
        if(i->id < src)
            ++it;
    }
    return it;
};

bool Graph::inList(const char &label) {
    for(auto i : nodes){
        if(i->id == label)
            return true;
    }
    return false;
}

bool Graph::haveEdge(const char &src, const char &dst) {
    if(!inList(src) || !inList(dst)){
        std::cout << "Restricted operation: \"Invalid Nodes!\"\n";
        return false;
    }
    for(auto i : (*utility(src))->edges)
        if(i->src->id == src && i->dst->id == dst)
            return true;
    return false;
}

int Graph::edgeWeight(const char &src, const char &dst) {
    for(auto i : (*utility(src))->edges){
        if(i->dst == *utility(dst))
            return i->weight;
    }
    return 0;
}

void Graph::clear() {
    for(auto i : nodes)
        i->visited = false;
}

void Graph::addNode(const char& label) {
    if(label > 90 || label < 65){
        std::cout << "Can not create node (" << label << "): "
        << "\"Node labels can only be capital characters!\"\n";
        return;
    }
    if(nodes.size() >= 25){
        std::cout << "Maximum amount of nodes!" << std::endl;
        return;
    }
    if(inList(label)){
        std::cout << "Node " << label << " already exists!\n";
        return;
    }
    if(nodes.empty()){
        Node* n = new Node;
        n->id = label;
        nodes.push_back(n);
        return;
    }
    else{
        Node* n = new Node;
        n->id = label;
        nodes.insert(utility(label),n);
    }
}

//The parameters will be converted to iterators.
//Then edge will be added.
void Graph::insertEdge(const char& src, const char& dst, const int& weight = 1) {
    if(src == dst){
        std::cout << "Can not create an edge from (" << src << ") to (" << dst
        <<"): \"Loops are not allowed!\"" << std::endl;
        return;
    }
    //*/
    if(nodes.size() < 2){
        std::cout << "Need 2 nodes to insert edge!" << std::endl;
        return;
    }
    if(!inList(src) || !inList(dst)){
        std::cout << "Can not create an edge from (" << src << ") to (" << dst
                  <<"): \"Nodes are not valid!\"" << std::endl;
        return;
    }
    if(weight < 1){
        //weight negatif olmasın.
        std::cout << "Restricted operation: \"Weight of an edge can not be less than 1!\"\n";
        return;
    }
    if(haveEdge(src,dst)){
        std::cout << "Restricted operation: \"This edge exists!\"\n";
        return;
    }
    Edge *e = new Edge;
    auto it = utility(dst);
    e->dst = *it;
    it = utility(src);
    e->src = *it;
    e->weight = weight;
    (*it)->edges.push_back(e);
}

void Graph::printNodes() const {
    std::cout << "List of all nodes:\n";
    for (auto i : nodes)
        if(i != *(--nodes.end()))
            std::cout << "(" << i->id << "), ";
        else
            std::cout << "(" << i->id << ")\n";
}

void Graph::printEdges() const {
    std::cout << "List of all edges:\n";
    for(auto i : nodes){
        for(auto j : i->edges){
            if(j != *(--(i->edges.end())))
                std::cout << "(" << j->src->id << "->" << j->dst->id << "," << j->weight << "), ";
            else
                std::cout << "(" << j->src->id << "->" << j->dst->id << "," << j->weight << ")\n";
        }
    }
}

void Graph::printEdges(const char &src) {
    std::cout << "Edge list of " << src << " node:\n";
    for(auto i : (*utility(src))->edges)
        if(i != *--((*utility(src))->edges).end())
            std::cout << "(" << i->src->id << "->" << i->dst->id << "," << i->weight << "), ";
        else
            std::cout << "(" << i->src->id << "->" << i->dst->id << "," << i->weight << ")\n";
}

void Graph::deleteNode(const char& src) {
    if(!inList(src)){
        std::cout << "Restricted operation: \"No node called (" << src << ")!\"\n";
        return;
    }
    for(auto i : nodes){
        auto it = i->edges.begin();
        for(auto j : i->edges)
            if(j->dst->id == src){
                i->edges.erase(it);
            }
            else it++;
    }
    nodes.erase(utility(src));
}

void Graph::deleteEdge(const char& src, const char &dst) {
    if(!inList(src) || !inList(dst)){
        std::cout << "Restricted operation: \"Usage of nonexistent node!\"\n";
        return;
    }

    auto it = (*utility(src))->edges.begin();
    bool flag = true;
    for(auto i : (*utility(src))->edges){
        if(i->dst->id == dst){
            (*utility(src))->edges.erase(it);
            flag = false;
        }
        it++;
    }
    if(flag)
        std::cout << "There is no edge between these nodes!\n";
}

void Graph::bfs(const char &src) {
    if(!inList(src)){
        std::cout << "Restricted operation: \"Node does not exist!\"";
        return;
    }
    std::queue<Node *> myQueue;
    std::list<char>  traverse;
    myQueue.push(*utility(src));
    int level = 0;
    while(!myQueue.empty()){
        if(myQueue.front()->visited){
            myQueue.pop();
            continue;
        }
        for(auto i : myQueue.front()->edges){
            myQueue.push(i->dst);
        }
        if(!myQueue.front()->visited){
            traverse.push_back(myQueue.front()->id);
            myQueue.front()->visited = true;
        }
        myQueue.pop();
    }
    for (auto i : traverse){
        if(i != *(--traverse.end())){
            std::cout << i << "->";
        }
        else std::cout << i << std::endl;
    }
    clear();
}

void Graph::dfs(const char &src) {
    if(!inList(src)){
        std::cout << "Restricted operation: \"Node does not exist!\"";
        return;
    }
    std::stack<Node *> myStack;
    std::list<char>  traverse;
    myStack.push(*utility(src));
    (*utility(src))->visited = true;
    while(!myStack.empty()){
        Node *v = myStack.top();
        traverse.push_back(v->id);
        myStack.pop();
        for(auto i : v->edges){
            if(!i->dst->visited){
                myStack.push(i->dst);
                i->dst->visited = true;
            }
        }
    }
    for (auto i : traverse){
        if(i != *(--traverse.end())){
            std::cout << i << "->";
        }
        else std::cout << i << std::endl;
    }
    clear();
}

//Depth first search recursive version
/*void Graph::dfs(const char &src) {
    (*utility(src))->visited = true;
    for(auto i : (*utility(src))->edges){
        if(!i->dst->visited){
            dfs(i->dst->id);
        }
    }
}*/

void Graph::degree(const char &src) {
    if(!inList(src)){
        std::cout << "Restricted operation: \"Node does not exist!\"\n";
        return;
    }
    int inDegree = 0, outDegree = 0;
    for(auto i : nodes)
        for(auto j : i->edges)
            if(j->dst->id == src)
                inDegree++;
    outDegree = (*utility(src))->edges.size();
    std::cout << "In degree of " << src << " = " << inDegree << std::endl;
    std::cout << "Out degree of " << src << " = " << outDegree << std::endl;
}

void Graph::jaccard(const char &param1, const char &param2) {
    if(!inList(param1) || !inList(param2)){
        std::cout << "Restricted operation: \"Node does not exist!\"\n";
        return;
    }
    double result = 0;
    std::set<char> mySet;
    for(auto i : (*utility(param1))->edges){
        mySet.insert(i->dst->id);
        for(auto j : (*utility(param2))->edges){
            mySet.insert(j->dst->id);
            if(i->dst->id == j->dst->id)
                result++;
        }
    }
    result /= mySet.size();
    std::cout << "Jaccard similarity of " << param1 << " and " << param2;
    std::cout << " " << result << '\n';
}

void Graph::cosine(const char &param1, const char &param2) {
    if(!inList(param1) || !inList(param2)){
        std::cout << "Restricted operation: \"Node does not exist!\"\n";
        return;
    }
    double res = 0, v1 = 0, v2 = 0;
    std::list<Node *> n;
    std::vector<int> p1, p2;
    for(auto i : nodes){
        if(i->id == param1 || i->id == param2)
            continue;
        n.push_back(i);
    }
    for(auto i : n){
        if(haveEdge(param1,i->id)){
            p1.push_back(edgeWeight(param1,i->id));
        }
        else p1.push_back(0);

        if(haveEdge(param2,i->id)){
            p2.push_back(edgeWeight(param2,i->id));
        }
        else p2.push_back(0);
    }
    for(auto i : p1){
        v1 += (i*i);
    }
    for(auto i : p2){
        v2 += (i*i);
    }
    for(int i = 0; i < p1.size(); i++){
        res += (p1[i] * p2[i]);
    }
    std::cout.precision(4);
    std::cout << "Cosine similarity of " <<
    param1 << " and " << param2 << ": " << res / (sqrt(v1) * sqrt(v2)) << '\n';
}

void Graph::simrank(const char &param1, const char &param2) {
    if(!inList(param1) || !inList(param2)){
        std::cout << "Restricted operation: \"Node does not exist!\"\n";
        return;
    }
    auto sr = [] (const char &p1, const char &p2){
        if(p1 == p2) return 1;
        else return 0;
    };
    std::vector<char> p1, p2;
    double res = 0;
    for(auto i : (*utility(param1))->edges){
        p1.push_back(i->dst->id);
    }
    for(auto i : (*utility(param2))->edges){
        p2.push_back(i->dst->id);
    }
    for(auto i : p1){
        for(auto j : p2){
            res += sr(i,j);
        }
    }
    res /= p1.size() * p2.size();
    std::cout << "Simrank(" << param1 << "," << param2
    << ") = c * " << res;
}

int main() {
    Graph newGraph;
    newGraph.addNode('A');
    newGraph.addNode('B');
    newGraph.addNode('W');
    newGraph.addNode('X');
    newGraph.addNode('Y');
    newGraph.addNode('Z');
    newGraph.addNode('D');
    newGraph.addNode('F');
    newGraph.insertEdge('A','W',2);
    newGraph.insertEdge('A','Z',1);
    newGraph.insertEdge('A','F',1);
    newGraph.insertEdge('A','Y', 4);
    newGraph.insertEdge('B','W',3);
    newGraph.insertEdge('B','X',1);
    newGraph.insertEdge('B','Z',2);
    newGraph.jaccard('A','B');
    newGraph.cosine('A','B');
    newGraph.simrank('A','B');
    return 0;
}