#include "rowparser.h"

#include <iostream>

//C++ Implementations, if necessary
RSaxParser::RSaxParser(bool outMode) : xmlpp::SaxParser() {
//Initialize fields field?
    fullOutput = outMode;
}

RSaxParser::~RSaxParser() {}

std::ostream& operator<<(std::ostream& os, const RSaxParser& obj){
    if(obj.fullOutput){
        os << "Row[" << obj.datas[0] << "]: ";
        for(uint32_t i = 1; i < obj.datas.size()-1; i++){
            os << obj.datas[i] << " | ";
        }
        //workaround because operator << incessantly
        //converts &#10; into literal newlines
        os << obj.datas[obj.datas.size()-1];
    }
    else{
        os << "Row[" << obj.datas[0] << "]: " << obj.datas[1];
    }
    return os;
}

void RSaxParser::on_start_document() {}
void RSaxParser::on_end_document() {}
void RSaxParser::on_start_element(const Glib::ustring& name,
        const AttributeList& properties) {
    if(fullOutput || name == "subj" || name == "row"){
        fields.push_back(name);
    }
    currentTag = name;
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
void RSaxParser::on_end_element(const Glib::ustring& name) {
    if(fields.size() != datas.size()){
        datas.push_back("");
    }
    replaceAll(datas[datas.size()-1], "\n", "&#10;");
    replaceAll(datas[datas.size()-1], "'", "&apos;");
    replaceAll(datas[datas.size()-1], "\"", "&quot;");
}
void RSaxParser::on_characters(const Glib::ustring& text) {
    if(fullOutput || currentTag == "subj" || currentTag == "row"){
        if(fields.size() != datas.size()){
            datas.push_back(text);
        }else{
            datas[datas.size()-1].append(text);
        }
    }
}
void RSaxParser::on_comment(const Glib::ustring& text) {}
void RSaxParser::on_warning(const Glib::ustring& text) {}
void RSaxParser::on_error(const Glib::ustring& text) {
}
void RSaxParser::on_fatal_error(const Glib::ustring& text) {
}
