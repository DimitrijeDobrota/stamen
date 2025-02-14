#pragma once

#include <functional>
#include <string>

namespace stamen {

using callback_f = std::function<int(size_t)>;

struct item_t
{
  callback_f callback;
  std::string prompt;
};

}  // namespace stamen
