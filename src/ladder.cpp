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
    int len1 = str1.size(), len2 = str2.size();

    if (abs(len1 - len2) > d) 
        return false;

    int i = 0;
    int j = 0;
    int changes = 0;

    while (i < len1 && j < len2) {
        if (str1[i] != str2[j]) {
            if (++changes > d) 
                return false;

            if (len1 > len2) 
                ++i;     
            else if (len1 < len2) 
                ++j; 
            else { 
                ++i; 
                ++j; 
            }         
        } else {
            ++i; 
            ++j;
        }
    }

    return changes + (len1 - i) + (len2 - j) <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (word_list.find(end_word) == word_list.end()) {
        return{};
    }

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
                    
                    if (word == end_word)
                        return new_ladder;
                    
                    ladder_queue.push(new_ladder);
                    level_visited.insert(word);
                }
            }
        }
        
        for (const string& w : level_visited)
            visited.insert(w);
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
    set<string> word_list;

    load_words(word_list, "src/words.txt");

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);

    /*
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
    */
}