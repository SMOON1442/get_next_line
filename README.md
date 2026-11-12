# get_next_line

*This project has been created as part of the 42 curriculum by
moboulah.*

## Description

`get_next_line` is a project from the 42 curriculum whose objective is
to implement a function that reads from a file descriptor and returns
one line at a time.

The function must preserve unread data between calls, allowing
successive calls to continue reading from where the previous call
stopped.

The returned line includes the terminating newline (`\n`) when present,
except when the end of the file is reached without a final newline.

The project also introduces several important concepts: - File
descriptors and system calls. - Static variables and their lifetime. -
Dynamic memory allocation. - Buffering and string manipulation. -
Reading files using the `read()` system call.

## Function Prototype

``` c
char    *get_next_line(int fd);
```

-   **Parameter:** `fd` the file descriptor to read from.
-   **Return value:** The next line read, or `NULL` when there is
    nothing left to read or an error occurs.

**Allowed external functions:** `read`, `malloc`, `free`.

## Algorithm

The implementation uses a static variable to preserve unread data
between function calls.

Rather than reading the entire file into memory, the algorithm reads
fixed-size chunks using `read()` and stops as soon as it encounters a
newline or reaches the end of the file.

### 1. Static remainder

A static pointer named `rest` stores the data that has been read but has
not yet been returned.

Since static variables retain their values between function calls, the
function can resume processing the remaining data on its next
invocation.

### 2. Handling previously stored data

The `handle_rest()` function checks whether the stored remainder
contains a newline.

-   If a newline exists, `final_word()` extracts the first line, and the
    remaining characters are saved for the next call.
-   If no newline exists, the stored data is appended to the current
    string, and reading continues.

### 3. Reading and accumulating data

The `fill_str()` function reads chunks of `BUFFER_SIZE` bytes using
`read()`.

Each chunk is appended to the accumulated string.

Reading continues until: - A newline is encountered. - The end of the
file is reached. - A read error occurs.

When a newline is found, the current line is extracted and any
additional data from that read is stored in `rest`.

### 4. Extracting a line

The `final_word()` function allocates a new string containing the
characters up to and including the first newline.

This ensures that each successful call returns exactly one line.

### 5. Memory management

The implementation uses dynamically allocated strings to accumulate data
and preserve the remainder.

The `append_str()` function combines two strings using `ft_strjoin()`
and releases the previous accumulated string.

The caller is responsible for freeing the line returned by
`get_next_line()`.

### Why this algorithm?

The main challenge is that `read()` operates on bytes rather than lines.
A single read operation may retrieve part of a line, one complete line,
or several lines.

Keeping the unread portion in a static variable solves this problem
without requiring the entire file to be loaded into memory.

The algorithm also adapts to different `BUFFER_SIZE` values:

-   **BUFFER_SIZE = 1:** Reads one byte at a time until a newline or EOF
    is encountered.
-   **Large BUFFER_SIZE:** May retrieve multiple lines in one read,
    preserving the additional data for subsequent calls.
-   **BUFFER_SIZE larger than the file:** Reads the available content
    and returns it without requiring multiple read operations.

## Bonus Part

The bonus implementation extends the mandatory version to support
multiple file descriptors simultaneously.

Instead of using a single static pointer:

``` c
static char    *rest;
```

The bonus uses an array of static pointers:

``` c
static char    *rest[OPEN_MAX];
```

Each file descriptor has its own remainder, indexed by its descriptor
number.

This allows multiple files to be read independently, even when calls to
`get_next_line()` are interleaved.

For example, reading from file descriptor 3 does not overwrite the
unread data associated with file descriptor 4.

The main reading algorithm and helper functions remain essentially the
same. The primary difference is how the remainder is stored and
accessed.

## Instructions

### Compilation

Compile the mandatory implementation using:

``` bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
get_next_line.c get_next_line_utils.c main.c
```

You can change the buffer size to test different configurations:

``` bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 \
get_next_line.c get_next_line_utils.c main.c
```

The project must also compile without explicitly defining `BUFFER_SIZE`.
A default value should therefore be provided in the header.

### Usage

Call `get_next_line()` repeatedly until it returns `NULL`:

``` c
char    *line;

while ((line = get_next_line(fd)) != NULL)
{
    /* Process the line. */
    free(line);
}
```

The returned line must be freed by the caller to avoid memory leaks.

### Project Files

**Mandatory:**

  File                      Purpose
  ------------------------- ------------------------------------------
  `get_next_line.c`         Main implementation and reading logic
  `get_next_line_utils.c`   String manipulation helper functions
  `get_next_line.h`         Function prototypes and required headers

**Bonus:**

  File                            Purpose
  ------------------------------- -------------------------------------------
  `get_next_line_bonus.c`         Multiple file descriptor implementation
  `get_next_line_utils_bonus.c`   Bonus utility functions
  `get_next_line_bonus.h`         Bonus function prototypes and definitions

## Constraints

-   `libft` is forbidden.
-   `lseek()` is forbidden.
-   Global variables are forbidden.
-   Only `read()`, `malloc()`, and `free()` are allowed as external
    functions.
-   The implementation must work with different `BUFFER_SIZE` values.
-   Reading binary files is outside the required behavior.

## Resources

The following resources were consulted to understand the concepts
involved in this project:

-   [Handling a file by its descriptor in C ---
    Codequoi](https://www.codequoi.com/en/handling-a-file-by-its-descriptor-in-c/#what-is-a-file-descriptor)
    --- Introduction to file descriptors and their usage in C.
-   [File descriptors and handles --- The Node
    Book](https://www.thenodebook.com/file-system/file-descriptors-and-handles#the-descriptor-table)
    --- Explains descriptor tables and the underlying kernel structures.
-   *The Linux Programming Interface* --- Michael Kerrisk --- Reference
    book covering file descriptors, system calls, and file-system
    concepts.
-   [UNIX System Calls](http://www.di.uevora.pt/~lmr/syscalls.html) ---
    Overview of system calls and their interaction with the operating
    system.
-   [The Linux System Administrator's Guide: The Buffer
    Cache](https://tldp.org/LDP/sag/html/buffer-cache.html) ---
    Introduction to buffering and disk I/O.
-   [Static Variables in C ---
    Codecademy](https://www.codecademy.com/resources/docs/c/static-variables)
    --- Explanation of static variables and their lifetime.
