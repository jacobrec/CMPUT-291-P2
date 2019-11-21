#include <cstring> // std::memset()
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "parser.hpp"

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

  std::string filepath;
  if (argc > 1)
    filepath = argv[1];
  else
    filepath = "xml/10.xml";

  auto return_code = EXIT_SUCCESS;
  try {
    JSaxParser parser;
    parser.set_substitute_entities(true);
    parser.parse_file(filepath);
  } catch (const xmlpp::exception &ex) {
    std::cerr << "libxml++ exception: " << ex.what() << std::endl;
    return_code = EXIT_FAILURE;
  }

  try {
    std::ifstream is(filepath.c_str());
    if (!is)
      throw xmlpp::exception("Could not open file " + filepath);

    char buffer[1024 * 1024]; // 1MB buffer
    const size_t buffer_size = sizeof(buffer) / sizeof(char);

    JSaxParser parser;
    parser.set_substitute_entities(true);
    do {
      std::memset(buffer, 0, buffer_size);
      is.read(buffer, buffer_size - 1);
      if (is.gcount()) {
        Glib::ustring input(buffer, buffer + is.gcount());
        parser.parse_chunk(input);
      }
    } while (is);

    parser.finish_chunk_parsing();
  } catch (const xmlpp::exception &ex) {
    std::cerr << "Incremental parsing, libxml++ exception: " << ex.what()
              << std::endl;
    return_code = EXIT_FAILURE;
  }

  return return_code;
}

