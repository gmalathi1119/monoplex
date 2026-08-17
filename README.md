# monorepo

A polyglot monorepo (Go, Python, C++, Java) built with [Bazel](https://bazel.build)
and [bzlmod](https://bazel.build/external/overview#bzlmod), designed as a
reference architecture for consolidating multi-language services into a
single, hermetically-buildable, incrementally-testable build graph.

## Why this exists

Most "monorepo + Bazel" examples online are toy hello-worlds with no
documented reasoning. This repo tries to do the opposite: every non-trivial
decision — monorepo vs polyrepo, bzlmod vs WORKSPACE, caching vs remote
execution, why TypeScript isn't in here yet — is written down as an
[Architecture Decision Record](docs/adr/), including the trade-offs and what
was explicitly rejected and why.

## Structure

```
.
├── MODULE.bazel          # bzlmod dependency graph (Go, Python, C++, Java)
├── .bazelrc               # build/test flags, CI config, cache config
├── go/                    # Go services (rules_go + gazelle)
├── python/                # Python services (rules_python, hash-locked deps)
├── cpp/                   # C++ services (native cc_* rules + googletest)
├── java/                  # Java services (rules_java + rules_jvm_external)
├── docs/adr/              # Architecture Decision Records
└── .github/workflows/     # CI: build //... and test //... on every PR
```

Each language directory follows the same pattern: a `BUILD.bazel` per
target, unit tests colocated with source, and no cross-language magic beyond
what's declared explicitly in `MODULE.bazel`.

## Getting started

```bash
# Bazelisk reads .bazelversion and downloads the pinned Bazel release
curl -fsSL https://github.com/bazelbuild/bazelisk/releases/latest/download/bazelisk-linux-amd64 -o /usr/local/bin/bazel
chmod +x /usr/local/bin/bazel

# Build and test everything
tools/bazel/build_all.sh

# Or target a single service
bazel run //go/cmd/hello
bazel test //cpp/hello:greeter_test
```

## Architecture Decision Records

| ADR | Decision |
|---|---|
| [0001](docs/adr/0001-monorepo-and-build-system.md) | Monorepo + Bazel over polyrepo |
| [0002](docs/adr/0002-bzlmod-over-workspace.md) | bzlmod over legacy WORKSPACE |
| [0003](docs/adr/0003-remote-caching-strategy.md) | Remote caching now, remote execution later |
| [0004](docs/adr/0004-typescript-deferred.md) | TypeScript/Node deliberately out of scope for v0.1 |

## Roadmap

- [ ] `bazel-diff`-based affected-target CI (only build/test what a PR touches)
- [ ] Remote cache backend (`bazel-remote` over object storage)
- [ ] `aspect_rules_ts` spike, gated per ADR 0004
- [ ] Container image rules (`rules_oci`) per service for deployment
- [ ] SBOM generation + artifact signing on release builds

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md). New services should include a
`BUILD.bazel`, unit tests, and — for anything architecturally non-obvious —
an ADR under `docs/adr/`.

## License

[Apache 2.0](LICENSE)
