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

    const char** parseRow(struct RSaxParser* parser, char* data, u_int32_t size,
            u_int32_t* itemCount){
        parser->parse_chunk(Glib::ustring(data, size));
        parser->finish_chunk_parsing();
        return parser->sendData(itemCount);
    }

#ifdef __cplusplus
}
#endif
