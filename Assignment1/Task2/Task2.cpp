#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void checkWords(const vector<string>& states, const vector<string>& alphabet, const vector<vector<int>>& transitionMatrix,
                const string& startState, const vector<string>& acceptingStates, const vector<string>& words) {
    string result;
    for (const string& word : words) {
        int currentStateIndex = find(states.begin(), states.end(), startState) - states.begin();
        bool isRejected = false;
        for (char symbol : word) {
            if (symbol == '_') {
                continue;
            }
            auto symbolIt = find(alphabet.begin(), alphabet.end(), string(1, symbol));
            if (symbolIt == alphabet.end()) {
                isRejected = true;
                break;
            }
            int symbolIndex = symbolIt - alphabet.begin();
            int newStateIndex = transitionMatrix[currentStateIndex][symbolIndex];
            if (newStateIndex == -1) {
                isRejected = true;
                break;
            }
            currentStateIndex = newStateIndex;
        }
        if (isRejected || find(acceptingStates.begin(), acceptingStates.end(), states[currentStateIndex]) == acceptingStates.end()) {
            result.append("R").append(" ");
        } else {
            result.append("A").append(" ");
        }
    }
    cout << result << endl;
}

int main() {
    int Q, S, F, N;
    cin >> Q >> S >> F >> N;
    cin.ignore();

    vector<string> states(Q);
    for (int i = 0; i < Q; ++i) {
        cin >> states[i];
    }

    vector<string> alphabet(S);
    for (int i = 0; i < S; ++i) {
        cin >> alphabet[i];
    }

    vector<vector<int>> transitionMatrix(Q, vector<int>(S, -1));

    for (int i = 0; i <= Q * S; ++i) {
        string line;
        getline(cin, line);
        if (line.empty()) {
            continue;
        }
        size_t pos = line.find(' ');
        if (pos == string::npos) {
            continue;
        }
        string currentState = line.substr(0, pos);
        line.erase(0, pos + 1);
        pos = line.find(' ');
        if (pos == string::npos) {
            continue;
        }
        string inputSymbol = line.substr(0, pos);
        string newState = line.substr(pos + 1);

        int stateIndex = find(states.begin(), states.end(), currentState) - states.begin();
        int inputIndex = find(alphabet.begin(), alphabet.end(), inputSymbol) - alphabet.begin();
        int newStateIndex = find(states.begin(), states.end(), newState) - states.begin();

        transitionMatrix[stateIndex][inputIndex] = newStateIndex;
    }

    string startState;
    cin >> startState;

    vector<string> acceptingStates(F);
    for (int i = 0; i < F; ++i) {
        cin >> acceptingStates[i];
    }

    vector<string> words(N);
    for (int i = 0; i < N; ++i) {
        cin >> words[i];
    }

    checkWords(states, alphabet, transitionMatrix, startState, acceptingStates, words);

    return 0;
}
