#!/usr/bin/env python3

from typing import List, Dict

import requests
from PIL import Image, ImageFont, ImageDraw
import numpy as np


def maib40(qqid):
    payload = {"qq": str(qqid)}
    fn = "/tmp/{}.png".format(str(qqid))
    resp = requests.post("https://www.diving-fish.com/api/maimaidxprober/query/player",
                         json=payload, allow_redirects=True)
    if resp.status_code == 400:
        return "400"
    if resp.status_code == 403:
        return "403"
    obj = resp.json()
    dx: List[Dict] = obj["charts"]["dx"]
    sd: List[Dict] = obj["charts"]["sd"]

    img = Image.new("RGB", (546, 806), "#c9ffdf")
    fnt = ImageFont.truetype("sarasa-gothic-sc-regular.ttf", 14)
    txt_color = (14, 14, 14)
    d = ImageDraw.Draw(img)
    d.text((14, 14), "{} : {} = 新{} + 旧{} + 段{}".format(
        obj['username'],
        obj['additional_rating'] + obj['rating'],
        np.sum([c["ra"] for c in dx]),
        np.sum([c["ra"] for c in sd]),
        obj["additional_rating"]),
           font=fnt, fill=txt_color)

    d.text((155, 32), "==== 不太新不太热 BEST 15 ====", font=fnt, fill=txt_color)
    for i in range(15):
        if i >= len(dx):
            break
        d.text((14, 50 + i * 18), str(dx[i]["title"][0:18]), font=fnt, fill=txt_color)
        d.text((330, 50 + i * 18), str(dx[i]["level_label"][0:2]), font=fnt, fill=txt_color)
        d.text((360, 50 + i * 18), str(dx[i]["ds"]), font=fnt, fill=txt_color)
        d.text((406, 50 + i * 18), str(dx[i]["achievements"]), font=fnt, fill=txt_color)
        d.text((502, 50 + i * 18), str(dx[i]["ra"]), font=fnt, fill=txt_color)

    d.text((155, 320), "==== 非常冷非常旧 BEST 25 ====", font=fnt, fill=txt_color)
    for i in range(25):
        if i >= len(sd):
            break
        d.text((14, 338 + i * 18), str(sd[i]["title"][0:18]), font=fnt, fill=txt_color)
        d.text((330, 338 + i * 18), str(sd[i]["level_label"][0:2]), font=fnt, fill=txt_color)
        d.text((360, 338 + i * 18), str(sd[i]["ds"]), font=fnt, fill=txt_color)
        d.text((406, 338 + i * 18), str(sd[i]["achievements"]), font=fnt, fill=txt_color)
        d.text((502, 338 + i * 18), str(sd[i]["ra"]), font=fnt, fill=txt_color)

    with open(fn, "wb") as fp:
        img.save(fp)
    return fn
