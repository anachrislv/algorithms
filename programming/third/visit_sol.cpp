#include <cstdio>  
#include <vector>  
#include <unordered_map>  
#include <tuple>  
#include <algorithm>  
#include <limits>  
#include <queue>  
  
using namespace std;  
  
struct Connection {  
    int to;  
    int time;  
};  
  
const int MAX_LOCATIONS = 100001;  
vector<Connection> cityConnections[MAX_LOCATIONS];  
  
struct HashPair {  
    size_t operator()(const pair<int, int>& p) const {  
        return hash<int>()(p.first) ^ hash<int>()(p.second);  
    }  
};  
  
using PeriodsMap = unordered_map<pair<int, int>, pair<int, int>, HashPair>;  
  
void BuildCityConnections(const vector<tuple<int, int, int>>& travelPaths) {  
    for (const auto& path : travelPaths) {  
        int from = get<0>(path);  
        int to = get<1>(path);  
        int time = get<2>(path);  
        cityConnections[from].push_back({to, time});  
        cityConnections[to].push_back({from, time});  
    }  
}  
  
PeriodsMap CalculateNonOperationalPeriods(const vector<int>& route, const vector<tuple<int, int, int>>& travelPaths) {  
    PeriodsMap nonOperationalPeriods;  
    int currentTime = 0;  
  
    for (size_t i = 0; i < route.size() - 1; ++i) {  
        int city1 = route[i];  
        int city2 = route[i + 1];  
        int travelTime = -1;  
  
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
            nonOperationalPeriods[{city1, city2}] = {currentTime, currentTime + travelTime - 1};  
            nonOperationalPeriods[{city2, city1}] = {currentTime, currentTime + travelTime - 1};  
            currentTime += travelTime;  
        }  
    }  
  
    return nonOperationalPeriods;  
}  
  
int FindOptimalRoute(int numLocations, int numPaths, int startLocation, int endLocation, int timeConstraint, const vector<int>& route, const vector<tuple<int, int, int>>& travelPaths) {  
    BuildCityConnections(travelPaths);  
    PeriodsMap nonOperationalPeriods = CalculateNonOperationalPeriods(route, travelPaths);  
  
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> routeQueue;  
    vector<int> travelTime(numLocations + 1, numeric_limits<int>::max());  
    routeQueue.push({timeConstraint, startLocation});  
    travelTime[startLocation] = timeConstraint;  
  
    while (!routeQueue.empty()) {  
        int currentTime = routeQueue.top().first;  
        int currentLocation = routeQueue.top().second;  
        routeQueue.pop();  
  
        if (currentLocation == endLocation) {  
            printf("%d\n", currentTime - timeConstraint);  
            return currentTime - timeConstraint;  
        }  
  
        for (const auto& neighbor : cityConnections[currentLocation]) {  
            int nextLocation = neighbor.to;  
            int pathTime = neighbor.time;  
            int arrivalTime = currentTime + pathTime;  
  
            auto periodInfoPair = nonOperationalPeriods.insert({{currentLocation, nextLocation}, {0, 0}});  
            auto& periodInfo = periodInfoPair.first;  
            int startNonOperational = periodInfo->second.first;  
            int endNonOperational = periodInfo->second.second;  
  
            if (currentTime < startNonOperational && arrivalTime > startNonOperational) {  
                arrivalTime = min(arrivalTime, endNonOperational + 1);  
            } else if (currentTime >= startNonOperational && currentTime <= endNonOperational) {  
                arrivalTime = endNonOperational + 1 + pathTime;  
            }  
  
            if (arrivalTime < travelTime[nextLocation]) {  
                travelTime[nextLocation] = arrivalTime;  
                routeQueue.push({arrivalTime, nextLocation});  
            }  
        }  
    }  
  
    return -1;  
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
