
import math
import random
import hashlib
from datetime import date


tiles = list(range(ord('\U0001F000'), ord('\U0001F022')))
tiles = tiles[7:] + tiles[0:4] + [tiles[6], tiles[5], tiles[4]]


def fortune(uid: str):
    token = uid + "@" + str(date.isoformat(date.today()))
    cookies = list(hashlib.sha1(token.encode('utf-8')).digest())

    rp = int(math.fabs(cookies[0] - cookies[1])) - 128

    rgen = random.Random(cookies[2])
    hand = rgen.sample(range(136), 14)
    hand.sort()
    str_hand = "".join([chr(tiles[t // 4]) for t in hand])

    return f"""：
今日人品：{rp}
一把起手：{str_hand}"""
