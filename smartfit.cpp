// SmartFit core in C++: health calculators + meal planner (0/1 knapsack vs greedy)
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

struct Food { string name; int kcal; int protein; };

vector<Food> foods = {
    {"Dal (1 bowl)",180,9},{"Paneer (100 g)",265,18},{"Chicken breast (100 g)",165,31},
    {"Boiled egg",78,6},{"Roti",100,3},{"Rice (1 cup)",205,4},{"Curd (1 cup)",150,8},
    {"Soya chunks (50 g)",170,26},{"Chana (1 cup)",270,15},{"Milk (1 glass)",150,8},
    {"Peanuts (30 g)",170,7},{"Oats (40 g)",150,5},{"Whey (1 scoop)",120,24}};

// 0/1 knapsack: maximize protein within calorie budget. Time O(n*W).
void knapsack(int budget) {
    int n = foods.size(), step = 10, W = budget / step;
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; i++) {
        int c = (foods[i-1].kcal + step - 1) / step, v = foods[i-1].protein;
        for (int j = 0; j <= W; j++) {
            dp[i][j] = dp[i-1][j];
            if (j >= c && dp[i-1][j-c] + v > dp[i][j]) dp[i][j] = dp[i-1][j-c] + v;
        }
    }
    cout << "\nKnapsack (optimal):\n";
    int j = W, kc = 0;
    for (int i = n; i > 0; i--)
        if (dp[i][j] != dp[i-1][j]) {
            cout << "  " << foods[i-1].name << " - " << foods[i-1].kcal << " kcal, " << foods[i-1].protein << " g\n";
            kc += foods[i-1].kcal; j -= (foods[i-1].kcal + step - 1) / step;
        }
    cout << "  Total: " << kc << " kcal, " << dp[n][W] << " g protein\n";
}

// Greedy: best protein per kcal first. Time O(n log n), not always optimal.
void greedy(int budget) {
    vector<Food> f = foods;
    sort(f.begin(), f.end(), [](const Food& a, const Food& b){ return (double)a.protein/a.kcal > (double)b.protein/b.kcal; });
    cout << "\nGreedy (ratio):\n";
    int kc = 0, p = 0;
    for (auto& x : f) if (kc + x.kcal <= budget) {
        cout << "  " << x.name << " - " << x.kcal << " kcal, " << x.protein << " g\n";
        kc += x.kcal; p += x.protein;
    }
    cout << "  Total: " << kc << " kcal, " << p << " g protein\n";
}

int main() {
    char g; int age, act, goal; double h, w;
    cout << "Gender (M/F): "; cin >> g;
    cout << "Age: "; cin >> age;
    cout << "Height (cm): "; cin >> h;
    cout << "Weight (kg): "; cin >> w;
    cout << "Activity (1 sedentary, 2 light, 3 moderate, 4 very active): "; cin >> act;
    cout << "Goal (1 maintain, 2 fat loss, 3 muscle gain, 4 bulking): "; cin >> goal;

    bool male = (g == 'M' || g == 'm');
    double mult[] = {1.2, 1.375, 1.55, 1.725};
    int adj[] = {0, -500, 250, 500};
    double k[] = {1.2, 1.6, 1.6, 1.4};

    double bmi = w / ((h / 100) * (h / 100));
    double fat = 1.2 * bmi + 0.23 * age - 10.8 * (male ? 1 : 0) - 5.4;
    double bmr = 10 * w + 6.25 * h - 5 * age + (male ? 5 : -161);
    int cal = (int)round(bmr * mult[act-1] + adj[goal-1]);
    int pro = (int)round(w * k[goal-1]);
    const char* cat = bmi < 18.5 ? "Underweight" : bmi < 25 ? "Normal" : bmi < 30 ? "Overweight" : "Obese";

    cout << "\nBMI: " << bmi << " (" << cat << ")\nBody fat: " << fat << " %\n"
         << "Calories: " << cal << " kcal\nProtein: " << pro << " g\nWater: " << w * 0.035 << " L\n";

    int budget = (cal / 3 / 10) * 10;  // one meal = a third of the day
    cout << "\nMeal budget: " << budget << " kcal\n";
    knapsack(budget);
    greedy(budget);
    return 0;
}
