# minishell

A Unix shell implementation built in C as part of the 42 school curriculum. minishell replicates core bash behavior — from tokenizing and parsing input into an AST, to executing commands with full support for piping, redirections, and built-ins.

---

## Features

- **Interactive prompt** with command history (via readline)
- **Command execution** — resolves binaries through `PATH`
- **Pipelines** — `cmd1 | cmd2 | cmd3`
- **Logical operators** — `&&` and `||` with correct short-circuit evaluation
- **Grouped commands** — `(cmd1 && cmd2) | cmd3`
- **Redirections**
  - Input `<`
  - Output `>`
  - Append `>>`
  - Heredoc `<<`
- **Environment variable expansion** — `$VAR`, `$?`
- **Wildcard expansion** — `*` glob matching in the current directory
- **Quote handling** — single quotes `'` and double quotes `"`
- **Signal handling** — `Ctrl+C`, `Ctrl+D`, `Ctrl+\` behave as in bash

### Built-ins

| Command  | Description                          |
|----------|--------------------------------------|
| `echo`   | Print arguments (`-n` flag supported)|
| `cd`     | Change directory                     |
| `pwd`    | Print working directory              |
| `export` | Set/display environment variables    |
| `unset`  | Remove environment variables         |
| `env`    | Print environment                    |
| `exit`   | Exit the shell with optional status  |

---

## Project Structure

```
minishell/
├── includes/
│   ├── minishell.h       # Main header
│   ├── parser.h          # Parser/tokenizer types and prototypes
│   └── executor.h        # Executor prototypes
├── src/
│   ├── main.c
│   ├── shell/            # Shell init utilities
│   ├── signals/          # Signal handlers
│   ├── parser/
│   │   ├── tokenizer/    # Lexer, tokenizer, validator
│   │   ├── expander/     # Variable/wildcard expansion
│   │   ├── ast/          # AST construction
│   │   └── parser.c      # Parser entry point
│   ├── executor/
│   │   ├── exec/         # Command & pipeline execution
│   │   └── redirections/ # Redirect, heredoc handling
│   └── built-in/         # Built-in command implementations
└── lib/
    └── libft/            # Custom C standard library
```

---

## Build & Run

### Requirements

- `gcc` or `cc`
- `readline` library
- GNU `make`

### Compile

```bash
make
```

### Run

```bash
./minishell
```

### Clean

```bash
make clean    # Remove object files
make fclean   # Remove objects + binary
make re       # Full rebuild
```

---

## Authors

- **mzelouan** — [mzelouan@student.42.fr](mailto:mzelouan@student.42.fr)
- **amasdouq** — [amasdouq@student.42.fr](mailto:amasdouq@student.42.fr)
