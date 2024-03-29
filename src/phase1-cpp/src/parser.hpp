#ifndef PARSER_H_
#define PARSER_H_

#include <libxml++/libxml++.h>

class JSaxParser : public xmlpp::SaxParser {
public:
  JSaxParser();
  ~JSaxParser() override;

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


#endif
