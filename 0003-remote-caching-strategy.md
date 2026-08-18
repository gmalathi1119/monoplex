# ADR 0003: Remote cache before remote execution

## Status
Proposed

## Context
As the monorepo grows, CI wall-clock time and local developer build times
both depend heavily on cache hit rate. Bazel supports two scaling paths that
are often conflated:

1. **Remote caching** — build/test *outputs* are content-addressed and
   shared across machines (CI workers, laptops). A target only needs to be
   *executed* by one machine, ever, for a given input hash.
2. **Remote execution (RBE)** — build/test *actions themselves* run on a
   remote worker pool, enabling massive parallelism beyond one machine's
   core count, and a consistent execution environment.

RBE is significantly higher operational cost: it requires a
container-hermetic toolchain for every language (no reliance on the host's
compiler/interpreter), a worker pool to run and scale, and substantially more
debugging complexity when actions fail remotely.

## Decision
Adopt **remote caching only** as the first milestone. Defer RBE until:
- monorepo CI wall-clock time regularly exceeds a defined SLO (proposed:
  p90 > 10 minutes for a typical single-service change), and
- toolchains for all active languages are verified fully hermetic
  (no ambient host dependency), a prerequisite for correct RBE.

Concretely: CI populates the remote cache on every merge to main
(`build --remote_upload_local_results=true`, via `--config=ci`). Local
developer builds read from that cache but do not upload
(`--noremote_upload_local_results`), avoiding cache pollution from
uncommitted/experimental local changes.

## Consequences
- Fast path: a developer syncing main and building a target already built in
  CI gets a near-instant cache hit instead of a full rebuild.
- We must operate a cache backend (candidates: self-hosted `bazel-remote`
  over GCS/S3, or a managed offering). Cost and cache eviction policy are
  tracked separately in the infra cost model, not detailed here.
- This defers, but does not solve, CI parallelism limits bound by a single
  worker's core count. That trade-off is accepted for now and revisited per
  the SLO trigger above.
