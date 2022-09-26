#!/usr/bin/env python3


import re
import random

pattern = r"[\(（](.*?)[\)）]"


def rcat(s: str):
    match = re.search(pattern, s)
    if match is None:
        return s
    return re.sub(pattern,
                  lambda m: random.choice(m.group(1).split('/')),
                  s)
