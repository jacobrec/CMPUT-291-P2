#include <cstring> // std::memset()
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "parser.hpp"

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

  auto return_code = EXIT_SUCCESS;

  try {

    char buffer[1024 * 1024]; // 1MB buffer
    const size_t buffer_size = sizeof(buffer) / sizeof(char);

    JSaxParser parser;
    parser.set_substitute_entities(true);
    do {
      std::memset(buffer, 0, buffer_size);
      std::cin.read(buffer, buffer_size - 1);
      if (std::cin.gcount()) {
        Glib::ustring input(buffer, buffer + std::cin.gcount());
        parser.parse_chunk(input);
      }
    } while (std::cin);

    parser.finish_chunk_parsing();
  } catch (const xmlpp::exception &ex) {
    std::cerr << "Incremental parsing, libxml++ exception: " << ex.what()
              << std::endl;
    return_code = EXIT_FAILURE;
  }

  return return_code;
}

