# simple-templates

Compile `//@`-annotated C++ templates into a printable reference PDF with Typst.

## Quick start

```sh
# --font-path fonts is required: it carries the CJK font the pairing needs
# (without it you only get an `unknown font family` warning, but Chinese would fall back)
typst compile --font-path fonts templates.typ   # or: typst watch --font-path fonts templates.typ
```

```typst
#import "srcs/render.typ": render-code-file, render-style

#show: render-style.with(topic: "CP Templates", author: "Blue Lagoon")

= Test

#render-code-file("others/default.cpp")
```

`render-style(topic:, author:)` prints a centered title and author, a two-level outline,
page numbers that restart after it, `=`/`==` numbered as `1`/`1.1`, and 5em margins. The
design lives in `srcs/render.typ`, where a commented-out `columns: 2` gives a two-column
layout.

`render-code-file(path, root: "codes/", heading-level: 2, lang: "cpp")` reads `path` under
`root`, and `root` is relative to the repository root, so `"others/default.cpp"` means
`codes/others/default.cpp`. Use `root: ""` for paths from the repository root.

To render a file outside the repository, read it in your document instead:
`#render-code(code-infos: decode-string(read(path)), path: path)`, with `decode-string`
imported from `srcs/decode.typ`.

## Annotating a source file

```cpp
/*@name A+B problem*/
/*@in 1 2*/
/*@out 3*/
/*@note this is a note.*/

#include <bits/stdc++.h>
using namespace std;

//@
void solve() {
  int a, b;
  cin >> a >> b;
  printf("a + b = %d\n", a + b);
}
//@

int main() {
  solve();
}
```

Only the text between a pair of `//@` lines is printed; the rest of the file just carries
tags. The example above renders one code block, the `solve` function.

| Block                 | Effect                                        |
| --------------------- | --------------------------------------------- |
| `//@` alone on a line | Opens a code region; the next `//@` closes it |
| `/*@name ...*/`       | The heading, defaulting to the file name      |
| `/*@in ...*/`         | Sample input, printed next to the output      |
| `/*@out ...*/`        | Sample output                                 |
| `/*@note ...*/`       | A note under the samples                      |

- `/*@...*/` blocks are read only outside regions; inside one, the text stays in the code.
- A block may span several lines, the last non-empty one wins, and unknown tags are ignored.
- A region left unclosed at the end of the file is dropped, so close the last one.
- A snippet loses the blank lines around it and the indentation shared by all its lines.
- A file without any `//@` is printed whole with the `/*@...*/` blocks removed, which is how
  `codes/others/default.cpp`, a source with no tags, still renders.
- A file with no region, note or sample prints nothing, so files can be listed in
  `templates.typ` before they have content.

## Layout

```text
codes/          sources to print, the default root
fonts/          XCharter + Erewhon Math + FandolSong, the metrics pairing (see fonts/README.md)
srcs/decode.typ the decoder: source text -> code-infos
srcs/render.typ render-style, render-code, render-code-file
test/test.typ   the smallest document that renders one file
templates.typ   the document that compiles to the book
```

## Fonts

`render-style` sets `XCharter + STKaiti/STSong + FandolSong` for text and `Erewhon Math`
for formulas, i.e. the free half of `simple-style`'s pairing: every family has OS/2
`sTypoAscender/sTypoDescender = 0.800/0.200 em`, so a line mixing Chinese, Latin and math
is always exactly `1.000 em` tall whichever family wins each glyph. `STKaiti`/`STSong` are
Windows-only, so `fonts/` ships the free CTAN stand-in `FandolSong` for CI — the runner has
no CJK font at all, and it warns about the two missing Windows families (harmless).
Details, hashes and measurements: `fonts/README.md`.

Verified with Typst 0.14.2.
