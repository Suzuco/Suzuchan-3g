from typing import List

from PIL import Image, ImageFont, ImageDraw
from datetime import datetime


def render_simple(font_size: int, ttf_path: str,
                  text: List,
                  bg_color="#2e2e2e"):
    """
    Renders all text of a single typeface and a fixed font size.
    :param font_size: Size of font. Will be taken into consideration with
     the font face when deciding the dimension of resulting image.
    :param ttf_path: Path to TrueType Font file.
    :param text: List of text with its color. (text: str, color)
    :param bg_color: Background color.
    :return: Path to generated image file.
    """
    # default palette: #2e2e2e #eee1c5 #ffffff #cccccc
    if len(text) == 0:
        return None
    font = ImageFont.truetype(ttf_path, font_size)
    margin = int(font_size * 1.618)
    img_size = (1024, 2048)

    img = Image.new("RGB", img_size, bg_color)
    canvas = ImageDraw.Draw(img)
    text_size = (0, 0)
    # Decide size of all text boundary boxes and crop image accordingly
    for stencil in text:
        bbox = canvas.multiline_textbbox((0, 0), stencil[0], font=font)
        bbox = (bbox[2], bbox[3] + font.getmetrics()[1])
        text_size = (bbox[0] if bbox[0] > text_size[0] else text_size[0],
                     bbox[1] if bbox[1] > text_size[1] else text_size[1])
    img_size = (text_size[0] + 2 * margin, text_size[1] + 2 * margin)
    img = img.crop((0, 0) + img_size)

    canvas = ImageDraw.Draw(img)
    for stencil in text:
        canvas.multiline_text((margin, margin), stencil[0], font=font,
                              fill=stencil[1])
    fn = "/tmp/suzuchan-xiv{}.png".format(datetime.timestamp(datetime.now()))
    with open(fn, "wb") as fp:
        img.save(fp)
    return fn
