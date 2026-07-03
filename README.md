# cppworkshop-examples

Curated, version-tagged **C++ usage examples** — the content corpus behind the
CppWorkshop reference site. Every example is original code that compiles under
the C++ standard it declares.

This repository is **content only**. The build driver, sync script, database and
web frontend live in the separate private `cppworkshop-site` repository.

## Layout

```
content/
  <section-slug>/
    section.yaml            # section title, description, sort order
    01-<example-slug>/
      meta.yaml             # title, standard, difficulty, tags, summary, notes
      main.cpp             # a real int main() that instantiates the feature
      *.hpp / *.cpp        # optional extra files (multi-file examples)
      quiz.yaml            # optional multiple-choice quiz
```

## Conventions

- **The C++ standard is declared once**, in `meta.yaml` (`standard:`). The build
  derives the `-std=` flag from it — never hard-code a standard anywhere else.
- **Every example has a `main.cpp` with a real `int main()`** that uses or
  instantiates the feature. Templates are only checked when instantiated.
- **File selection is by convention**: all `*.cpp` in an example directory are
  compiled together with `-I <dir>`; `*.hpp` files are never compiled alone.
- **Original code only.** Do not copy or closely paraphrase examples or prose
  from cppreference. Linking to cppreference from tags is fine.

## meta.yaml

```yaml
title: "Move vs copy: seeing which one the compiler picks"
standard: "C++11"          # MINIMUM standard; single source of truth for -std=
difficulty: 1              # 1..3
summary: "One-line description shown in listings."
notes: "Longer nuance / gotchas shown on the example page."
run: true                  # execute the built binary (required for concurrency)
sanitizers: []             # e.g. [thread, undefined, address]
main_tags:
  - { name: "std::move", url: "https://en.cppreference.com/w/cpp/utility/move" }
detailed_tags:
  - { name: "move constructor", url: "https://en.cppreference.com/w/cpp/language/move_constructor" }
```

## Sections

- `move-semantics` — std::move, move ctor/assignment, rule of five, moving into
  containers, perfect forwarding, noexcept moves.

## License

MIT — see [LICENSE](LICENSE). The examples are original code, so a permissive
license applies.
