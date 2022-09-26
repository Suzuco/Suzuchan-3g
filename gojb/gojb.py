#!/usr/bin/env python3

import sys
import pandas as pd
import pickle
import re

# gojb lv 8-10.2 bpm 155-300 bsc adv ext 4个

p_lv = r"[Ll][Vv]\s*(\d{1,2}(?:\.\d)?)(-\d{1,2}(?:\.\d)?)?"
p_bpm = r"[Bb][Pp][Mm]\s*(\d{1,3})(-\d{1,3})?"
p_bsc = r"[Bb][Ss][Cc]|绿"
p_adv = r"[Aa][Dd][Vv]|黄"
p_ext = r"[Ee][Xx][Tt]|红"
p_n = r"(\d+)[连連个首個]"
p_var = r"[变變変]速"


def go(ss: str):
    lv_l, lv_u = 0, 99
    bpm_l, bpm_u = 0, 999
    n = 1
    m_lv = re.search(p_lv, ss)
    m_bpm = re.search(p_bpm, ss)
    m_n = re.search(p_n, ss)
    m_var = re.search(p_var, ss)
    diff = [re.search(p_bsc, ss) is not None, re.search(p_adv, ss) is not None, re.search(p_ext, ss) is not None]
    if (not diff[0]) and (not diff[1]) and (not diff[2]):
        diff = [True, True, True]
    if m_lv:
        if m_lv.group(2):
            lv_l = float(m_lv.group(1))
            lv_u = float(m_lv.group(2)[1:])
            if lv_l > lv_u:
                lv_l, lv_u = lv_u, lv_l
        else:
            lv_l = float(m_lv.group(1))
            lv_u = 10.9 if lv_l == 10 else 9.9 if lv_l == 9 else lv_l
    if m_bpm:
        if m_bpm.group(2):
            bpm_l = int(m_bpm.group(1))
            bpm_u = int(m_bpm.group(2)[1:])
        else:
            bpm_l = int(m_bpm.group(1)) - 5
            bpm_u = int(m_bpm.group(1)) + 5
    if m_n:
        n = int(m_n.group(1))

    # print("lv {}-{}, bpm {}-{}, {} {} {}, {} times".format(
    #     lv_l, lv_u, bpm_l, bpm_u,
    #     "+bsc" if diff[0] else "-bsc",
    #     "+adv" if diff[1] else "-adv",
    #     "+ext" if diff[2] else "-ext",
    #     n
    # ))
    with open("gojb/jubeat_cache.pickle", "rb") as fp_cache_jb:
        cache_jb = pickle.load(fp_cache_jb)
    subset = cache_jb[(cache_jb["bpm1"] >= bpm_l) & (cache_jb["bpm2"] <= bpm_u)]

    if m_var:
        subset = subset[subset["bpm1"] != subset["bpm2"]]

    pool = pd.DataFrame(data=[], columns=["title", "artist", "bpm", "diff", "lv"])
    for selector in [{"idx": 0, "hdr": "bsc_level", "display": "BASIC"},
                     {"idx": 1, "hdr": "adv_level", "display": "ADVANCED"},
                     {"idx": 2, "hdr": "ext_level", "display": "EXTREME"},
                     ]:
        if diff[selector["idx"]]:
            for _, row in subset[(subset[selector["hdr"]] >= lv_l) & (subset[selector["hdr"]] <= lv_u)].iterrows():
                pool = pd.concat([pool, pd.DataFrame(data=[(row.title, row.artist,
                                                            "{}-{}".format(row.bpm1, row.bpm2)
                                                            if row.bpm1 != row.bpm2 else str(row.bpm1),
                                                            selector["display"], row[selector["hdr"]])],
                                                     columns=pool.columns)])
    n = min(n, len(pool))
    result = ""
    if n == 0:
        result = "并喵有这样的鸽喔。"
    elif n == 1:
        row = pool.sample(1).iloc[0]
        result = "{}\n{}\n[{} {}] / BPM {}".format(row.title, row.artist, row["diff"], row.lv, row.bpm)
    else:
        for _, row in pool.sample(n).iterrows():
            result += "{} / [{} {}] / BPM {}\n".format(row.title, row["diff"], row.lv, row.bpm)
        result = result[:-1]
    return result


if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == "update":
        df = pd.read_csv("gojb/jubeat_list.csv", index_col=None)
        df.rename(columns={"bpm": "bpm1"}, inplace=True)

        def bpm_parse(x: pd.Series):
            if "-" in x["bpm1"]:
                x["bpm1"], x["bpm2"] = x["bpm1"].split("-")
            else:
                x["bpm2"] = x["bpm1"]
            return x

        df.insert(3, "bpm2", "")
        df = df.apply(bpm_parse, axis=1)
        for r in ["bpm1", "bpm2"]:
            df[r] = pd.to_numeric(df[r])

        with open("gojb/jubeat_cache.pickle", "wb") as fp:
            pickle.dump(df, fp)
            print("jubeat缓存已更新！")
            exit(0)
        print("jubeat缓存更新失败！")
