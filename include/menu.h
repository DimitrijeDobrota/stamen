#ifndef STAMEN_MENU_H
#define STAMEN_MENU_H

#include "stamen.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace stamen {

class Menu {
    struct private_ctor_t {};

  public:
    // Tag type dispatch
    Menu(private_ctor_t, const std::string &code, const std::string &prompt)
        : Menu(code, prompt) {}

    Menu(const Menu &) = delete;
    Menu &operator=(const Menu &) = delete;
    Menu(Menu &&) = delete;
    Menu &operator=(Menu &&) = delete;
    ~Menu() noexcept = default;

    static int dynamic(const std::string &code, display_f display) {
        Menu::display_stub_default = code;
        Menu::display = display;
        return display_stub(-1);
    };

    static void read(const std::string &s);
    static void insert(const std::string &s, callback_f callback) {
        free_lookup.emplace(s, callback);
    }

    const std::string &getCode() const { return code; }
    const std::string &getTitle() const { return title; }

    const item_t *getItemv() const { return entries.items.data(); }
    std::size_t getSize() const { return entries.items.size(); }

    callback_f getCallback(std::size_t idx) const;
    const std::string &getCode(std::size_t idx) const;
    const std::string &getPrompt(std::size_t idx) const;

    static std::unordered_map<std::string, Menu> menu_lookup;

  private:
    Menu(std::string code, std::string prompt)
        : code(std::move(code)), title(std::move(prompt)) {}

    struct entries_t {
        entries_t() = default;
        entries_t(const entries_t &) = delete;
        entries_t &operator=(const entries_t &) = delete;
        entries_t(entries_t &&) = delete;
        entries_t &operator=(entries_t &&) = delete;
        ~entries_t() noexcept {
            for (const auto [_, prompt] : items) {
                delete[] prompt;
            }
        }

        void insert(const std::string &code, const std::string &prompt,
                    callback_f callback = display_stub);

        struct code_t {
            std::string code;
            std::string prompt;
        };

        std::vector<code_t> codes;
        std::vector<item_t> items;
    };

    const std::string code, title;
    entries_t entries;

    static int display_stub(int idx);

    static std::unordered_map<std::string, callback_f> free_lookup;
    static std::string display_stub_default;
    static display_f display;
};

} // namespace stamen

#endif
