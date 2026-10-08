#import "decode.typ": decode-string

// Whole-document style: title block, outline, page numbering and margins.
#let render-style(
  topic: "Competitive Programming Templates",
  author: "author",
  doc,
) = {
  // simple-style's metrics pairing: every family in these two lists has OS/2
  // sTypoAscender/sTypoDescender = 0.800/0.200 em, so a line mixing CJK, Latin and math
  // is always exactly 1.000 em tall no matter which family wins each glyph. The Windows
  // faces come first; `FandolSong` is the free stand-in shipped in `fonts/` that keeps
  // GitHub Actions (which has no CJK font at all) on the same line box. Families that are
  // absent only warn, so a CI log shows two harmless `unknown font family` lines.
  set text(font: ("XCharter", "STKaiti", "STSong", "FandolSong"))
  show math.equation: set text(font: "Erewhon Math")

  show title: set align(center)
  title(topic)
  align(center)[#author]

  outline(depth: 2)
  pagebreak()
  counter(page).update(1)

  show heading.where(level: 1): set heading(numbering: "1")
  show heading.where(level: 2): set heading(numbering: "1.1")
  set page(
    // columns: 2,
    margin: (x: 5em, y: 5em),
    numbering: "1",
  )
  doc
}

// One snippet in a shaded rounded block, highlighted as `lang`.
#let code(snippet, lang: "") = {
  block(
    width: 100%,
    fill: luma(248),
    inset: 10pt,
    radius: 5pt,
    raw(lang: lang, snippet),
  )
}

// Render one `code-infos` dict: a heading, one block per snippet, input and output side by
// side, then the note. `path` only names the heading when the dict has no `name`.
#let render-code(
  code-infos: (:),
  heading-level: 2,
  lang: "cpp",
  path: "",
) = {
  // A hand-written dict may omit keys; `decode` always returns all of them.
  let default-code-infos = (
    name: "default name",
    snippets: (),
    note: "",
    sample-in: "",
    sample-out: "",
  )
  code-infos = default-code-infos + code-infos

  // Nothing to show, so stay silent: a file can be listed before it has content.
  let fields = (code-infos.snippets, code-infos.note, code-infos.sample-in, code-infos.sample-out)
  if fields.all(f => f.len() == 0) { return }

  // The heading is `@name`, else the file name, else the placeholder.
  let name = if code-infos.name != "" {
    code-infos.name
  } else if path != "" {
    path.split("/").last()
  } else {
    default-code-infos.name
  }

  heading(name, level: heading-level)
  for snippet in code-infos.snippets {
    code(snippet, lang: lang)
  }
  grid(
    columns: (1fr, 1fr),
    [#if code-infos.sample-in != "" {
      heading("Input", level: heading-level + 1)
      code(code-infos.sample-in, lang: "txt")
    }],
    [#if code-infos.sample-out != "" {
      heading("Output", level: heading-level + 1)
      code(code-infos.sample-out, lang: "txt")
    }],
  )
  if code-infos.note != "" {
    heading("Note", level: heading-level + 1)
    code-infos.note
  }
}

// Join a directory and a relative path, adding "/" only when `left` lacks a trailing one.
#let join(left, right) = if left == "" or left.ends-with("/") { left + right } else { left + "/" + right }

// Decode `path` under `root` and render it. `root` is relative to the repository root and
// defaults to `codes/`, so `render-code-file("tests/a.cpp")` reads `codes/tests/a.cpp`.
// `read` here resolves against this file in `srcs/`, hence the leading `../`.
//
// For a source outside the repository, read it in the document instead -- there `read`
// resolves against that document, and `path` only names the fallback heading:
//
//   #render-code(code-infos: decode-string(read(path)), path: path)
#let render-code-file(path, root: "codes/", heading-level: 2, lang: "cpp") = {
  render-code(
    code-infos: decode-string(read("../" + join(root, path))),
    heading-level: heading-level,
    lang: lang,
    path: path,
  )
}
