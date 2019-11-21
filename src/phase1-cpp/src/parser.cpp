#include "parser.hpp"
#include "mail.hpp"
#include <glibmm/convert.h> //For Glib::ConvertError

#include <iostream>
#include <fstream>
#include <stack>

std::stack<std::string> xmlstack;
int row;
std::ostream* emails = &std::cout;
std::ostream* dates = &std::cout;
std::ostream* terms = &std::cout;
std::ostream* recs = &std::cout;

std::string node;

JSaxParser::JSaxParser() : xmlpp::SaxParser() {
    emails = new std::ofstream("emails.txt");
    dates = new std::ofstream("dates.txt");
    terms = new std::ofstream("terms.txt");
    recs = new std::ofstream("recs.txt");
}

JSaxParser::~JSaxParser() {
    delete emails;
    delete dates;
    delete terms;
    delete recs;
}

void JSaxParser::on_start_document() { }

void JSaxParser::on_end_document() { }

void JSaxParser::on_start_element(const Glib::ustring &name,
        const AttributeList &attributes) {
    xmlstack.push(name);
    if (name != "emails") {
        node.append("<");
        node.append(name);
        node.append(">");
    }
}

void replaceAll(std::string& str, const std::string& from,
        const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}


void JSaxParser::on_end_element(const Glib::ustring &name) {
    if (name != "emails") {
        node.append("</");
        node.append(name);
        node.append(">");
    }
    if (name == "mail") {
        while('<' != node[0]) { node.erase(0, 1); }
        replaceAll(node, "\n", "&#10;");
        replaceAll(node, "'", "&apos;");
        replaceAll(node, "\"", "&quot;");
        *recs << row << ":" << node << '\n';
        node = "";
    }
    xmlstack.pop();
}

void JSaxParser::on_characters(const Glib::ustring &text) {
    std::string x = text;
    replaceAll(x, "&", "&amp;");
    replaceAll(x, "<", "&lt;");
    replaceAll(x, ">", "&gt;");
    node.append(x);
    if (xmlstack.top() == "row") {
        row = std::stoi(text);
    } else if(xmlstack.top() == "to") {
        writeEmails(text, row, "to", emails);
    } else if(xmlstack.top() == "from") {
        writeEmails(text, row, "from", emails);
    } else if(xmlstack.top() == "cc") {
        writeEmails(text, row, "cc", emails);
    } else if(xmlstack.top() == "bcc") {
        writeEmails(text, row, "bcc", emails);
    } else if(xmlstack.top() == "date") {
        writeDate(text, row, dates);
    } else if(xmlstack.top() == "subj") {
        writeTerms(text, row, "s", terms);
    } else if(xmlstack.top() == "body") {
        writeTerms(text, row, "b", terms);
    }
}

void JSaxParser::on_comment(const Glib::ustring &text) { }

void JSaxParser::on_warning(const Glib::ustring &text) { }

void JSaxParser::on_error(const Glib::ustring &text) {
    try {
        std::cout << "on_error(): " << text << std::endl;
    } catch (const Glib::ConvertError &ex) {
        std::cerr << "JSaxParser::on_error(): Exception caught while converting "
            "text for std::cout: "
            << ex.what() << std::endl;
    }
}

void JSaxParser::on_fatal_error(const Glib::ustring &text) {
    try {
        std::cout << "on_fatal_error(): " << text << std::endl;
    } catch (const Glib::ConvertError &ex) {
        std::cerr << "JSaxParser::on_characters(): Exception caught while "
            "converting value for std::cout: "
            << ex.what() << std::endl;
    }
}

