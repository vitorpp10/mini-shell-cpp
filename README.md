# custom c++ shell

this project is a mini linux terminal built from scratch in c++. instead of just writing code, the goal here was to understand how an operating system handles processes under the hood.

## architecture & core concepts

### the repl cycle (read-eval-print loop)

the terminal runs on an infinite loop. at the start of every iteration, it clears out "zombie" processes (background processes that have finished) using `waitpid` with the `wnohang` flag, ensuring the system doesn't get overloaded.

### tokenization and parsing

the user input is read as a single string and then split (tokenized) into an array of arguments. the code specifically looks for the `&` character at the end of the command to determine if it should run in the background.

### process management (fork-and-exec)

to run external commands, the shell uses the fork-and-exec pattern:

* **fork**: the parent process duplicates itself.
* **child**: uses `execve` to replace its memory image with the requested command.
* **parent**: decides whether to wait for the child (`waitpid`) or immediately free the terminal for the user (if it's a background process).

### built-ins (internal commands)

commands like `cd` and `exit` are handled directly by the shell. they need to alter the state of the parent process (like changing the current directory via `chdir`), which a child process wouldn't be able to do.

## technical decisions

### converting strings to c-strings

used `const_cast<char*>` to convert `std::string` tokens into the array of `char*` required by the `execve` function, bridging the gap between high-level c++ and low-level linux c apis.

### handling defunct processes

the `waitpid(-1, null, wnohang)` call at the very beginning of the loop is crucial to collect background processes that have finished, preventing resource leaks.

## how to run

compile the code using `g++`:

```bash
g++ main.cpp -o shell
```

run the executable:

```bash
./shell
```

## tech stack

* **language**: c++
* **apis**: linux posix system calls (`<unistd.h>`, `<sys/wait.h>`, `<sys/types.h>`)
