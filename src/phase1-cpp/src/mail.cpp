#include <iostream>
#include <string>
#include <ctype.h>
#include <vector>

#include "mail.hpp"

using namespace std;

void getEmails(string emailLine, ostream* out, string type, int row) {
    static string delimiter = ",";
    size_t last = 0;
    size_t next = 0;
    while ((next = emailLine.find(delimiter, last)) != string::npos) {
        *out << type << "-" << emailLine.substr(last, next-last) << ":" << row <<  '\n';
        last = next + 1;
    }
    *out << type << "-" << emailLine.substr(last) << ":" << row <<  '\n';
}

void getTerms(string terms, ostream* out, int row, string type) {
    size_t start = 0;
    size_t end = 0;

    char c;
#define PUSHIF \
    if (end > start + 2) { \
        *out << type << "-" << \
            terms.substr(start, end-start) << ":" << row << '\n'; \
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
    getEmails(line, out, type, row);
}

void writeTerms(std::string line, int row,
        std::string type, std::ostream* out) {
    getTerms(line, out, row, type);

}

void writeDate(std::string line, int row, std::ostream* out) {
    *out << line << ":" << row << '\n';
}

