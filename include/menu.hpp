#ifndef STAMEN_MENU_HPP
#define STAMEN_MENU_HPP

#include "stamen.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace stamen {
namespace menu {

class menu_t;

extern std::unordered_map<std::string, callback_f> free_lookup;
extern std::unordered_map<std::string, menu_t> menu_lookup;
extern std::string display_stub_default;
extern display_f display;

void read(const char *filename);
void insert(const char *code, callback_f callback);
int dynamic(const char *code, display_f display);
int display_stub(int idx);

class menu_t {
    struct private_ctor_t {};
    friend void read(const char *filename);

  public:
    // Tag type dispatch
    menu_t(private_ctor_t, const std::string &code, const std::string &prompt)
        : menu_t(code, prompt) {}

    ~menu_t() noexcept {
        for (const auto [_, prompt] : items) {
            delete[] prompt;
        }
    }

    const std::string &getCode() const { return code; }
    const std::string &getTitle() const { return title; }

    const item_t *getItemv() const { return items.data(); }
    std::size_t getSize() const { return items.size(); }

    auto getCallback(std::size_t idx) const { return items[idx].callback; }
    const auto &getCode(std::size_t idx) const { return codes[idx].code; }
    const auto &getPrompt(std::size_t idx) const { return codes[idx].prompt; }

  private:
    menu_t(std::string code, std::string prompt)
        : code(std::move(code)), title(std::move(prompt)) {}

    menu_t(const menu_t &) = delete;
    menu_t &operator=(const menu_t &) = delete;
    menu_t(menu_t &&) = delete;
    menu_t &operator=(menu_t &&) = delete;

    void insert(const std::string &code, const std::string &prompt,
                callback_f callback = display_stub);

    struct code_t {
        std::string code;
        std::string prompt;
    };

    const std::string code, title;
    std::vector<code_t> codes;
    std::vector<item_t> items;
};

} // namespace menu
} // namespace stamen

#endif
