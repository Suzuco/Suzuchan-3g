# Suzuchan-3G

![GitHub](https://img.shields.io/github/license/Suzuco/Suzuchan-3g)
![GitHub last commit](https://img.shields.io/github/last-commit/Suzuco/Suzuchan-3g)
![works on my computer](https://img.shields.io/badge/works%20on-my%20computer-important)

based on [Nambers/MiraiCP](https://github.com/Nambers/MiraiCP)

with [ArashPartow/exprtk](https://github.com/ArashPartow/exprtk) [MIT]

## 功能

| 命令   | 说明                                   | 管理  |
|------|--------------------------------------|-----|
| roll | 掷骰子                                  ||
| rsel | 从空字符分隔的多个字符串中随机抽选一个（`resl`等效，用来免疫手癌） ||
| rcat | 随机从提供了的选项中抽选字符串，并替换在对应的位置            ||
| shfl | 将空字符分隔的多个字符串随机排序                     ||
| scat | 同`shfl`，但输出不含空格                      ||
| eval | 基于exprtk的计算器                         ||
| gojb | 可以指定多种参数的jubeat随机选曲                  ||
| mb40 | 拉取水鱼maimai查分器上绑定账号的成绩，然后列表           ||
| 抽卡   | 十连抽卡模拟器（SSR 1%，SR 10%，R 89%，无保底无天井）  ||
| 抽烟   | 禁言十连抽，不保证中奖.jpg                      | ✔️  |
| 递烟   | 一种原始的群内PVP形式（禁言->禁烟->敬烟->递烟）         | ✔️  |
| 啵啵   | 同`递烟` “如果感觉公共场合抽烟不太恰当，用嘴堵住别人的嘴也是可以的” | ✔️  |

## Dependencies

- boost
- CPython 3.10
  - requests
  - pillow
  - numpy
  - pandas
