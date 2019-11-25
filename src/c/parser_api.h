#ifdef __cplusplus
extern "C" {
#endif

    struct RSaxParser* makeParser(bool outMode);
    void cleanParser(struct RSaxParser* parser);
    const char** parseRow(struct RSaxParser* parser, char* data, u_int32_t size, 
            u_int32_t* itemCount);

#ifdef __cplusplus
}
#endif

