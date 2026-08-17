# Contributing

Thanks for considering a contribution.

## Adding a new service

1. Create a directory under the relevant language root (`go/`, `python/`,
   `cpp/`, `java/`).
2. Add a `BUILD.bazel` (Go: run `bazel run //:gazelle` to generate it).
3. Include a unit test target — CI runs `bazel test //...` and will fail
   builds without test coverage for new logic.
4. If the change introduces a new external dependency, add it to
   `MODULE.bazel` (Go/Python/Java) with a pinned version, not a floating one.

## Adding an ADR

If your change involves a non-obvious trade-off (a new ruleset, a caching
strategy change, a new language), add a numbered ADR under `docs/adr/`
following the existing template: Status, Context, Decision, Consequences.
Include what you *rejected* and why — that's the part most PRs omit and the
part that's most useful to the next person.

## Before opening a PR

```bash
tools/bazel/build_all.sh
bazel run @buildifier_prebuilt//:buildifier -- -mode=fix $(find . -name BUILD.bazel)
```

## Code review expectations

- BUILD file changes should keep target `visibility` as narrow as possible.
- New cross-language dependencies (e.g. a shared proto consumed by two
  languages) should go through design review — open an issue first.
