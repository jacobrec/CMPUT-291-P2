#include <assert.h>
#include <stdio.h>
#include "parser_api.h"

char* single_record = "<mail><row>5</row><date>2000/10/05</date><from>phillip.allen@enron.com</from><to>david.delainey@enron.com</to><subj></subj><cc></cc><bcc></bcc><body>Dave, &#10;&#10; Here are the names of the west desk members by category. The origination &#10;side is very sparse. &#10;&#10;&#10;&#10;&#10;&#10;Phillip&#10;</body></mail>";
size_t length = 321;

void test_api() {
    struct RSaxParser* parser = makeParser(true);
    parseRow(parser, single_record, length);
    cleanParser(parser);

    parser = makeParser(false);
    parseRow(parser, single_record, length);
    cleanParser(parser);

    fprintf(stderr, "Tested XML Parser C API\n");
}
