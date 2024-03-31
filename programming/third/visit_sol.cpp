#include <cstdio>  
#include <vector>  
#include <unordered_map>  
#include <tuple>  
#include <algorithm>  
#include <limits>  
#include <queue>  
  
using namespace std;  
  
// Structure to represent a connection between cities
struct Connection {  
    int to;  // Destination city
    int time;  // Travel time
};  
  
// Maximum number of locations
const int MAX_LOCATIONS = 100001;  
vector<Connection> cityConnections[MAX_LOCATIONS];  // Vector of connections for each city
  
// Hash function for pairs (needed for unordered_map)
struct HashPair {  
    size_t operator()(const pair<int, int>& p) const {  
        return hash<int>()(p.first) ^ hash<int>()(p.second);  
    }  
};  
  
// Alias for unordered_map holding periods of non-operational paths
using PeriodsMap = unordered_map<pair<int, int>, pair<int, int>, HashPair>;  
  
// Function to build city connections from provided travel paths
void BuildCityConnections(const vector<tuple<int, int, int>>& travelPaths) {  
    for (const auto& path : travelPaths) {  
        int from = get<0>(path);  
        int to = get<1>(path);  
        int time = get<2>(path);  
        // Add connections in both directions
        cityConnections[from].push_back({to, time});  
        cityConnections[to].push_back({from, time});  
    }  
}  
  
// Function to calculate periods of non-operational paths based on the given route
PeriodsMap CalculateNonOperationalPeriods(const vector<int>& route, const vector<tuple<int, int, int>>& travelPaths) {  
    PeriodsMap nonOperationalPeriods;  // Map to store non-operational periods
    int currentTime = 0;  // Current time
  
    for (size_t i = 0; i < route.size() - 1; ++i) {  
        int city1 = route[i];  
        int city2 = route[i + 1];  
        int travelTime = -1;  
  
        // Find travel time between cities in provided travel paths
        for (const auto& path : travelPaths) {  
            int from = get<0>(path);  
            int to = get<1>(path);  
            int time = get<2>(path);  
            if ((from == city1 && to == city2) || (from == city2 && to == city1)) {  
                travelTime = time;  
                break;  
            }  
        }  
  
        if (travelTime != -1) {  
            // Update non-operational periods for the current path
            nonOperationalPeriods[{city1, city2}] = {currentTime, currentTime + travelTime - 1};  
            nonOperationalPeriods[{city2, city1}] = {currentTime, currentTime + travelTime - 1};  
            currentTime += travelTime;  // Update current time
        }  
    }  
  
    return nonOperationalPeriods;  // Return the map of non-operational periods
}  
  
// Function to find the optimal route given constraints
int FindOptimalRoute(int numLocations, int numPaths, int startLocation, int endLocation, int timeConstraint, const vector<int>& route, const vector<tuple<int, int, int>>& travelPaths) {  
    BuildCityConnections(travelPaths);  // Build city connections from provided travel paths
    PeriodsMap nonOperationalPeriods = CalculateNonOperationalPeriods(route, travelPaths);  // Calculate non-operational periods
  
    // Priority queue to explore routes
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> routeQueue;  
    vector<int> travelTime(numLocations + 1, numeric_limits<int>::max());  // Vector to store travel times
    routeQueue.push({timeConstraint, startLocation});  // Push initial time and location
    travelTime[startLocation] = timeConstraint;  // Set initial travel time
  
    while (!routeQueue.empty()) {  
        int currentTime = routeQueue.top().first;  // Current time
        int currentLocation = routeQueue.top().second;  // Current location
        routeQueue.pop();  
  
        if (currentLocation == endLocation) {  
            printf("%d\n", currentTime - timeConstraint);  // Print the total time taken
            return currentTime - timeConstraint;  // Return the total time taken
        }  
  
        for (const auto& neighbor : cityConnections[currentLocation]) {  
            int nextLocation = neighbor.to;  // Next location
            int pathTime = neighbor.time;  // Time to travel to next location
            int arrivalTime = currentTime + pathTime;  // Arrival time at next location
  
            auto periodInfoPair = nonOperationalPeriods.insert({{currentLocation, nextLocation}, {0, 0}});  
            auto& periodInfo = periodInfoPair.first;  
            int startNonOperational = periodInfo->second.first;  // Start of non-operational period
            int endNonOperational = periodInfo->second.second;  // End of non-operational period
  
            // Adjust arrival time considering non-operational periods
            if (currentTime < startNonOperational && arrivalTime > startNonOperational) {  
                arrivalTime = min(arrivalTime, endNonOperational + 1);  
            } else if (currentTime >= startNonOperational && currentTime <= endNonOperational) {  
                arrivalTime = endNonOperational + 1 + pathTime;  
            }  
  
            // Update travel time and push the next location to the queue
            if (arrivalTime < travelTime[nextLocation]) {  
                travelTime[nextLocation] = arrivalTime;  
                routeQueue.push({arrivalTime, nextLocation});  
            }  
        }  
    }  
  
    return -1;  // If no route found, return -1
}  
  
int main() {  
    int numLocations, numPaths, startLocation, endLocation, timeConstraint, numSelectedLocations;  
    scanf("%d %d %d %d %d %d", &numLocations, &numPaths, &startLocation, &endLocation, &timeConstraint, &numSelectedLocations);  
  
    vector<int> selectedRoute(numSelectedLocations);  
    for (int i = 0; i < numSelectedLocations; ++i) {  
        scanf("%d", &selectedRoute[i]);  
    }  
  
    vector<tuple<int, int, int>> travelPaths(numPaths);  
    for (int i = 0; i < numPaths; ++i) {  
        int from, to, time;  
        scanf("%d %d %d", &from, &to, &time);  
        travelPaths[i] = make_tuple(from, to, time);  
    }  
  
    FindOptimalRoute(numLocations, numPaths, startLocation, endLocation, timeConstraint, selectedRoute, travelPaths);  
  
    return 0;  
} 
