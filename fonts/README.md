# fonts/ —— CI 与本地共用的一套字体

`--font-path fonts` 之后，GitHub Actions（Ubuntu，无任何中文字体）和本地 Windows
排出来的行盒完全一致：**所有文件的 OS/2 `sTypoAscender/sTypoDescender` 都是 0.800 / 0.200 em**。

## 为什么是这几个

simple-style 的默认搭配是 `XCharter + STKaiti/STSong + Erewhon Math`：三者度量都是
0.800 / 0.200 em ⇒ 纯中、纯英、中英混排、中文夹行内公式的行盒恒为 `1.000 em`。

这套搭配的中文半边是 Windows 自带字体（STKaiti / STSong / STFangsong / SimHei），
在 `ubuntu-latest` 上既不存在（该镜像的 apt 列表里唯一的字体包是 `fonts-noto-color-emoji`），
也不能随仓库分发。逐面读过 CTAN 免费中文字体的 OS/2 表之后：

> **FandolSong-Regular 是唯一免费、可分发、且同属 0.800 / 0.200 簇的中文字体。**

所以 CI 里由它顶替 STSong 的位置，其余照旧。

## 文件清单

| 文件 | 大小 | SHA-256（前 24 位） | 来源 | 许可 |
|---|---|---|---|---|
| XCharter-Roman.otf | 146 KB | `E5D22B0F417A0A3E22EE2D7E` | CTAN `xcharter` v1.26 / Michael Sharpe | Bitstream Charter 许可（可自由使用/修改/再分发）+ LPPL 1.3 |
| XCharter-Bold.otf | 132 KB | `BAF595556BBD65749A83B410` | 同上 | 同上 |
| XCharter-Italic.otf | 124 KB | `46100192810480A54DD287E8` | 同上 | 同上 |
| XCharter-BoldItalic.otf | 112 KB | `255D912EB1D70AAC000A27AD` | 同上 | 同上 |
| Erewhon-Math.otf | 443 KB | `AC5E6B6418A71E01C9F560D5` | CTAN `erewhon-math` 0.76 | SIL OFL 1.1（字体内嵌 license 字段） |
| FandolSong-Regular.otf | 4.83 MB | `A3820003227D4ABE31FE28B4` | CTAN `fandol` v0.3（Fandol team: Clerk Ma & Jie Su） | GPL + GPL font exception，见 `FandolSong-COPYING.txt` |

`xcharter.zip` / `erewhon-math.zip` 的成员逐字节比对、两个 CTAN 镜像（`mirrors.ctan.org`
与 `mirrors.tuna.tsinghua.edu.cn/CTAN`）的哈希互校记录在 simple-style 的
`test/fonts/VERIFY.md`；本目录的 5 个文件与那份记录里的哈希一致。
`fandol.zip` 的两个镜像 SHA-256 均为
`9278F01B417DED5766D98C3937192A1A6A2C73A5E94A3493FDFC932B2A55005A`，本目录的
`FandolSong-Regular.otf` 即该 zip 成员，`FandolSong-COPYING.txt` 即其 `fandol/COPYING`。

## 度量（Typst 真正会用的那一对）

| 文件 | OS/2 sTypoAsc / Desc | em | 
|---|---|---|
| XCharter-{Roman,Bold,Italic,BoldItalic}.otf | 800 / −200 @1000 | 0.8000 / 0.2000 |
| Erewhon-Math.otf | 800 / −200 @1000 | 0.8000 / 0.2000 |
| FandolSong-Regular.otf | 800 / −200 @1000 | 0.8000 / 0.2000 |

同一个 CTAN 包里其余的面都**不在**这一簇，所以没有进库：

| 面 | em | 备注 |
|---|---|---|
| FandolSong-Bold | 0.880 / 0.120 | 放进去会让粗体中文那几行高 0.08em |
| FandolHei-Regular / FandolHei-Bold | 0.880 / 0.120 | 黑体无免费同簇替代 |
| FandolKai-Regular | 0.880 / 0.120 | 楷体（STKaiti 的位置）无免费同簇替代 |
| FandolFang-Regular | 0.880 / 0.120 | 同上 |

思源 / Noto / 霞鹜 / Klee / MiSans 全族也是 0.880 / 0.120，而免费数学字体里没有同簇的
（最接近的 New Computer Modern Math 是 0.806 / 0.194）⇒ 混排一行 1.074 em，比纯中文行高 7.4%。

## 为什么刻意不放 FandolSong-Bold.otf

- FandolSong-**Bold** 是 880 / −120，单独放进来会让**粗体中文**突然抖 0.08em（11pt 下 0.88pt）；
- 不放粗体文件时，Typst 用描边伪造粗体 —— 和 Windows 的 STSong / STKaiti 一样，度量不变。

Typst 0.14.2 实测（`typst query` 探针，5 种行：纯中 / 纯英 / 中英混排 / 中文夹 `$x$` / 中文夹 `$sum_(i=1)^n$`）：

| 组合 | regular 波动 | bold 波动 |
|---|---|---|
| `XCharter` + `FandolSong` + `Erewhon Math` | 0.0000 em（5 行全 1.0000） | 0.0000 em（5 行全 1.0000） |
| `XCharter` + `STKaiti` + `Erewhon Math`（本地 Windows） | 0.0000 em（5 行全 1.0000） | 0.0000 em（5 行全 1.0000） |

即：CI 上只有 FandolSong、本地有 STKaiti/STSong，两者排出来的行盒是一样的。

## 用法

```sh
typst compile --font-path fonts --root . templates.typ templates.pdf
```

CI 里就是这一条（见 `.github/workflows/main.yml`）。字体列表已经写死在 `srcs/render.typ`
的 `render-style` 里，本机字体在前、`FandolSong` 兜底：

```typst
#set text(font: ("XCharter", "STKaiti", "STSong", "FandolSong"))
#show math.equation: set text(font: "Erewhon Math")
```

两者度量一致，所以哪个赢都不改变行盒。缺哪个家族只出 warning 不改结果，
所以 CI 日志里会有 `unknown font family: stkaiti` / `stsong` 两条（正常）。

## 重新核验

```powershell
# 1) 度量：直接读 OS/2 两个整数，无依赖
powershell -File <simple-style>\test\tools\os2-typo.ps1 fonts
# 2) 字体自报的版本 / 版权 / 许可
powershell -File <simple-style>\test\tools\font-info.ps1 fonts
# 3) Typst 看到的家族名
typst fonts --font-path fonts
# 4) 与官方包比对（fandol.zip 的哈希见上）
```
