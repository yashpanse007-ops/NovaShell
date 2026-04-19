\# NovaShell: POSIX-Compliant Command Line Interpreter



NovaShell is a custom Linux shell developed to explore low-level systems programming. It implements core OS concepts including process management, inter-process communication (IPC), and defensive programming.



\## 🚀 Technical Highlights

\- \*\*Process Lifecycle:\*\* Implements the `fork-exec-wait` model to manage system tasks.

\- \*\*IPC Architecture:\*\* Features a Producer-Consumer implementation using synchronized `pipe` system calls.

\- \*\*Safety Mechanisms:\*\* Built-in detection for \*\*Overflow\*\* (buffer limits) and \*\*Underflow\*\* (empty pipes).

\- \*\*POSIX Compliant:\*\* Environment-agnostic logic designed for Ubuntu, WSL, or any POSIX-standard system.



\## 🛠 Features

\- Custom interactive REPL with a robust tokenizer.

\- Real-time PID and PPID tracking to visualize process hierarchies.

\- Integrated IPC test suite (`pc\_test`).



\## 💻 Usage

Compile using `g++` to link the C++ standard libraries:

```bash

g++ novashell.cpp -o novashell

./novashell

