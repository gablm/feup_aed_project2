#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstddef>
#include <vector>
#include <queue>
#include <stack>
#include <list>

using namespace std;

template <class T, class W> class Vertex;
template <class T, class W> class Edge;
template <class T, class W> class Graph;

template <class T, class W>
class Vertex {
    T info;
    std::vector<Edge<T, W>> adj;
    bool visited;
    bool processing;
    int indegree;
    int num;
    int low;

    void addEdge(Vertex<T, W> *dest, double w, W &info);
    bool removeEdgeTo(Vertex<T, W> *d);
public:
    Vertex(T in);
    T getInfo() const;
    void setInfo(T in);
    bool isVisited() const;
    void setVisited(bool v);
    bool isProcessing() const;
    void setProcessing(bool p);
    const std::vector<Edge<T, W>> &getAdj() const;
    void setAdj(const std::vector<Edge<T, W>> &adj);
    int getIndegree() const;
    void setIndegree(int indegree);
    int getNum() const;
    void setNum(int num);
    int getLow() const;
    void setLow(int low);
    friend class Graph<T, W>;
};

template <class T, class W>
class Edge {
    Vertex<T, W>* dest;
    double weight;
	W info;
public:
    Edge(Vertex<T, W> *d, double w, W &info);
    Vertex<T, W> *getDest() const;
    void setDest(Vertex<T, W> *dest);
    double getWeight() const;
    void setWeight(double weight);
	W getInfo() const;
    friend class Graph<T, W>;
    friend class Vertex<T, W>;
};

template <class T, class W>
class Graph {
    vector<Vertex<T, W>*> vertexSet;
    int _index_;
    stack<Vertex<T, W>> _stack_;
    list<list<T>> _list_sccs_;

    void dfsVisit(Vertex<T, W> *v,  vector<T> & res) const;
    bool dfsIsDAG(Vertex<T, W> *v) const;
public:
    Vertex<T, W> *findVertex(const T &in) const;
    int getNumVertex() const;
    bool addVertex(const T &in);
    bool removeVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, double w, W &info);
    bool removeEdge(const T &sourc, const T &dest);
    vector<Vertex<T, W> * > getVertexSet() const;
    vector<T> dfs() const;
    vector<T> dfs(const T & source) const;
    vector<T> bfs(const T &source) const;
    bool isDAG() const;
};

template <class T, class W>
Edge<T, W>::Edge(Vertex<T, W> *d, double w, W &info): dest(d), weight(w), info(info) {}

template<class T, class W>
Vertex<T, W> *Edge<T, W>::getDest() const {
    return dest;
}

template<class T, class W>
void Edge<T, W>::setDest(Vertex<T, W> *d) {
    Edge::dest = d;
}

template<class T, class W>
double Edge<T, W>::getWeight() const {
    return weight;
}

template<class T, class W>
void Edge<T, W>::setWeight(double weight) {
    Edge::weight = weight;
}

template<class T, class W>
W Edge<T, W>::getInfo() const {
	return info;
}

template<class T, class W>
Vertex<T, W>::Vertex(T in): info(in) {}

template<class T, class W>
T Vertex<T, W>::getInfo() const {
    return info;
}

template<class T, class W>
void Vertex<T, W>::setInfo(T in) {
    Vertex::info = in;
}

template<class T, class W>
bool Vertex<T, W>::isProcessing() const {
    return processing;
}

template<class T, class W>
void Vertex<T, W>::setProcessing(bool p) {
    Vertex::processing = p;
}

template<class T, class W>
bool Vertex<T, W>::isVisited() const {
    return visited;
}

template<class T, class W>
int Vertex<T, W>::getIndegree() const {
    return indegree;
}

template<class T, class W>
void Vertex<T, W>::setIndegree(int indegree) {
    Vertex::indegree = indegree;
}

template<class T, class W>
int Vertex<T, W>::getNum() const {
    return num;
}

template<class T, class W>
void Vertex<T, W>::setNum(int num) {
    Vertex::num = num;
}

template<class T, class W>
int Vertex<T, W>::getLow() const {
    return low;
}

template<class T, class W>
void Vertex<T, W>::setLow(int low) {
    Vertex::low = low;
}

template<class T, class W>
void Vertex<T, W>::setVisited(bool v) {
    Vertex::visited = v;
}

template<class T, class W>
const std::vector<Edge<T, W>> &Vertex<T, W>::getAdj() const {
    return adj;
}

template<class T, class W>
void Vertex<T, W>::setAdj(const std::vector<Edge<T, W>> &adj) {
    Vertex::adj = adj;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T, class W>
void Vertex<T, W>::addEdge(Vertex<T, W> *d, double w, W &info) {
    adj.push_back(Edge<T, W>(d, w, info));
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T, class W>
bool Vertex<T, W>::removeEdgeTo(Vertex<T, W> *d) {
    for (auto it = adj.begin(); it != adj.end(); it++)
        if (it->dest  == d) {
            adj.erase(it);
            return true;
        }
    return false;
}

template<class T, class W>
int Graph<T, W>::getNumVertex() const {
    return vertexSet.size();
}

template<class T, class W>
vector<Vertex<T, W>*> Graph<T, W>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template<class T, class W>
Vertex<T, W> * Graph<T, W>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class T, class W>
bool Graph<T, W>::addVertex(const T &in) {
    if ( findVertex(in) != NULL)
        return false;
    vertexSet.push_back(new Vertex<T, W>(in));
    return true;
}


/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template<class T, class W>
bool Graph<T, W>::addEdge(const T &sourc, const T &dest, double w, W &info) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2,w, info);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template<class T, class W>
bool Graph<T, W>::removeEdge(const T &sourc, const T &dest) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    return v1->removeEdgeTo(v2);
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template<class T, class W>
bool Graph<T, W>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++)
        if ((*it)->info  == in) {
            auto v = *it;
            vertexSet.erase(it);
            for (auto u : vertexSet)
                u->removeEdgeTo(v);
            delete v;
            return true;
        }
    return false;
}


/****************** DFS ********************/
/*
 * Performs a depth-first search (dfs) traversal in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template<class T, class W>
vector<T> Graph<T, W>::dfs() const {
    vector<T> res;
    for (auto v : vertexSet)
        v->visited = false;
    for (auto v : vertexSet)
        if (! v->visited)
            dfsVisit(v, res);
    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template<class T, class W>
void Graph<T, W>::dfsVisit(Vertex<T, W> *v, vector<T> & res) const {
    v->visited = true;
    res.push_back(v->info);
    for (auto & e : v->adj) {
        auto w = e.dest;
        if ( ! w->visited)
            dfsVisit(w, res);
    }
}


/****************** DFS ********************/
/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order,
 * from the source node.
 */
template<class T, class W>
vector<T> Graph<T, W>::dfs(const T & source) const {
    vector<T> res;
    auto s = findVertex(source);
    if (s == nullptr)
        return res;

    for (auto v : vertexSet)
        v->visited = false;

    dfsVisit(s, res);
    return res;
}


/****************** BFS ********************/
/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by bfs order.
 */
template<class T, class W>
vector<T> Graph<T, W>::bfs(const T & source) const {
    vector<T> res;
    auto s = findVertex(source);
    if (s == NULL)
        return res;
    queue<Vertex<T, W> *> q;
    for (auto v : vertexSet)
        v->visited = false;
    q.push(s);
    s->visited = true;
    while (!q.empty()) {
        auto v = q.front();
        q.pop();
        res.push_back(v->info);
        for (auto & e : v->adj) {
            auto w = e.dest;
            if ( ! w->visited ) {
                q.push(w);
                w->visited = true;
            }
        }
    }
    return res;
}


/****************** isDAG  ********************/
/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */

template<class T, class W>
bool Graph<T, W>::isDAG() const {
    for (auto v : vertexSet) {
        v->visited = false;
        v->processing = false;
    }
    for (auto v : vertexSet)
        if (! v->visited)
            if ( ! dfsIsDAG(v) )
                return false;
    return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template<class T, class W>
bool Graph<T, W>::dfsIsDAG(Vertex<T, W> *v) const {
    v->visited = true;
    v->processing = true;
    for (auto & e : v->adj) {
        auto w = e.dest;
        if (w->processing)
            return false;
        if (! w->visited)
            if (! dfsIsDAG(w))
                return false;
    }
    v->processing = false;
    return true;
}

#endif /* GRAPH_H_ */
