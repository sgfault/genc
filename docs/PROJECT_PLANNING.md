# GenC Build System - Project Review & Roadmap

## Project Overview

GenC is a build system and package manager for C projects, inspired by modern tools like Cargo (Rust) and npm (Node.js). The goal is to simplify C project management, dependency handling, and compilation.

---

## Current Architecture Review

### Strengths

1. **Clean separation of concerns** - Distinct modules for commands, manifest handling, parsing, and file system operations
2. **Command pattern** - Using `CommandType` enum and function pointers provides good extensibility
3. **TOML for manifest** - Excellent choice, human-readable and widely adopted (Cargo, Poetry, etc.)
4. **Core commands identified** - `build`, `init`, `new` cover the essential workflows
5. **Updated string handling** - `Str` struct now properly uses `char*` and `size` (good!)
6. **Improved file_system API** - Functions now have proper signatures with paths and content parameters

### Areas Needing Attention

#### 1. Command Function Signature (`command.h:16`)
```c
typedef CommandResult (*CommandFunction)();  // Missing parameters
```
- Commands will need context (manifest, arguments, working directory)
- **Suggestion**: `CommandResult (*CommandFunction)(Context* ctx, int argc, char** argv)`

#### 2. Empty Structures

**Manifest struct** needs fields for:
- Project metadata (name, version, authors)
- Dependencies list
- Build configuration
- Compiler flags
- Output specifications

**CommandResult struct** should include:
- Success/failure status
- Error messages
- Exit code

#### 3. Missing Critical Components

**Not yet defined:**
- Error handling strategy (return codes vs Result types)
- Dependency structure (name, version, source)
- Build context/configuration
- Logging utilities
- TOML parser integration (consider using an existing library like `tomlc99`)

#### 4. File System API

Current signatures are good, but consider:
- Return types for error handling (currently all `void`)
- `read_file` should return content or populate a buffer
- Consider using `FILE*` abstractions or higher-level wrappers

---

## Development Roadmap

### Phase 1: Foundation (Start Here)

| Priority | Task | Description | Estimated Effort | Assignne |
|----------|------|-------------|------------------|----------|
| 1 | Argument parsing | Implement CLI argument parser (`genc <command> [args]`) | Small | Aymane |
| 2 | Basic file operations | Implement `read_file`, `write_to_file`, `create_file` | Small | Aymane | 
| 3 | Project templates | Define default project structure for `genc new` | Small | Sidati |
| 4 | `genc new` command | Create new project with basic `genc.toml` template | Medium | Sidati |
| 5 | `genc init` command | Initialize `genc.toml` in existing directory | Small | Ayamne |
| 6 | Basic manifest structure | Define minimal `Manifest` struct (name, version) | Small | Sidati | 
| 7 | TOML parser integration | Integrate `tomlc99` or similar library | Medium | Sidati |
| 8 | Parse basic manifest | Read and parse simple `genc.toml` files | Medium | Aymane |

---
### Phase 2: Core Build System

| Priority | Task | Description | Estimated Effort |
|----------|------|-------------|------------------|
| 9 | Define build configuration | Add compiler flags, output paths to manifest | Small | Sidati |
| 10 | Source file discovery | Implement `walk_dir` to find `.c` files | Medium | Aymane |
| 11 | Simple compilation | Compile single-file projects (no deps) | Medium | Sidati |
| 12 | Linking | Link object files into executable | Medium | Sidati |
| 13 | `genc build` command | Implement full build workflow | Medium | Aymane |
| 14 | Build output directory | Create `bin/debug` and `target/release` | Small | Aymane |

---
### Phase 3: Basic Dependency Support

| Priority | Task | Description | Estimated Effort |
|----------|------|-------------|------------------|
| 16 | Dependency structure | Define `Dependency` struct (name, git URL) | Small |
| 17 | Git clone integration | Download deps from Git repositories | Medium |
| 18 | Local dependencies | Support local path dependencies | Small |
| 19 | Include path management | Add dependency headers to compilation | Medium |
| 20 | Link dependencies | Link against dependency libraries | Medium |
| 21 | Cache directory | Create `.genc/deps` for dependency storage | Small |

---

## Dream Features (Advanced/Future)

These are complex features to implement later once the core system is stable:

### High Complexity Features

1. **Lock file (`genc.lock`)**
   - Pin exact dependency versions
   - Ensure reproducible builds across machines
   - Requires version resolution algorithm

2. **Dependency graph resolution**
   - Build complete dependency tree
   - Detect circular dependencies
   - Handle version conflicts
   - Topological sort for build order

3. **Incremental compilation**
   - Track file modification timestamps
   - Rebuild only changed files and dependents
   - Maintain dependency graph of source files
   - Significant performance optimization for large projects

4. **Semantic versioning**
   - Parse and compare semver strings (1.2.3, ^1.0, ~2.1)
   - Version constraint resolution
   - Automatic minor/patch updates

5. **Central package registry**
   - Host packages on remote server
   - Package publishing workflow
   - Authentication and package ownership
   - Search functionality

6. **Parallel compilation**
   - Multi-threaded build system
   - Job scheduling and worker pools
   - Requires careful synchronization

7. **Cross-compilation support**
   - Target different architectures
   - Toolchain management
   - Platform-specific dependencies

8. **Build caching**
   - Cache compiled artifacts
   - Share caches across projects
   - Content-addressable storage

9. **Custom build scripts**
   - Pre/post-build hooks
   - Custom compilation steps
   - Code generation support

10. **Workspace support**
    - Monorepo with multiple projects
    - Shared dependencies across workspace
    - Workspace-level commands

11. **Testing framework integration**
    - `genc test` command
    - Discover and run test files
    - Coverage reporting

12. **Documentation generation**
    - Extract comments into docs
    - Generate API documentation
    - `genc doc` command

13. **Binary caching**
    - Download pre-compiled dependencies
    - Remote cache support
    - Drastically faster builds

14. **Dependency vendoring**
    - Copy all dependencies into project
    - Offline builds
    - Version control friendly

---

## Recommended Project Structure (for projects using GenC)

```
myproject/
├── genc.toml           # Manifest file
├── genc.lock          # Lock file (future)
├── src/               # Source files
│   ├── main.c
│   └── ...
├── include/           # Public headers
│   └── myproject.h
├── tests/             # Test files (future)
│   └── test_main.c
├── target/            # Build output (gitignored)
│   ├── debug/
│   │   └── myproject
│   └── release/
│       └── myproject
└── .genc/             # GenC metadata (gitignored)
    └── deps/          # Downloaded dependencies
```

---

## Sample `genc.toml` Format

```toml
[project]
name = "myproject"
version = "0.1.0"
authors = ["Your Name <you@example.com>"]

[build]
compiler = "gcc"
flags = ["-Wall", "-Wextra", "-std=c99"]
output = "myproject"

[dependencies]
# Local dependency
mylib = { path = "../mylib" }

# Git dependency (future)
json = { git = "https://github.com/user/json-c.git", tag = "v1.0" }

# Registry dependency (far future)
# sqlite = "3.40.0"

[profiles.debug]
flags = ["-g", "-O0"]

[profiles.release]
flags = ["-O3", "-DNDEBUG"]
```

---

## Next Steps

1. Start with **Phase 1, Task 1**: Implement basic argument parsing
2. Build incrementally - get each command working before moving to the next
3. Focus on the "happy path" first (no error handling initially)
4. Add error handling once basic functionality works
5. Write simple test projects to validate each feature

## Notes

- Consider using existing libraries for TOML parsing (`tomlc99`, `toml-c`)
- Study Cargo's architecture for inspiration
- Keep things simple initially - avoid premature optimization
- Manual dependency management is fine for v1.0 (just git clone)
- Don't worry about lock files or version resolution in early stages

---

**Last Updated**: 2026-01-08
