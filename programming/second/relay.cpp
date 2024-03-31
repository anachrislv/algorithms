#include <vector>
#include <iostream>

struct Line { // Represents a line in the convex hull trick
    Line& assign(const Line &other) { intercept = other.intercept; slope = other.slope; return *this; }
    long long evaluate(const long long x) const { return intercept - slope * x; } // Evaluates the line at given x
    long long intercept, slope; // y = intercept - slope * x
};

long double calculateIntersectionX(const Line &line1, const Line &line2) {
    // Calculates the intersection point of two lines
    return static_cast<long double>((line1.intercept - line2.intercept) / (line1.slope - line2.slope));
}

class ConvexHullTrick {
public:
    ConvexHullTrick() : lines() {}

    ConvexHullTrick& assign(const ConvexHullTrick &other) { lines = other.lines; return *this; }

    // Adds a line to the convex hull trick
    void add(const long long intercept, const long long slope) {
        int index = lines.size();
        const Line newLine = { intercept, slope };
        // Remove lines that become redundant after adding new line
        while (index-- > 1 && calculateIntersectionX(newLine, lines[index]) <= calculateIntersectionX(lines[index], lines[index - 1]))
            lines.pop_back();
        lines.push_back(newLine);
    }

    // Finds the minimum value at given x using the convex hull trick
    long long findMinimum(const long long x) const {
        const int numLines = lines.size();
        if (numLines < 2) return lines[0].evaluate(x); // If only one line exists
        // Binary search to find the correct line for given x
        int low = 0, high = numLines - 1;
        long double intersection1, intersection2;
        while (low <= high) {
            int mid = (low + high) / 2;
            intersection1 = intersection2 = x;
            if (mid > 0) intersection1 = calculateIntersectionX(lines[mid], lines[mid - 1]);
            if (mid < numLines - 1) intersection2 = calculateIntersectionX(lines[mid], lines[mid + 1]);

            if (intersection1 <= x && x <= intersection2) return lines[mid].evaluate(x);
            if (x < intersection1) high = mid - 1;
            else low = mid + 1;
        }
        return -1; // Should not reach here
    }

private:
    std::vector<Line> lines; // Stores lines in the convex hull trick
};

struct Node {
    Node(const long long vertex, const int distance, const Node* nextNode): vertex(vertex), distance(distance), nextNode(nextNode) {}
    long long vertex; // Represents a vertex
    int distance; // Distance to the next node
    const Node *nextNode; // Pointer to the next node
};

long long   point[100005], speed[100005], dynamicProgram[100005]; // 10^5 + 5
const Node* edges[100005];
bool explored[100005];

// Returns the number of unexplored children of a given vertex
int getNumberOfUnexploredChildren(const long long vertex) {
    int count = 0;
    const Node *node  = edges[vertex];
    while(node != nullptr) {
        if(!explored[node->vertex]) ++count;
        node = node->nextNode;
    }
    return count;
}

// Performs Depth First Search starting from vertex v
void DepthFirstSearch(const long long vertex, const long long totalDistance, const int convexHullIndex) {
    explored[vertex] = true;

    if (vertex != 1) {
        // Update the dynamic program value for the vertex
        dynamicProgram[vertex] = point[vertex] + speed[vertex] * totalDistance + convexHullTricks[convexHullIndex].findMinimum(speed[vertex]);
        convexHullTricks[convexHullIndex].add(dynamicProgram[vertex], totalDistance);
    }

    int unexploredChildren = getNumberOfUnexploredChildren(vertex);
    const Node *node  = edges[vertex];
    while(node != nullptr) {
        if(!explored[node->vertex]) {
            // Copy the convex hull trick only if there are more than one unexplored children
            int newHullIndex = convexHullIndex;
            if (--unexploredChildren) newHullIndex = node->vertex;
            DepthFirstSearch(node->vertex, totalDistance + node->distance, newHullIndex);
        }
        node = node->nextNode;
    }
}

int main() {
    int numVertices, vertex_i, vertex_j, distance_ij;
    std::cin >> numVertices;
    for (int i = 1; i < numVertices; ++i) {
        std::cin >> vertex_i >> vertex_j >> distance_ij;
        edges[vertex_i] = new Node(vertex_j, distance_ij, edges[vertex_i]);
        edges[vertex_j] = new Node(vertex_i, distance_ij, edges[vertex_j]);
    }

    for (int i = 2; i <= numVertices; ++i)
        std::cin >> point[i] >> speed[i];

    convexHullTricks[1].add(0, 0);
    DepthFirstSearch(1, 0, 1);

    for (int i = 2; i < numVertices; ++i)
        std::cout << dynamicProgram[i] << ' ';
    std::cout << dynamicProgram[numVertices] << std::endl;

    return 0;
}
