#include "Prompt.hpp"

#include <memory>

int main(int argc, char **argv) {
  auto prompt = std::make_unique<Prompt>();
  return prompt->main();
}
