# ADR 0001: Adopt a monorepo with Bazel as the build system

## Status
Accepted

## Context
Services in this org currently live in per-language, per-team repos (polyrepo).
As the number of services and cross-language dependencies grows, this creates
recurring problems:

- **Dependency drift.** Shared libraries (e.g. an internal auth SDK consumed
  by Go, Python, and Java services) are versioned and released independently,
  and consumers lag behind for months. Coordinated changes across services
  require a fan-out of PRs and manual version bumps.
- **Inconsistent build/test tooling per repo.** Each team hand-rolls CI,
  leading to divergent caching strategies, flaky test isolation, and
  duplicated effort maintaining N build pipelines instead of one.
- **Poor cross-service visibility.** It's hard to answer "what breaks if I
  change this proto/interface?" without a single dependency graph.
- **Slow incremental builds/tests at scale**, because polyrepo CI typically
  rebuilds/retests everything in a repo rather than only what changed.

## Decision
We will consolidate services into a single monorepo, built with **Bazel**.

Rationale for Bazel specifically, evaluated against alternatives:

| Requirement | Bazel | Nx | Turborepo | Make/per-lang tools |
|---|---|---|---|---|
| True polyglot (Go/Python/C++/Java/TS) | Yes, first-class | JS-centric, others via plugins | JS-centric | Manual glue per language |
| Hermetic, reproducible builds | Yes (sandboxing, no ambient PATH) | No | No | No |
| Correct incremental build/test via content-hashed dependency graph | Yes | Partial | Partial | No |
| Remote caching + remote execution | Yes (native) | Cloud-only | Cloud-only | No |
| Fine-grained build graph (down to individual files/targets) | Yes | Package-level | Package-level | Coarse |

The dominant cost of Bazel is **adoption friction**: BUILD file authoring,
learning curve, and less mature ecosystem support for some languages (notably
Node/TS) compared to native tooling. We accept this cost because our primary
pain point is cross-language correctness and build/test speed at scale, which
is exactly what Bazel's hermetic, content-addressed graph is built for.

## Consequences
- New services must ship a `BUILD.bazel` file; `gazelle` auto-generates and
  maintains these for Go, reducing hand-authoring burden.
- We take on operational responsibility for a remote cache (see ADR 0003).
- Onboarding requires ramp-up time on Bazel concepts (targets, visibility,
  hermeticity) — mitigated with `tools/bazel` wrapper scripts and this
  documentation.
- We explicitly defer full TypeScript/Node support (see ADR 0004) rather than
  adopting an immature ruleset under time pressure.
