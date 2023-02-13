"""
Universalis.app API
"""
import requests
import json
from typing import Dict
from datetime import datetime
from .xivdb_cn import api_item_search, api_item_info
from .text_renderer import render_simple


def market_search(item_id: int, params: Dict) -> Dict:
    try:
        url = "https://universalis.app/api/v2/{}/{}?listings=6&entries=6{}"\
            .format(params["dc"], item_id, params["hq"])
        resp = requests.get(url)
    except Exception:
        return {"Error": "乌乌，网线被拔力。"}
    if resp.status_code >= 500:
        return {"Error": "我趣，土豆熟了（HTTP {}）"
                         .format(resp.status_code)}
    elif resp.status_code == 400:
        return {"Error": "API参数有误捏。（HTTP 400）"}
    elif resp.status_code == 404:
        return {"Error": "我趣，服务器蒸发力（HTTP 404）"}
    elif resp.status_code != 200:
        return {"Error": "虽然布吉岛发生了甚喵但是API挂了捏。（HTTP {}）"
                         .format(resp.status_code)}
    content = json.loads(resp.content)
    return content


def msearch(args: str) -> str:
    args = args.split()
    params = {"hq": "", "dc": "猫小胖"}

    if len(args) < 2:
        return "用法：msearch 物品名|物品ID [hq] [数据中心]"

    item = api_item_search(args[1])
    if item["Pagination"]["ResultsTotal"] == 1:
        item_name = args[1]
        item_id = item["Results"][0]["ID"]
    else:
        try:
            item_id = int(args[1])
        except ValueError:
            return "需要物品的数字ID捏。可以先用isearch查询喵。"
        item_name = api_item_info(item_id)
        if "Error" in item_name.keys():
            return item_name["Error"]
        item_name = item_name["Name_chs"]

    for s in args:
        if "hq" == s.lower():
            params["hq"] = "&hq=true"

        elif s in ["鸟", "陆行鸟"] or s.lower() in ["n", "lxn"]:
            params["dc"] = "陆行鸟"
        elif s in ["猪", "莫古力"] or s.lower() in ["z", "mgl"]:
            params["dc"] = "莫古力"
        elif s in ["猫", "猫小胖"] or s.lower() in ["m", "mxp"]:
            params["dc"] = "猫小胖"
        elif s in ["狗", "豆豆柴"] or s.lower() in ["g", "ddc"]:
            params["dc"] = "豆豆柴"

    response = market_search(item_id, params)
    if "Error" in response.keys():
        return response["Error"]

    result_text_all = ""
    result_text_dull = ""
    result_text_highlight = "{} / {}".format(params["dc"], item_name)
    result_text_overlay = "{} /".format(params["dc"])

    result_text_all += "\n上次更新：{}".format(
        datetime.fromtimestamp(response["lastUploadTime"] / 1000)
    ).split(".")[0]
    result_text_dull += "\n上次更新："
    result_text_highlight += "\n"
    result_text_overlay += "\n"

    result_text_all += "\n"
    result_text_dull += "\n正在出售"
    result_text_highlight += "\n"
    result_text_overlay += "\n"

    if len(response["listings"]) == 0:
        result_text_all += "\n"
        result_text_dull += "\n（暂无出售）"
        result_text_highlight += "\n"
        result_text_overlay += "\n"
    for i in response["listings"]:
        result_text_all += "\n"
        result_text_dull += "\n{}×{}={} {} {}@{}".format(
            i["pricePerUnit"], i["quantity"], i["total"],
            "HQ" if i["hq"] else "",
            i["retainerName"], i["worldName"]
        )
        result_text_highlight += "\n{}×{}={} {}".format(
            i["pricePerUnit"], i["quantity"], i["total"],
            "HQ" if i["hq"] else ""
        )
        result_text_overlay += "\n{}×{}={}".format(
            i["pricePerUnit"], i["quantity"], i["total"]
        )

    result_text_all += "\n"
    result_text_dull += "\n交易记录"
    result_text_highlight += "\n"
    result_text_overlay += "\n"

    if len(response["recentHistory"]) == 0:
        result_text_all += "\n"
        result_text_dull += "\n（暂无交易记录）"
        result_text_highlight += "\n"
        result_text_overlay += "\n"
    for i in response["recentHistory"]:
        result_text_all += "\n"
        result_text_dull += "\n{}×{}={} {} {}@{}".format(
            i["pricePerUnit"], i["quantity"], i["total"],
            "HQ" if i["hq"] else "",
            i["buyerName"], i["worldName"]
        )
        result_text_highlight += "\n{}×{}={} {}".format(
            i["pricePerUnit"], i["quantity"], i["total"],
            "HQ" if i["hq"] else ""
        )
        result_text_overlay += "\n{}×{}={}".format(
            i["pricePerUnit"], i["quantity"], i["total"]
        )

    return render_simple(24, "sarasa-gothic-sc-regular.ttf",
                         [(result_text_all, "#ffffff"),
                          (result_text_dull, "#aaaaaa"),
                          (result_text_highlight, "#eee1c5"),
                          (result_text_overlay, "#ffffff")
                          ])
