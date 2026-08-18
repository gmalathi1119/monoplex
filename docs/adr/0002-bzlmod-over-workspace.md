# ADR 0002: Use bzlmod (MODULE.bazel) instead of legacy WORKSPACE

## Status
Accepted

## Context
Bazel historically resolved external dependencies via a `WORKSPACE` file,
which requires manually and transitively declaring every dependency's
dependencies (no real dependency resolution — just textual concatenation of
whatever each ruleset's macro pulls in). This causes:

- **Diamond dependency conflicts** that are resolved by load-order accident
  rather than an explicit policy.
- **No dependency deduplication** across rulesets pulling in different
  versions of the same external repo.
- **Painful upgrades**, since bumping one ruleset's WORKSPACE snippet can
  silently break another's transitive pin.

Bazel 7+ ships `bzlmod` (MODULE.bazel) as the replacement: a real module
resolution system (MVS-style, similar in spirit to Go modules) with a central
registry (registry.bazel.build) and explicit `bazel_dep` / `use_repo` /
extension APIs.

## Decision
Use `MODULE.bazel` / bzlmod exclusively. `WORKSPACE` is not present in this
repo (an empty `WORKSPACE.bzlmod` marker is sufficient where required by
older tooling).

## Consequences
- **Positive:** deterministic version resolution, smaller and more legible
  dependency declarations, better long-term support (Bazel is deprecating
  WORKSPACE — Bazel 9 removes it entirely).
- **Negative / risk:** a subset of older rulesets (particularly some
  Node/TS and vendor-internal rules) do not yet publish to the Bazel
  Central Registry and require a `bazel_dep` + manual `archive_override`, or
  aren't compatible at all. We track this per-dependency rather than
  reverting the whole repo to WORKSPACE. See ADR 0004 for how this affects
  the TypeScript rollout timeline.
- CI and local dev must pin a Bazel version via `.bazelversion` (managed with
  Bazelisk) to avoid resolution drift between bzlmod registry snapshots.
