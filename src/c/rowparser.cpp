#include "rowparser.h"

#include <vector>
#include <iostream>

//C++ Implementations, if necessary
std::vector<std::string> fields;
std::vector<std::string> datas;
bool fullOutput;

RSaxParser::RSaxParser(bool outMode) : xmlpp::SaxParser() {
//Initialize fields field?
    fullOutput = outMode;
}

RSaxParser::~RSaxParser() {}

const char** RSaxParser::sendData(u_int32_t* itemCount){
    *itemCount = datas.size();
    const char** all = new const char*[*itemCount];
    for(uint32_t i = 0; i < *itemCount; i++){
        all[i] = datas[i].c_str();
    }
    return all;
}

void RSaxParser::on_start_document() {}
void RSaxParser::on_end_document() {}
void RSaxParser::on_start_element(const Glib::ustring& name,
        const AttributeList& properties) {
    std::cout << name << "\n";
    if(fullOutput || name == "subj" || name == "row"){
        fields.push_back(name);
    }
}
void RSaxParser::on_end_element(const Glib::ustring& name) {
    if(fields.size() != datas.size()){
        datas.push_back("");
    }
}
void RSaxParser::on_characters(const Glib::ustring& text) {
    if(fields.size() != datas.size()){
        datas.push_back(text);
    }else{
        datas[datas.size()-1].append(text);
    }
}
void RSaxParser::on_comment(const Glib::ustring& text) {}
void RSaxParser::on_warning(const Glib::ustring& text) {}
void RSaxParser::on_error(const Glib::ustring& text) {
}
void RSaxParser::on_fatal_error(const Glib::ustring& text) {
}
