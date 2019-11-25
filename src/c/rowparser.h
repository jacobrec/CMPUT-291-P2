#include <libxml++/libxml++.h>
#include <vector>
#include <iostream>

class RSaxParser : public xmlpp::SaxParser {
public:
    std::vector<std::string> fields;
    std::vector<std::string> datas;
    bool fullOutput;
    std::string currentTag;

    RSaxParser(bool outMode);
    ~RSaxParser() override;

protected:
    //overrides:
    void on_start_document() override;
    void on_end_document() override;
    void on_start_element(const Glib::ustring& name,
                                  const AttributeList& properties) override;
    void on_end_element(const Glib::ustring& name) override;
    void on_characters(const Glib::ustring& characters) override;
    void on_comment(const Glib::ustring& text) override;
    void on_warning(const Glib::ustring& text) override;
    void on_error(const Glib::ustring& text) override;
    void on_fatal_error(const Glib::ustring& text) override;
};
std::ostream& operator<<(std::ostream& os, const RSaxParser& obj);
