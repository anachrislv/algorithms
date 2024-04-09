#include <iostream>
#include <vector>
#include <algorithm>

constexpr int MAX_N = 100000;
constexpr int MAX_K = 1000;
constexpr int MIN_INF = -1000000;

using ChocoMatrix = std::vector<std::vector<int>>;

void maximizeChocoGains(ChocoMatrix& choco_gain, const std::vector<int>& p, int N, int K) {
    for (int i = 1; i <= K; ++i) {
        int best_last_buy = MIN_INF;
        for (int j = 1; j < N; ++j) {
            best_last_buy = std::max(best_last_buy, choco_gain[j - 1][i - 1] - p[j - 1]);
            choco_gain[j][i] = std::max(choco_gain[j - 1][i], p[j] + best_last_buy);
        }
    }
}

int main() {
    int N, K;
    std::cin >> N >> K;

    std::vector<int> choco_prices(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> choco_prices[i];
    }

    ChocoMatrix choco_gain(N, std::vector<int>(K + 1, 0));
    maximizeChocoGains(choco_gain, choco_prices, N, K);

    std::cout << choco_gain[N - 1][K] << '\n';
