#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <stdexcept>
#include <set>
#include <algorithm>

using namespace std;

struct triple {
    string from, to, transition;

    triple(string _from, string _to, string s_transition)
        : from(std::move(_from)), to(std::move(_to)), transition(std::move(s_transition)) {}

    bool operator<(const triple &rhs) const {
        if (from != rhs.from)
            return from < rhs.from;
        if (to != rhs.to)
            return to < rhs.to;
        return transition < rhs.transition;
    }

    bool operator==(const triple &rhs) const {
        return from == rhs.from && to == rhs.to && transition == rhs.transition;
    }
};

struct tensor {
    vector<vector<vector<string>>> matrix;
    explicit tensor(size_t size)
        : matrix(vector<vector<vector<string>>>(size, vector<vector<string>>(size, vector<string>(size, "")))) {}
};

vector<string> handle(string);
vector<triple> handleTransitions(string);
void dfs(vector<string> &, vector<triple> &);
void dfsVisit(vector<bool> &, const string &, vector<string> &, vector<triple> &);
bool part_of(vector<string> &, const string &);

int main() {
    vector<string> acceptingVec, initialVec, statesVec, alphaVec;
    vector<triple> transVec;
    string s_type, s_states, s_alpha, s_initial, s_accepting, s_trans, buffer, answer;
    size_t initial_index;
    ifstream input("input.txt");
    getline(input, s_type);
    getline(input, s_states);
    getline(input, s_alpha);
    getline(input, s_initial);
    getline(input, s_accepting);
    getline(input, s_trans);

    acceptingVec = handle(s_accepting);
    initialVec = handle(s_initial);
    statesVec = handle(s_states);
    alphaVec = handle(s_alpha);
    transVec = handleTransitions(s_trans);
    sort(acceptingVec.begin(), acceptingVec.end());

    set<string> acceptingSet(acceptingVec.begin(), acceptingVec.end());
    set<string> initialSet(initialVec.begin(), initialVec.end());
    set<string> statesSet(statesVec.begin(), statesVec.end());
    set<string> alphaSet(alphaVec.begin(), alphaVec.end());
    set<triple> transSet(transVec.begin(), transVec.end());
    vector<string> accepting(acceptingSet.begin(), acceptingSet.end());
    vector<string> initial(initialSet.begin(), initialSet.end());
    vector<string> states(statesSet.begin(), statesSet.end());
    vector<string> alpha(alphaSet.begin(), alphaSet.end());
    vector<triple> trans(transSet.begin(), transSet.end());

    tensor regexp(states.size());
    vector<vector<string>> regexp_initial(states.size(), vector<string>(states.size(), ""));

    try {
        if (s_accepting.find("accepting=[") == string::npos) {
            throw logic_error("E1");
        }
        if (getline(input, buffer))
            throw logic_error("E1");
        if (states.empty())
            throw logic_error("E1");
        if (alpha.empty()) {
            throw logic_error("E1");
        }
        if (s_alpha.find(",]") != string::npos) {
            throw logic_error("E1");
        }
        for (int i = 0;i<trans.size();i++){
            for (int j = i + 1;j<trans.size();j++){
                if (trans[i].to == trans[j].to && trans[i].from == trans[j].from && trans[i].transition == trans[j].transition){
                    throw logic_error("E1");
                }
            }
        }
        if (initial.empty())
            throw logic_error("E2");
        if (accepting.empty()) {
            throw logic_error("E3");
        }
        for (const string &s: initialVec) {
            if (!part_of(states, s)) {
                throw logic_error("E4" + s);
            }
        }
        for (auto &tran: trans) {
            if (!part_of(states, tran.from) || !part_of(states, tran.to))
                throw logic_error("E4" + (part_of(states, tran.from) ? tran.to : tran.from));
        }
        for (const triple &t: transVec) {
            if (t.transition.empty()) {
                throw logic_error("E1");
            }
            if (!part_of(alphaVec, t.transition)) {
                throw logic_error("E5" + t.transition);
            }
        }
        try {
            dfs(states, transVec);
        }
        catch (logic_error &error) {
            throw;
        }
        for (size_t i = 0; i < transVec.size(); ++i) {
            bool flag = false;
            if (s_type == "type=[deterministic]") {
                for (size_t j = i + 1; j < transVec.size(); ++j) {
                    if (transVec[i].from == transVec[j].from && transVec[i].transition == transVec[j].transition) {
                        throw logic_error("E7");
                    }
                }
            }
            for (const auto &j: alphaVec) {
                if (transVec[i].transition == j)
                    flag = true;
            }
            if (!flag)
                throw logic_error("E5");
        }

        for (auto &i: transVec) {
            size_t from, to;
            for (size_t j = 0; j < states.size(); ++j) {
                if (states[j] == i.from) from = j;
                if (states[j] == i.to) to = j;
            }
            if (!regexp_initial[from][to].empty())
                regexp_initial[from][to] += "|" + i.transition;
            else
                regexp_initial[from][to] = i.transition;
        }

        for (size_t i = 0; i < states.size(); ++i) {
            if (regexp_initial[i][i].empty())
                regexp_initial[i][i] = "eps";
            else
                regexp_initial[i][i] += "|eps";

            for (size_t j = 0; j < states.size(); ++j) {
                if (i != j && regexp_initial[i][j].empty())
                    regexp_initial[i][j] = "{}";
            }
        }
        for (size_t k = 0; k < states.size(); ++k) {
            for (size_t i = 0; i < states.size(); ++i) {
                for (size_t j = 0; j < states.size(); ++j) {
                    if (k > 0)
                        regexp.matrix[i][j][k] = "(" + regexp.matrix[i][k][k - 1] + ")(" + regexp.matrix[k][k][k - 1] + ")*(" + regexp.matrix[k][j][k - 1] + ")|(" + regexp.matrix[i][j][k - 1] + ")";
                    else
                        regexp.matrix[i][j][k] = "(" + regexp_initial[i][k] + ")(" + regexp_initial[k][k] + ")*(" + regexp_initial[k][j] + ")|(" + regexp_initial[i][j] + ")";
                }
            }
        }
        for (size_t i = 0; i < states.size(); ++i) {
            if (states[i] == initialVec[0]) {
                initial_index = i;
                break;
            }
        }
        for (const auto &i: accepting) {
            size_t index;

            for (size_t j = 0; j < states.size(); ++j) {
                if (states[j] == i)
                    index = j;
            }

            if (answer.empty())
                answer = "(" + regexp.matrix[initial_index][index][states.size() - 1] + ")";
            else
                answer += "|(" + regexp.matrix[initial_index][index][states.size() - 1] + ")";
        }

        if (answer.empty())
            answer = "{}";

        cout << answer << endl;
    }
    catch (const logic_error &error) {
        string exception_message(error.what()), error_code(exception_message.substr(0, 2)), argument;
        if (error_code == "E1") {
            cout << "E1: Input file is malformed" << endl;
        } else if (error_code == "E2") {
            cout << "E2: Initial state is not defined" << endl;
        } else if (error_code == "E3") {
            cout << "E3: Set of accepting states is empty" << endl;
        } else if (error_code == "E4") {
            argument = exception_message.substr(2);
            cout << "E4: A state '" << argument << "' is not in the set of states" << endl;
        } else if (error_code == "E5") {
            argument = exception_message.substr(2);
            cout << "E5: A transition '" << argument << "' is not represented in the alphabet" << endl;
        } else if (error_code == "E6") {
            cout << "E6: Some states are disjoint" << endl;
        } else if (error_code == "E7") {
            cout << "E7: FSA is non-deterministic" << endl;
        }
    }

    input.close();
}

vector<string> handle(string str) {
    vector<string> processed;
    string buffer;
    bool inside = false;

    for (size_t i = 0; i < str.size() - 1; ++i) {
        if (inside) {
            if (str[i] != ',') {
                buffer.push_back(str[i]);
            } else {
                if (!buffer.empty() && find(processed.begin(), processed.end(), buffer) == processed.end()) {
                    processed.push_back(buffer);
                }
                buffer.clear();
            }
        }
        if (str[i] == '[')
            inside = true;
    }
    if (!buffer.empty() && find(processed.begin(), processed.end(), buffer) == processed.end()) {
        processed.push_back(buffer);
    }

    return processed;
}


vector<triple> handleTransitions(string str) {
    vector<string> trans_split = handle(std::move(str));
    vector<triple> processed;
    string from, to, weight, buffer;
    bool from_word = true, second_word = false;
    for (const string &transition: trans_split) {
        for (char i: transition) {
            if (i == '>' && from_word && !second_word) {
                from = buffer;
                from_word = false;
                buffer.clear();
                continue;
            } else if (i == '>' && !from_word && !second_word) {
                weight = buffer;
                second_word = true;
                buffer.clear();
                continue;
            }
            buffer.push_back(i);
        }
        to = buffer;
        from_word = true;
        second_word = false;
        buffer.clear();
        processed.emplace_back(from, to, weight);
    }
    return processed;
}

void dfs(vector<string> &states, vector<triple> &trans) {
    vector<bool> visited(states.size(), false);

    visited[0] = true;
    dfsVisit(visited, states[0], states, trans);

    for (size_t i = 0; i < states.size(); ++i) {
        if (!visited[i])
            throw logic_error("E6");
    }
}

void dfsVisit(vector<bool> &visited, const string &state, vector<string> &states, vector<triple> &trans) {
    for (size_t i = 0; i < trans.size(); ++i) {
        if (trans[i].from == state) {
            for (size_t j = 0; j < states.size(); ++j) {
                if (states[j] == trans[i].to && !visited[j]) {
                    visited[j] = true;
                    dfsVisit(visited, states[j], states, trans);
                }
            }
        }
    }
}

bool part_of(vector<string> &set, const string &element) {
    return find(set.begin(), set.end(), element) != set.end();
}

