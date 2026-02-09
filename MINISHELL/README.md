*This project has been created as part of the 42 curriculum by jeid and bmazraan.*

# Minishell

## Description

**Minishell** is a simple shell implementation written in C. The goal of this project is to recreate some of the core functionality of bash, providing an interactive command-line interface that can interpret and execute user commands.

This project explores fundamental concepts of Unix systems programming, including:
- Process creation and management using `fork()` and `execve()`
- File descriptor manipulation for input/output redirections
- Inter-process communication through pipes
- Signal handling for proper terminal behavior
- Environment variable management
- Lexical analysis and parsing of user input

The shell displays a prompt, waits for user input, and executes commands while handling special characters, quotes, redirections, and pipes similar to how bash operates.

### Features

- **Command Execution**: Search and launch executables based on the `PATH` variable or using relative/absolute paths
- **Built-in Commands**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Redirections**: Input (`<`), Output (`>`), Append (`>>`), Here-doc (`<<`)
- **Pipes**: Connect multiple commands using `|`
- **Quote Handling**: Single quotes (`'`) and double quotes (`"`)
- **Environment Variables**: Expansion of `$VAR` and `$?` (last exit status)
- **Signal Handling**: Proper handling of `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`
- **Command History**: Navigate through previous commands using arrow keys

## Instructions

### Prerequisites

- CC compiler
- Make
- Readline library

**On Debian/Ubuntu:**
```bash
sudo apt-get install libreadline-dev
```

**On macOS with Homebrew:**
```bash
brew install readline
```

### Compilation

Clone the repository and compile the project:

```bash
git clone <repository-url> minishell
cd minishell
make
```

### Makefile Commands

| Command       | Description                              |
|---------------|------------------------------------------|
| `make`        | Compile the project                      |
| `make clean`  | Remove object files                      |
| `make fclean` | Remove object files and executable       |
| `make re`     | Recompile the entire project             |

### Execution

Launch the shell:

```bash
./minishell
```

You will see the custom prompt:
```
42-minishell  ➜
```

### Usage Examples

```bashassistance and error message interpretation
# Simple command
42-minishell  ➜ ls -la

# Piping commands
42-minishell  ➜ cat file.txt | grep "search" | wc -l

# Output redirection
42-minishell  ➜ echo "Hello World" > output.txt

# Input redirection
42-minishell  ➜ cat < input.txt

# Append redirection
42-minishell  ➜ echo "New line" >> output.txt

# Here-doc
42-minishell  ➜ cat << EOF
> Hello
> World
> EOF

# Environment variables
42-minishell  ➜ echo $HOME
42-minishell  ➜ echo "Exit status: $?"

# Export and use variables
42-minishell  ➜ export MY_VAR="Hello"
42-minishell  ➜ echo $MY_VAR

# Exit the shell
42-minishell  ➜ exit
```

### Built-in Commands

| Command  | Description                                    |
|----------|------------------------------------------------|
| `echo`   | Display a line of text (supports `-n` flag)    |
| `cd`     | Change the current directory                   |
| `pwd`    | Print working directory                        |
| `export` | Set environment variables                      |
| `unset`  | Unset environment variables                    |
| `env`    | Display the environment                        |
| `exit`   | Exit the shell                                 |

## Resources
	
### Documentation & References

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) - Official GNU Bash documentation
- [The Open Group Base Specifications - Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) - POSIX shell specification
- [Readline Library Documentation](https://tiswww.case.edu/php/chet/readline/rltop.html) - GNU Readline library documentation
- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf) - Systems programming guide
- Linux man pages: `fork(2)`, `execve(2)`, `pipe(2)`, `dup2(2)`, `wait(2)`, `signal(2)`

### AI Usage Disclosure

AI tools (such as GitHub Copilot) were used in the development of this project for the following tasks:
- Generating documentation and README content
- Code suggestions for repetitive patterns
- Debugging assistance and error message interpretation

All AI-generated code was reviewed, understood, and adapted to fit the project requirements and 42 coding standards (Norminette).
