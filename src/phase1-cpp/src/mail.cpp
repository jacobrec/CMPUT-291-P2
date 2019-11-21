#include <iostream>
#include <string>
#include <ctype.h>
#include <vector>

#include "mail.hpp"

using namespace std;

void getEmails(string emailLine, vector<string>& out) {
    static string delimiter = ",";
    size_t last = 0;
    size_t next = 0;
    while ((next = emailLine.find(delimiter, last)) != string::npos) {
        out.push_back(emailLine.substr(last, next-last));
        last = next + 1;
    }
    out.push_back(emailLine.substr(last));
}

void getTerms(string terms, vector<string>& out) {
    size_t start = 0;
    size_t end = 0;

    char c;
#define PUSHIF \
    if (end > start + 2) { \
        out.push_back(terms.substr(start, end-start)); \
    }

    for (size_t i = 0; i < terms.size(); i++) {
        c = terms[i];
        if (('a' <= c && 'z' >= c) || ('A' <= c && 'Z' >= c) ||
                ('0' <= c && '9' >= c) || c == '_' || c == '-') {
            terms[i] = tolower(c);
            end++;
        } else {
            PUSHIF;
            end++;
            start = end;
        }
    }

    PUSHIF;
    #undef PUSHIF
}

void writeEmails(string line, int row,
        string type, ostream* out) {
    vector<string> emails;
    getEmails(line, emails);
    for (string s : emails) {
        *out << type << "-" << s << ":" << row <<  '\n';
    }
}

void writeTerms(std::string line, int row,
        std::string type, std::ostream* out) {
    vector<string> terms;
    getTerms(line, terms);
    for (string s : terms) {
        *out << type << "-" << s << ":" << row << '\n';
    }
}

void writeDate(std::string line, int row, std::ostream* out) {
    *out << line << ":" << row << '\n';
}

