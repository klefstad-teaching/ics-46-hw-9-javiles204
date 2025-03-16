#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    int len1 = str1.size();
    int len2 = str2.size();

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

    return changes + abs(len1 - i) + abs(len2 - j) <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word)
        return {begin_word};

    if (word_list.find(end_word) == word_list.end()) {
        return {};
    }

    queue<vector<string>> q;
    q.push({begin_word});
    set<string> visited;
    visited.insert(begin_word);

    while (!q.empty()) {
        int level_size = q.size();
        set<string> words_used_in_level;

        while (level_size--) {
            vector<string> ladder = q.front();
            string last_word = ladder.back();
            q.pop();

            for (const string& word : word_list) {
                if (visited.count(word)) continue;

                if (is_adjacent(last_word, word)) {
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);

                    if (word == end_word) 
                        return new_ladder;

                    q.push(new_ladder);
                    words_used_in_level.insert(word);
                }
            }
        }

        for (const string& w : words_used_in_level)
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
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}