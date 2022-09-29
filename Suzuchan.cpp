//
// Created by suzuco on 2/2/22.
//

#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <dlfcn.h>
#include <Python.h>

#include "Suzuchan.h"

void Suzuchan::process(GroupMessageEvent e)
{
    Member src = e.sender;
    std::string msg = e.message.toMiraiCode();

    Logger::logger.info(msg);
    if (boost::starts_with(msg, "mb40"))
        maib40(e);
    else if (boost::starts_with(msg, "rcat "))
        randcat(e);
    else if (boost::starts_with(msg, "rsel ") || boost::starts_with(msg, "resl "))
        randsel(e);
    else if (boost::starts_with(msg, "gojb"))
        go_jubeat(e);
    else if (msg == "抽卡")
        gacha(e, false);
    else if (msg == "抽烟")
        gacha(e, true);
    else if (boost::starts_with(msg, "递烟"))
        smoke(e, "烟");
    else if (boost::starts_with(msg, "啵啵"))
        smoke(e, "啵");
    else if (boost::starts_with(msg, "roll"))
        diceroll(e);
    else if (boost::starts_with(msg, "yb50"))
        yb50(e);
    else if (boost::starts_with(msg, "shfl "))
        shuffle(e);
    else if (boost::starts_with(msg, "scat "))
        shuffle(e, true);
    else if (boost::starts_with(msg, "eval "))
        evaluate(e);
}

void Suzuchan::fortune(GroupMessageEvent e)
{

}

void Suzuchan::randsel(GroupMessageEvent e)
{
    std::string msg = person_swap(e.message.toMiraiCode(), e);
    std::vector<std::string> candidates = vectorize(msg);
    candidates.erase(candidates.begin());
    if (unlikely(candidates.empty())) {
        e.group.sendMessage("喵有签却要我嗯抽，也是一种抽签。\n（用法：rsel 选项1 [选项2 [选项3 [...]]]）");
        return;
    }
    switch (candidates.size()) {
        case 1:
            candidates.emplace_back("不要");
            break;
        case 3:
        case 4:
            if (!(urandom<ull>() % 2)) {
                candidates.emplace_back("小孩子才做选择，大人全都要");
                candidates.emplace_back("小孩子才做选择，大人做填空");
            }
            break;
    }
    ull idx = urandom<ull>() % candidates.size();
    e.group.sendMessage(MiraiCode(candidates.at(idx) + "！"));
}

bool pyenv_ok;
static PyObject * sys, * path;
static PyObject * mod_gojb_string, * mod_gojb, * mod_gojb_dict, * go;
static PyObject * mod_rcat_string, * mod_rcat, * mod_rcat_dict, * rcat;
static PyObject * mod_maib40_string, * mod_maib40, * mod_maib40_dict, * maib40_gen;
void Suzuchan::_initialize_pyenv()
{
    if (pyenv_ok)
        return;
    auto libpython = dlopen("libpython3.10.so", RTLD_LAZY | RTLD_GLOBAL);
    if (!libpython) {
        std::cerr << "Failed to load libpython3.10.so." << std::endl;
        exit(1);
    }
    Py_Initialize();
    sys = PyImport_ImportModule("sys");
    path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("."));

    mod_gojb_string = PyUnicode_FromString((char *)"gojb.gojb");
    mod_gojb = PyImport_Import(mod_gojb_string);
    mod_gojb_dict = PyModule_GetDict(mod_gojb);
    go = PyDict_GetItemString(mod_gojb_dict, "go");

    mod_rcat_string = PyUnicode_FromString((char *)"rcat");
    mod_rcat = PyImport_Import(mod_rcat_string);
    mod_rcat_dict = PyModule_GetDict(mod_rcat);
    rcat = PyDict_GetItemString(mod_rcat_dict, "rcat");

    mod_maib40_string = PyUnicode_FromString((char *)"maib40");
    mod_maib40 = PyImport_Import(mod_maib40_string);
    mod_maib40_dict = PyModule_GetDict(mod_maib40);
    maib40_gen = PyDict_GetItemString(mod_maib40_dict, "maib40");

    pyenv_ok = true;
}

void Suzuchan::randcat(GroupMessageEvent e)
{
    _initialize_pyenv();
    auto msg = person_swap(e.message.toMiraiCode().substr(5), e);
    PyObject * rcat_string = PyUnicode_FromString(msg.c_str());

    PyObject * args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, rcat_string);
    PyObject * result = PyObject_CallObject(rcat, args);
    Py_ssize_t size;
    const char * data = PyUnicode_AsUTF8AndSize(result, &size);
    std::string response(data, size);

    e.group.sendMessage(MiraiCode(response + "！"));
}

void Suzuchan::shuffle(GroupMessageEvent e, bool concatenate)
{
    std::string msg = person_swap(e.message.toMiraiCode(), e);
    std::vector<std::string> candidates = vectorize(msg);
    candidates.erase(candidates.begin());
    if (unlikely(candidates.empty())) {
        e.group.sendMessage("nPr = n! / (n-r)!\n"
                            "用法：\n"
                            "shfl 选项1 [选项2 [选项3 [...]]]\n"
                            "scat 字符串1 [字符串2 [字符串3 [...]]]");
        return;
    }
    std::string result;
    std::shuffle(candidates.begin(), candidates.end(), std::default_random_engine(urandom<ull>()));
    std::string delimiter = concatenate ? "" : " ";
    for (const std::string & s : candidates) {
        result += s + delimiter;
    }
    if (!concatenate) result.pop_back();
    result += "！";
    e.group.sendMessage(MiraiCode(result));
}

void Suzuchan::go_jubeat(GroupMessageEvent e)
{
    _initialize_pyenv();
    PyObject * go_args = PyUnicode_FromString(e.message.toMiraiCode().c_str());
    PyObject * args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, go_args);
    PyObject * result = PyObject_CallObject(go, args);
    Py_ssize_t size;
    const char * data = PyUnicode_AsUTF8AndSize(result, &size);
    std::string response(data, size);

    e.group.sendMessage(response);
}

void Suzuchan::diceroll(GroupMessageEvent e)
{
    std::string msg = e.message.toMiraiCode();
    dice_param param = parse_dice(msg);
    if (param.count <= 0) {
        e.group.sendMessage("用法：roll AdX[+B]");
        return;
    }
    if (param.count > 255) {
        e.group.sendMessage("骰子批发？9.9包邮？");
        return;
    }
    if (param.faces > 1023) {
        e.group.sendMessage("这不是球吗。");
        return;
    }
    std::string result;
    std::vector<ull> dice;
    long long points = param.offset;
    for (auto i = 0; i < param.count; ++i) {
        auto d = 1 + std::abs(urandom<int>()) % param.faces;
        dice.push_back(d);
        points += d;
    }
    result += std::to_string(param.count)+"d"+std::to_string(param.faces);
    if (param.offset < 0) result += std::to_string(param.offset);
    else if (param.offset > 0) result += "+" + std::to_string(param.offset);
    result += " = " + std::to_string(points) + "！";
    if (param.count <= 14) {
        result += "（";
        for (auto & d : dice) {
            result += std::to_string(d) + "、";
        }
        result.pop_back();result.pop_back();result.pop_back();
        result += "）";
    }
    else {
        result += "（骰子太多不写了x）";
    }
    e.group.sendMessage(result);
}

ull Suzuchan::generate_gacha_qword()
{
    ull result = 0;
    // SSR 1% ; SR 10% ; R 89%
    // 110s    010s    000s
    // 4 bits for each card, with 3 bits for rarity and 1 for sign
    for (auto i = 0; i < 10; ++i) {
        result <<= 4;
        auto draw = urandom<ull>() % 200;
        result += draw & 1;
        if (draw < 22)
            result += 4;
        if (draw < 2)
            result += 8;
    }
    return result;
}

void Suzuchan::gacha(MiraiCP::GroupMessageEvent e, bool smoke)
{
    std::string response;
    std::string msg(e.message.toMiraiCode());

    bool bot_is_mod = e.group.getMember(e.bot.id, nullptr).permission > 0;
    if (smoke && !bot_is_mod)
        return;

    auto deck = generate_gacha_qword();
    int sum = smoke ? 0 : -65535;

    response += make_at(e.sender.id());
    response += " 抽到了...\n";

    for (auto i = 0; i < 10; ++i) {
        auto card = deck & 0xf;
        deck >>= 4;
        switch (card) {
            case 0xc:
            case 0xd:
                response += "[SSR]";
                if (smoke) {
                    response += ((card & 1) == 1) ? "+6分" : "-6分";
                    sum += ((card & 1) == 1) ? 360 : -360;
                }
                break;
            case 0x4:
            case 0x5:
                response += "[SR]";
                if (smoke) {
                    response += ((card & 1) == 1) ? "+3分" : "-3分";
                    sum += ((card & 1) == 1) ? 180 : -180;
                }
                break;
            case 0x0:
            case 0x1:
                response += "[R]";
                if (smoke) {
                    response += ((card & 1) == 1) ? "+半分" : "+30喵";
                    sum += 30;
                }
                break;
            default:
                Logger::logger.error("card=" + std::to_string(card) + '\n');
                abort();
        }
        response += " ";
    }
    response.pop_back();
    response += "！";

    if (sum > 0)
    {
        response += "\n差不多" + std::to_string(sum / 60) + "分钟喵！";
        try { e.sender.mute(sum); }
        catch (...) {}
    }
    else if (smoke)
        response += "\n...手气不错喵！";
    e.group.sendMessage(MiraiCode(response));
}

void Suzuchan::smoke(GroupMessageEvent e, const std::string & verb)
{
    std::string response;
    std::string msg(e.message.toMiraiCode());

    bool bot_is_mod = e.group.getMember(e.bot.id, nullptr).permission > 0;
    if (!bot_is_mod)
        return;

    std::smatch match_result;
    if (std::regex_search(msg, match_result, at)) {
        QQID target_id = std::stoull(match_result[1]);
        if (target_id == e.bot.id) {
            response += "长能耐了哦，宝宝";
            try { e.sender.mute(60); }
            catch (...) {}
        }
        else {
            Member target(target_id, e.group.id(), e.bot.id);
            Group group(e.group.id(), e.bot.id);
            ull p = urandom<ull>() % 99 + 1, q = urandom<ull>() % 99 + 1;
            if (q == p) {
                if (unlikely(p == 1)) q = 2;
                else q = p - 1;
            }
            response += "对方掷出了" + std::to_string(p) + "点！\n";
            response += "你掷出了" + std::to_string(q) + "点！\n";
            if (p > q) {
                if (e.sender.permission > 0) {
                    response += "没" + verb + "上，但你可以自己动动手。";
                }
                else {
                    e.sender.mute(60);
                    response += "你自己" + verb + "上罢。";
                }
            }
            else {
                if (target.permission > 0) {
                    response += verb + "上了，但没完全" + verb + "上。";
                }
                else {
                    target.mute(60);
                    response += verb + "上了。";
                }
            }
        }
    }
    else {
        response += verb + "谁？.txt";
    }

    e.group.sendMessage(response);
}

void Suzuchan::evaluate(GroupMessageEvent e)
{
    std::string expr_str = e.message.toMiraiCode().substr(5 /*kw_evaluate.size()*/);
    e.group.sendMessage(eval(expr_str) + "！");
}

void Suzuchan::yb50(GroupMessageEvent e) {
    system("python3 ./maimai-profile-parser/app.py png");
    Image i = e.group.uploadImg("miaomiaodx_ratings_ahiru233_0.png");
    e.group.sendMessage(i);
}

void Suzuchan::maib40(GroupMessageEvent e)
{
    _initialize_pyenv();
    PyObject * mb40_args = PyUnicode_FromString(std::to_string(e.sender.id()).c_str());
    PyObject * args = PyTuple_New(1);
    PyTuple_SetItem(args, 0, mb40_args);
    PyObject * result = PyObject_CallObject(maib40_gen, args);
    Py_ssize_t size;
    const char * data = PyUnicode_AsUTF8AndSize(result, &size);
    std::string response(data, size);

    if (boost::starts_with(response, "/tmp/")) {
        Image i = e.group.uploadImg(response);
        e.group.sendMessage(i);
    }
    else if (response == "400") {
        e.group.sendMessage("查无此人喵。打开https://www.diving-fish.com/maimaidx/prober/有数据导入指南喵。");
    }
    else if (response == "403") {
        e.group.sendMessage("你不许查了喵.jpg");
    }
}

template <typename T> T Suzuchan::urandom()
{
    T result = 0;
    std::ifstream randdev("/dev/urandom", std::ios::in | std::ios::binary);
    if (randdev) {
        randdev.read(reinterpret_cast<char *>(&result), sizeof(T));
        if (!randdev) {
            Logger::logger.error("Failed to read /dev/urandom");
        }
    }
    else {
        Logger::logger.error("Unable to open /dev/urandom");
    }
    randdev.close();
    return result;
}

std::vector<std::string> Suzuchan::vectorize(const std::string &s)
{
    std::stringstream ss(s);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> vstrings(begin, end);
    return vstrings;
}

std::string Suzuchan::person_swap(const std::string &s, const GroupMessageEvent & e)
{
    auto result = s;
    auto at_text = make_at(e.bot.id);
    if (boost::contains(s, at_text))
        result = std::regex_replace(s, std::regex("你"), at_text);
    result = std::regex_replace(result, std::regex("我"), "你");
    result = std::regex_replace(result, std::regex(make_at(e.bot.id, true)), "我");
    return result;
}

dice_param Suzuchan::parse_dice(const std::string & s)
{
    int count, faces, offset;
    auto parse = std::smatch();
    auto match = std::regex_match(s, parse, dice_notation);
    if (match) {
        count = std::stoi(parse[1].str());
        faces = std::stoi(parse[2].str());
        offset = parse[3].str().empty() ? 0 : std::stoi(parse[3].str());
    }
    else {
        count = -1;
    }
    return { count, faces, offset };
}