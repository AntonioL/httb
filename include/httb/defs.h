/*!
 * httb.
 * defs.h
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef HTTB_DEFS_H
#define HTTB_DEFS_H

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

namespace httb {


/// \brief Simple std::pair<std::string, std::string>
using kv = std::pair<std::string, std::string>;

/// \brief Simple vector of pairs wss::web::KeyValue
using kv_vector = std::vector<httb::kv>;

inline bool case_insensitive_equal(const std::string &str1, const std::string &str2) noexcept {
    return str1.size() == str2.size() &&
        std::equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
          return tolower(a) == tolower(b);
        });
}
class CaseInsensitiveEqual {
public:
    bool operator()(const std::string &str1, const std::string &str2) const noexcept {
        return case_insensitive_equal(str1, str2);
    }
};
// Based on https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x/2595226#2595226
class CaseInsensitiveHash {
public:
    std::size_t operator()(const std::string &str) const noexcept {
        std::size_t h = 0;
        std::hash<int> hash;
        for (auto c : str)
            h ^= hash(tolower(c)) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

using CaseInsensitiveMultimap = std::unordered_multimap<std::string,
                                                        std::string,
                                                        CaseInsensitiveHash,
                                                        CaseInsensitiveEqual>;

using CaseInsensitiveMap = std::unordered_map<std::string,
                                              std::string,
                                              CaseInsensitiveHash,
                                              CaseInsensitiveEqual>;

}

#endif //HTTB_DEFS_H