#include <libxml++/libxml++.h>

class RSaxParser : public xmlpp::SaxParser {
public:
    RSaxParser(bool outMode);
    ~RSaxParser() override;

    const char** sendData(u_int32_t* itemCount);

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
