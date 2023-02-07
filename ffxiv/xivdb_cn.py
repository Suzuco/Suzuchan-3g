"""
XIVDB API, the CN fork
"""

import requests
import json
from urllib.parse import quote_plus
from .text_renderer import render_simple


def api_item_search(kw: str):
    """
    isearch kw
    :param kw: keyword
    :return: json-converted dict if query succeeds;
             dict with error message if fails
    """
    try:
        resp = requests.get(
            "https://cafemaker.wakingsands.com/search?string={}&indexes=Item"
            .format(quote_plus(kw)))
    except Exception:
        return {"Error": "乌乌，网线被拔力。"}
    if resp.status_code >= 500:
        return {"Error": "我趣，土豆熟了（HTTP {}）"
                         .format(resp.status_code)}
    elif resp.status_code >= 400:
        return {"Error": "你不许查了喵。（HTTP {}）"
                         .format(resp.status_code)}
    elif resp.status_code != 200:
        return {"Error": "虽然布吉岛发生了甚喵但是API挂了捏。（HTTP {}）"
                         .format(resp.status_code)}
    content = json.loads(resp.content)
    return content


def api_item_info(item_id: int):
    try:
        item_info = requests.get("https://cafemaker.wakingsands.com/Item/{}"
                                 .format(item_id))
    except Exception:
        return {"Error": "乌乌，网线被拔力。"}
    if item_info.status_code >= 500:
        return {"Error": "我趣，土豆熟了（HTTP {}）"
                .format(item_info.status_code)}
    elif item_info.status_code == 404:
        return {"Error": "查无此物喵。"
                .format(item_info.status_code)}
    elif item_info.status_code != 200:
        return {"Error": "API挂了捏。（HTTP {}）"
                .format(item_info.status_code)}
    item_info = json.loads(item_info.content)
    return item_info


def isearch(args: str) -> str:
    args = args.split()
    if len(args) == 1:
        return "用法：isearch 物品名称"
    item_name = args[1]
    content = api_item_search(item_name)
    if "Error" in content.keys():
        return content["Error"]

    n_hit = content["Pagination"]["ResultsTotal"]
    msg_sys = "共找到{}个物品喵。".format(n_hit)
    msg_id = ""
    msg_name = ""
    if n_hit > 32:
        n_hit = 32
        msg_sys += "\n因为太多了所以只显示前32个物品喵。"
        msg_id += "\n"
        msg_name += "\n"
    for i in range(n_hit):
        msg_id += "\n" + str(content["Results"][i]["ID"]) + " - "
        msg_name += "\n" + str(content["Results"][i]["ID"])\
                    + " - " + content["Results"][i]["Name"]
    return render_simple(24, "sarasa-gothic-sc-regular.ttf",
                         [(msg_sys, "#eee1c5"),
                          (msg_name, "#ffffff"), (msg_id, "#aaaaaa")])

