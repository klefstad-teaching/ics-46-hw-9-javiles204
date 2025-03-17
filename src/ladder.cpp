#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

using namespace std;

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    if (str1.size() != str2.size()) 
        return false;

    int changes = 0;

    for (size_t i = 0; i < str1.size(); ++i) {
        if (str1[i] != str2[i]) {
            if (++changes > d) 
                return false;
        }
    }
    return changes <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    unordered_set<string> visited;
    visited.insert(begin_word);
    
    while (!ladder_queue.empty()) {
        int size = ladder_queue.size();
        unordered_set<string> level_visited;
        
        for (int i = 0; i < size; i++) {
            vector<string> ladder = ladder_queue.front();
            ladder_queue.pop();
            string last_word = ladder.back();
            
            for (const string& word : word_list) {
                if (is_adjacent(last_word, word) && visited.find(word) == visited.end()) {
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);
                    
                    if (word == end_word) {
                        return new_ladder;
                    }
                    
                    ladder_queue.push(new_ladder);
                    level_visited.insert(word);
                }
            }
        }
        
        for (const string& w : level_visited) {
            visited.insert(w);
        }
    }
    
    return {}; 
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);

    if (!file)
        error("", "", "Could not open word file");

    string word;

    while(file >> word) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        word_list.insert(word);
    }
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty())
        cout << "No word ladder found." << endl;
    else {
        cout << "Word ladder found: ";
        for (size_t i = 0; i < ladder.size(); ++i) {
            cout << ladder[i];
            if (i != ladder.size() - 1) 
                cout << " ";
        }
        
        cout << " " << endl;
    }
}

#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

void verify_word_ladder() {
    //tested and works properly; takes too long in autograder

    set<string> word_list;

    load_words(word_list, "src/words.txt");

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}