#!/usr/bin/env python3


import re
import random

pattern = r"[\(（](.*?)[\)）]"


def rcat(s: str):
    result = re.sub(pattern,
                    lambda m: random.choice(m.group(1).split('/')),
                    s)
    result = "".join([str(random.randint(0, 9)) if s == '#' else s for s in result])
    return result
