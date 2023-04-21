# Suzuchan-3G

![GitHub](https://img.shields.io/github/license/Suzuco/Suzuchan-3g)
![GitHub last commit](https://img.shields.io/github/last-commit/Suzuco/Suzuchan-3g)

>Notice: The underlying infrastructure was again, banned, hence the archival and migration to newer platform.
>Nice job, Tencent. /s

based on [Nambers/MiraiCP](https://github.com/Nambers/MiraiCP), with [ArashPartow/exprtk](https://github.com/ArashPartow/exprtk) [MIT]

## 功能

| 命令      | 举例                     | 说明                                       | 管理  |
|---------|------------------------|------------------------------------------|-----|
| roll    | roll 1d6               | 掷骰子                                      ||
| rsel    | rsel 面条 饺子 米饭          | 从空字符分隔的多个字符串中随机抽选一个（`resl`等效，用来免疫手癌）     ||
| rcat    | rcat 洗（衣/碗/地）机         | 随机从提供了的选项中抽选字符串，并替换在对应的位置；`#`会被替换为1位随机数字 ||
| shfl    | shfl 吃饭 洗澡 打游戏         | 将空字符分隔的多个字符串随机排序                         ||
| scat    | scat 1 1 1 4 4 5       | 同`shfl`，但输出不含空格                          ||
| eval    | eval 1+1               | 基于exprtk的计算器                             ||
| gojb    | gojb lv 10 bpm 133-160 | 可以指定多种参数的jubeat随机选曲                      ||
| mb40    | mb40                   | 拉取水鱼maimai查分器上绑定账号的成绩，然后列表               ||
| isearch | isearch 武略魔晶石          | 搜索FF14（国服/简中）中的物品。                       |
| msearch | msearch 13637 hq 猫     | 搜索FF14（国服/简中）市场出售及交易记录                   |
| 运势      | 运势                     | 今日运势，附带一把~~米诺~~起手                        |
| 抽卡      | 抽卡                     | 十连抽卡模拟器（SSR 1%，SR 10%，R 89%，有保底无天井）      ||
| 抽烟      | 抽烟                     | 禁言十连抽，不保证中奖.jpg                          | ✔️  |
| 递烟      | 递烟@易烟丁真                | 一种原始的群内PVP形式（禁言->禁烟->敬烟->递烟）             | ✔️  |
| 啵啵      | 啵啵@猫娘                  | 同`递烟` “如果感觉公共场合抽烟不太恰当，用嘴堵住别人的嘴也是可以的”     | ✔️  |

## Dependencies

- boost
- CPython 3.10
  - requests
  - pillow
  - numpy
  - pandas
