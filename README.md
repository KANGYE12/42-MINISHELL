# 🐚 Minishell — As beautiful as a shell

![Minishell Banner](https://img.shields.io/badge/42-Minishell-000000?style=for-the-badge&logo=42&logoColor=white)
![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

---

## 📖 Introduction

**Minishell** is a core project of the **42 School** curriculum that consists of recreating a simplified version of the **Bash (Bourne Again SHell)**.  
The goal is to deeply understand how a shell works internally by implementing its essential behavior from scratch using the C programming language.

This project introduces and reinforces fundamental UNIX concepts such as:
- Process creation and synchronization
- File descriptor manipulation
- Environment variable management
- Signal handling
- Lexical analysis and parsing

Minishell behaves similarly to Bash for the supported features and aims to be stable, predictable, and memory-safe.

---

## 🎯 Project Objectives

The main objectives of this project are to:

- Understand how a Unix shell parses and executes commands
- Learn how to manage processes with `fork`, `execve`, and `wait`
- Manipulate file descriptors for pipes and redirections
- Handle Unix signals correctly in both interactive and execution contexts
- Implement shell built-in commands
- Manage environment variables dynamically
- Write robust C code that never crashes and does not leak memory

---

## 👥 Authors

This project was built in collaboration by:

- **[KANGYE12](https://github.com/KANGYE12)**
- **[israelguerreroortiz](https://github.com/israelguerreroortiz)**

---

## ✨ Features

This implementation of Minishell supports the following features:

### 🧭 General
- Interactive prompt using the `readline` library
- Command history navigation
- Behavior closely matching Bash for supported features

### ⚙️ Command Execution
- Execution of system binaries using `$PATH`
- Execution via absolute and relative paths
- Proper exit status propagation

### 🧩 Built-in Commands
The following built-ins are implemented internally (without calling external binaries):

- `echo` (with `-n` option)
- `cd` (with relative, absolute, and `~`)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### 🔁 Pipes
- Pipe operator `|`
- Connects the output of one command to the input of the next
- Supports multiple chained pipes

### 📂 Redirections
- `<` Input redirection
- `>` Output redirection (overwrite)
- `>>` Output redirection (append)
- `<<` Here-document (heredoc)

### 🌱 Environment Variables
- Variable expansion using `$`
- `$?` expands to the last command exit status
- Fully dynamic environment variable list

### 💬 Quotes
- Single quotes `'...'` prevent all expansions
- Double quotes `"..."` allow variable expansion
- Correct handling of spaces and special characters inside quotes

### 🚦 Signals
- `Ctrl-C` (SIGINT): interrupts current command
- `Ctrl-D` (EOF): exits the shell
- `Ctrl-\` (SIGQUIT): ignored in interactive mode
- Signal behavior matches Bash as closely as possible

---

## 🛠️ Installation & Usage

### 📦 Prerequisites

- `gcc` or `clang`
- `make`
- `readline` library  
  - macOS: included by default
  - Linux:
    ```bash
    sudo apt-get install libreadline-dev
    ```

---

### 🔧 Build Instructions

Clone the repository and compile the project:

```bash
git clone https://github.com/KANGYE12/minishell.git minishell
cd minishell
make
