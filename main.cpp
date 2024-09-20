#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

/* 
• Size of the input (num cards in price list for problem)
• Computed max profit, in int dollars
• Num cards purchased to achieve the max profit
• Time (in seconds) to solve the problem.
2 2750 2 0.000027
JimRice1975
*/

// Get maximum profit
void findMaxProfit(vector<string>& names, vector<int>& marketValues, vector<int>& prices, 
                   int W, int index, int currentProfit, int currentWeight, vector<string>& currentSet, 
                   int& maxProfit, vector<string>& bestSet) {
    // Check if this set is better
    if (index == names.size()) {
        if (currentWeight <= W && currentProfit > maxProfit) {
            maxProfit = currentProfit;
            bestSet = currentSet;
        }
        return;
    }

    // Discard curr item
    findMaxProfit(names, marketValues, prices, W, index + 1, currentProfit, currentWeight, currentSet, 
                  maxProfit, bestSet);

    // Keep curr item
    currentSet.push_back(names[index]);
    findMaxProfit(names, marketValues, prices, W, index + 1, currentProfit + (marketValues[index] - prices[index]), 
                  currentWeight + prices[index], currentSet, maxProfit, bestSet);
    currentSet.pop_back(); // Backtrack
}

// Helper function
vector<string> ComputeMaxProfit(vector<string>& names, vector<int>& marketValues, vector<int>& prices, int W) {
    vector<string> bestSet;
    vector<string> currentSet;
    int maxProfit = 0;

    findMaxProfit(names, marketValues, prices, W, 0, 0, 0, currentSet, maxProfit, bestSet);
    return bestSet;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "Error:./program1 <market-price-file> <price-list-file> <output-file>" << endl;
        return 1;
    }

    ifstream marketFile(argv[1]);
    ifstream priceFile(argv[2]);
    ofstream outputFile(argv[3]);

    // Reading the market price file
    vector<string> marketNames;
    vector<int> marketValues;
    int numMarketCards;
    marketFile >> numMarketCards;
    for (int i = 0; i < numMarketCards; i++) {
        string cardName;
        int marketValue;
        marketFile >> cardName >> marketValue;
        marketNames.push_back(cardName);
        marketValues.push_back(marketValue);
    }

    int num, weight;
    while (priceFile >> num >> weight) {
        vector<string> problemNames;
        vector<int> problemPrices;
        vector<int> problemMarketValues;
        bool valid = true;

        for (int i = 0; i < num; i++) {
            bool found = false;
            string cardName;
            int price = 0;
            priceFile >> cardName >> price;
            for (int j = 0; j < marketNames.size(); j++) {
                if (marketNames[j] == cardName) {
                    problemNames.push_back(cardName);
                    problemPrices.push_back(price);
                    problemMarketValues.push_back(marketValues[j]);
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Error: Card " << cardName << " not found in market price file" << endl;
                valid = false;
                break;
            }
        }

        if (!valid) {
            continue;
        }

        // Timing
        clock_t startTime = clock();
        vector<string> solution = ComputeMaxProfit(problemNames, problemMarketValues, problemPrices, weight);

        // Calc total profit
        int totalProfit = 0;
        for (int i = 0; i < solution.size(); i++) {
            for (int j = 0; j < problemNames.size(); j++) {
                if (solution[i] == problemNames[j]) {
                    totalProfit += problemMarketValues[j] - problemPrices[j];
                    break;
                }
            }
        }
        float elapsedTime = float(clock() - startTime) / CLOCKS_PER_SEC;

        // Ouput
        outputFile << num << " " << totalProfit << " " << solution.size() << " " << elapsedTime << endl;
        for (int i = 0; i < solution.size(); i++) {
            outputFile << solution[i] << endl;
        }
    }

    return 0;
}