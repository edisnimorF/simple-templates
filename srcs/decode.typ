// Decode `//@`-annotated source files. The annotation rules are documented in the readme,
// under "Annotating a source file".

#let marker-re = regex("(?m)^[ \t]*//@[^\n]*$") // whole line: opens or closes a region
#let block-re = regex("(?ms)^[ \t]*/\\*@[ \t]*(\\w+)(.*?)\\*/") // a /*@tag value*/ block
#let lead-re = regex("^[ \t]*")
#let head-blank-re = regex("^(?:[ \t]*\n)+") // blank lines, plus the marker's own newline
#let tail-blank-re = regex("(?:\n[ \t]*)+$")

// Longest whitespace prefix shared by every non-blank line.
#let indent(lines) = {
  let nb = lines.filter(l => l.trim() != "")
  if nb.len() == 0 { return "" }
  let lead = nb.first().matches(lead-re).first().text
  lead.slice(0, range(lead.len() + 1).filter(k => nb.all(l => l.starts-with(lead.slice(0, k)))).last())
}

// Remove that prefix from each line; nothing is removed unless all lines share it.
#let dedent(lines) = {
  let p = indent(lines)
  if p == "" { lines } else { lines.map(l => if l.starts-with(p) { l.slice(p.len()) } else { l }) }
}

// Body of a region: no blank lines around it, then dedented.
#let body(c) = dedent(c.replace(head-blank-re, "").replace(tail-blank-re, "").split("\n")).join("\n")

// A `/*@tag value*/` match as (tag, value), with each value line trimmed.
#let tag-pair(m) = (
  m.captures.first(),
  m.captures.last().split("\n").map(l => l.trim()).join("\n").trim(),
)

// Last non-empty value of `tag`. Unknown tags are never asked for.
#let value(pairs, tag) = pairs.filter(p => p.first() == tag and p.last() != "").map(p => p.last()).at(-1, default: "")

// Raw bodies of the regions to keep: the odd chunks, each closed by a later marker. A file
// without any marker has no region, so it is taken whole with its tag blocks cut out.
#let regions(chunks, text) = if chunks.len() == 1 {
  (text.replace(block-re, ""),)
} else {
  chunks.filter(((i, _)) => calc.rem(i, 2) == 1 and i < chunks.len() - 1).map(((_, c)) => c)
}

// Source text -> the `code-infos` dict.
#let decode-string(src) = {
  let text = src.replace("\u{feff}", "").replace("\r\n", "\n")
  let chunks = text.split(marker-re).enumerate()
  // Splitting on markers alternates: even chunks are outside every region, odd ones inside.
  let pairs = chunks.filter(((i, _)) => calc.rem(i, 2) == 0).map(((_, c)) => c.matches(block-re)).flatten().map(tag-pair)
  (
    name: value(pairs, "name"),
    snippets: regions(chunks, text).map(body).filter(s => s.trim() != ""),
    note: value(pairs, "note"),
    sample-in: value(pairs, "in"),
    sample-out: value(pairs, "out"),
  )
}

// `read` resolves against this file rather than the caller, so a document should call
// `decode-string(read(path))` itself; `decode` is for sources next to this package.
#let decode(path) = decode-string(read(path))
