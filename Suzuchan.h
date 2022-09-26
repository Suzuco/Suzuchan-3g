//
// Created by suzuco on 2/2/22.
//

#ifndef MIRAICP_SUZUCHAN_H
#define MIRAICP_SUZUCHAN_H

#include <string>
#include <regex>
#include <MiraiCP.hpp>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

using namespace MiraiCP;

typedef unsigned long long ull;


typedef struct dice_param {
    int count;
    int faces;
    int offset;
} dice_param;

const std::regex at(R"(\[mirai:at:(\d{6,11})\])");
const std::regex dice_notation(R"(.*?(\d+)d(\d+)([+-]\d+)?.*)");

std::string eval(const std::string & expr_str);

class Suzuchan final {
private:
    template <typename T> static T urandom();
    static ull generate_gacha_qword();
    static std::vector<std::string> vectorize(const std::string & s);
    static std::string person_swap(const std::string & s, const GroupMessageEvent & e);
    static dice_param parse_dice(const std::string & s);
    static void _initialize_pyenv();

    static inline std::string make_at(QQID id, bool regex = false)
    {
        if (regex)
            return "\\[mirai:at:" + std::to_string(id) + "\\] ?";
        else
            return "[mirai:at:" + std::to_string(id) + "]";
    }

public:
    static void process(MiraiCP::GroupMessageEvent e);
    static void fortune(GroupMessageEvent e);
    static void randsel(GroupMessageEvent e);
    static void randcat(GroupMessageEvent e);
    static void gacha(GroupMessageEvent e, bool smoke);
    static void smoke(GroupMessageEvent e, const std::string & verb);
    static void evaluate(GroupMessageEvent e);
    static void shuffle(GroupMessageEvent e, bool concatenate = false);
    static void go_jubeat(GroupMessageEvent e);
    static void diceroll(GroupMessageEvent e);
    static void yb50(GroupMessageEvent e);
};


#endif //MIRAICP_SUZUCHAN_H
