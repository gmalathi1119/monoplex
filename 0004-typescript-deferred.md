# ADR 0004: Defer full TypeScript/Node Bazel integration

## Status
Accepted

## Context
The monorepo aims to be genuinely polyglot, and a meaningful fraction of
internal tooling and frontends are TypeScript/Node. Two ruleset options
exist:

- `rules_nodejs` / `rules_ts` (community, bzlmod support inconsistent across
  versions at time of writing)
- `aspect_rules_ts` + `aspect_rules_js` (more actively maintained, better
  bzlmod support, but still requires pnpm-in-Bazel plumbing and a
  significantly more complex `MODULE.bazel` surface: pnpm lock parsing,
  npm package hub repos, and per-package `BUILD.bazel` generation via a
  codegen step).

Bringing this in now would roughly double the MODULE.bazel surface area and
introduce a second, less mature dependency-resolution path (npm/pnpm) layered
on top of bzlmod, before the Go/Python/C++/Java foundation is proven out.

## Decision
Ship the initial monorepo with Go, Python, C++, and Java only. TypeScript/Node
services remain in their existing repo(s) with native tooling (`tsc`, `pnpm`,
`turborepo`) until:
- the four initial languages' Bazel setup is stable in CI for a full quarter, and
- `aspect_rules_js`/`aspect_rules_ts` bzlmod support is confirmed stable for
  our pnpm workspace layout (tracked as a spike, not blind adoption).

## Consequences
- Frontend/tooling teams keep their current (faster to iterate) native JS
  toolchain in the near term — this is a deliberate scope cut, not an
  oversight.
- Cross-language dependency visibility (ADR 0001's core motivation) is
  incomplete until TS is folded in; tracked as a known gap, not silently
  dropped.
- Avoids importing build-graph instability from an immature integration into
  the rest of the monorepo's CI.
