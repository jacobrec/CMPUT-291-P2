#include <stdbool.h>

#include "rowparser.h"
#include "parser_api.h"

#ifdef __cplusplus
extern "C" {
#endif

    //C API Implementation
    struct RSaxParser* makeParser(bool outMode){
        RSaxParser* p = new RSaxParser(outMode);
        p->set_substitute_entities(true);
        return p;
    }
    void cleanParser(struct RSaxParser* parser){ delete parser; }

    void parseRow(struct RSaxParser* parser, char* data, size_t size){
        parser->parse_chunk(Glib::ustring(data, size));
        parser->finish_chunk_parsing();
        std::cout << *parser << '\n';
    }

#ifdef __cplusplus
}
#endif
