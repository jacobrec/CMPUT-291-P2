#ifdef __cplusplus
extern "C" {
#endif

    #include <stdbool.h>

    struct RSaxParser* makeParser(bool outMode);
    void cleanParser(struct RSaxParser* parser);
    void parseRow(struct RSaxParser* parser, char* data, size_t size);

#ifdef __cplusplus
}
#endif

