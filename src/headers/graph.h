#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstddef>
#include <vector>
#include <queue>
#include <stack>
#include <list>
#include <map>

using namespace std;

template <class T, class W> class Vertex;
template <class T, class W> class Edge;
template <class T, class W> class Graph;

/**
 * Represents an element of a graph.
 * It is composed by the information it store, 
 * a vector of edges and some fields necessary to search and traverse the graph.
*/
template <class T, class W>
class Vertex {
    T info;
    std::vector<Edge<T, W>> adj;

	vector<pair<Vertex<T, W>*, W>> lasts;
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
	//
	void clearLast();
	void addLast(Vertex<T, W> *v, W info);
	vector<pair<Vertex<T, W>*, W>> getLasts() const;
	//
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

/**
 * Represents a connection between two vertexes.
 * Store the weight of such connection 
 * and store a piece of information related to that connection.
*/
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
	W &getInfo();
	bool operator==(const Edge<T,W> cmp);
    friend class Graph<T, W>;
    friend class Vertex<T, W>;
};

/**
 * Represents a graph of elements.
 * Uses both a vector to enable simpler traversal 
 * and a map for a faster access to specific elements.
 * Both structures contain the same content.
*/
template <class T, class W>
class Graph {
    map<std::string, Vertex<T, W>*> vertexMap;
	vector<Vertex<T, W>*> vertexSet;
public:
    Vertex<T, W> *findVertex(std::string id);
    int getNumVertex() const;
    bool addVertex(const T &in, std::string id);
    bool removeVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, double w, W &info);
    bool removeEdge(const T &sourc, const T &dest);
    map<std::string, Vertex<T, W>*> getVertexMap() const;
	vector<Vertex<T, W>*> getVertexSet() const;
};

/** 
 * Constructor for an Edge
 * @param d Target Vertex
 * @param w Weight
 * @param info Information to be stored in the Edge	
*/
template <class T, class W>
Edge<T, W>::Edge(Vertex<T, W> *d, double w, W &info): dest(d), weight(w), info(info) {}

/** @return Target of a connection */
template<class T, class W>
Vertex<T, W> *Edge<T, W>::getDest() const {
    return dest;
}

/** 
 * Sets the target of a connection
 * @param d New vertex target 
*/
template<class T, class W>
void Edge<T, W>::setDest(Vertex<T, W> *d) {
    Edge::dest = d;
}

/** @return Weight of a connection */
template<class T, class W>
double Edge<T, W>::getWeight() const {
    return weight;
}

/** 
 * Sets the weight of a connection 
 * @param weight New weight value
*/
template<class T, class W>
void Edge<T, W>::setWeight(double weight) {
    Edge::weight = weight;
}

/** @return Information of the connection */
template<class T, class W>
W &Edge<T, W>::getInfo() {
	return info;
}

/** 
 * Equality operator for an Edge.
 * Two edges are equal if all their fields are equal.
 * @param cmp Edge to compare to
 * @return If edges are equal
*/
template<class T, class W>
bool Edge<T, W>::operator==(const Edge<T,W> cmp) {
	return dest == cmp.dest && weight == cmp.weight && info == cmp.info;
}

/** 
 *  Constructor for a Vertex 
 *  @param in Information to store in the vertex
*/
template<class T, class W>
Vertex<T, W>::Vertex(T in): info(in) {}

/** @return Information of the Vertex */
template<class T, class W>
T Vertex<T, W>::getInfo() const {
    return info;
}

/**
 * Sets the information on a vertex 
 * @param in New information to be stored
*/
template<class T, class W>
void Vertex<T, W>::setInfo(T in) {
    Vertex::info = in;
}

/**
 * Adds a new Pair to the lasts vector.
 * This is useful in Path Finding between Vertexes.
 * @param v Last vertex
 * @param info Connection information
*/
template<class T, class W>
void Vertex<T, W>::addLast(Vertex<T, W> *v, W info) {
	lasts.push_back(make_pair(v, info));
}

/** Clear the pairs stored inside the lasts vector. */
template<class T, class W>
void Vertex<T, W>::clearLast() {
	lasts.clear();
}

/** @return Lasts vector */
template<class T, class W>
std::vector<std::pair<Vertex<T, W> *, W>> Vertex<T, W>::getLasts() const {
	return lasts;
}

/** @return Processing tag */
template<class T, class W>
bool Vertex<T, W>::isProcessing() const {
    return processing;
}

/** Sets the Processing tag
 * @param p New value
 */
template<class T, class W>
void Vertex<T, W>::setProcessing(bool p) {
    Vertex::processing = p;
}

/** @return ~Visited tag */
template<class T, class W>
bool Vertex<T, W>::isVisited() const {
    return visited;
}

/** @return Indegree count */
template<class T, class W>
int Vertex<T, W>::getIndegree() const {
    return indegree;
}

/** Sets the Indegree value
 * @param indegree New value
 */
template<class T, class W>
void Vertex<T, W>::setIndegree(int indegree) {
    Vertex::indegree = indegree;
}

/** @return Num value */
template<class T, class W>
int Vertex<T, W>::getNum() const {
    return num;
}

/** Sets the Num value
 * @param num New value
 */
template<class T, class W>
void Vertex<T, W>::setNum(int num) {
    Vertex::num = num;
}

/** @return Low value */
template<class T, class W>
int Vertex<T, W>::getLow() const {
    return low;
}

/** Sets the Low value
 * @param low New value
 */
template<class T, class W>
void Vertex<T, W>::setLow(int low) {
    Vertex::low = low;
}

/** Sets the Visited tag
 * @param v New value
 */
template<class T, class W>
void Vertex<T, W>::setVisited(bool v) {
    Vertex::visited = v;
}

/** @return Vector of Connections/Edges */
template<class T, class W>
const std::vector<Edge<T, W>> &Vertex<T, W>::getAdj() const {
    return adj;
}

/** Sets the adjacent vector
 * @param adj New vector of Edges
 */
template<class T, class W>
void Vertex<T, W>::setAdj(const std::vector<Edge<T, W>> &adj) {
    Vertex::adj = adj;
}

/**
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template<class T, class W>
void Vertex<T, W>::addEdge(Vertex<T, W> *d, double w, W &info) {
    adj.push_back(Edge<T, W>(d, w, info));
}

/**
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

/** @return Number of Vertexes */
template<class T, class W>
int Graph<T, W>::getNumVertex() const {
    return vertexSet.size();
}

/** @return Map of String, Vertex*/
template<class T, class W>
map<std::string, Vertex<T, W>*> Graph<T, W>::getVertexMap() const {
    return vertexMap;
}

/** @return Vector of Vertexes */
template<class T, class W>
vector<Vertex<T, W>*> Graph<T, W>::getVertexSet() const {
    return vertexSet;
}

/**
 * Auxiliary function to find a vertex with a given associated tag.
 */
template<class T, class W>
Vertex<T, W> * Graph<T, W>::findVertex(std::string id) {
    try {
		return vertexMap.at(id);
	} catch (const exception &e) {
		return NULL;
	}
}

/**
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template<class T, class W>
bool Graph<T, W>::addVertex(const T &in, std::string id) {
    if (findVertex(id) != NULL)
		return false;
	
	auto newVtx = new Vertex<T, W>(in);
    vertexMap[id] = newVtx;
	vertexSet.push_back(newVtx);
    return true;
}


/**
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
    v1->addEdge(v2, w, info);
    return true;
}

/**
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

/**
 * Removes a vertex with a given content (in) from a graph (this), and
 * all outgoing and incoming edges.
 * Returns true if successful, and false if such vertex does not exist.
 */
template<class T, class W>
bool Graph<T, W>::removeVertex(const T &in) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++)
        if ((*it)->info == in) {
            auto v = *it;
            vertexSet.erase(it);
			vertexMap.erase(v.getInfo().getCode());
            for (auto u : vertexSet)
                u.second->removeEdgeTo(v);
            delete v;
            return true;
        }
    return false;
}

#endif /* GRAPH_H_ */
