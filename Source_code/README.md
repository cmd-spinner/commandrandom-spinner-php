# Source code used in the paper

This page contains the code for the analysis and system call hooking modules.

## Analysis Tools

<!--![](https://i.ibb.co/rZsYKjX/overview-1.jpg)-->

Spinner conducts bidirectional flow analysis on the target applications to do the `Command Composition Analysis` and `Program Instrumentation`. Forward flow analysis begins (1) from trusted sources to identify variables holding trusted commands and (2) from untrusted sources to identify variables that are not relevant to commands. Backward flow analysis begins from the arguments passed to command execution APIs (e.g., system()).

We use both the forward flow analysis and backward flow analysis (Section 4) to reduce over and under-approximations. The bidirectional analysis merges results from forward and backward analyses together to improve the analysis accuracy.

### Static Analysis

We develop the Spinner's static analysis tool based on the below 3 existing tools.

#### Psalm

We reuse part of the Psalm's code for the static analysis.

- Target applications need to be scanned and configured by Psalm in the same way.

> Original source: **[Link](https://psalm.dev/)**.

#### PHP-CFG

PHP implementation of a control flow graph (CFG) with instructions in the SSA (Single Static Assignment) form.

> Original source: **[Link](https://github.com/ircmaxell/php-cfg)**.

#### PHP-Parser

A PHP parser written in PHP. We use it to simplify static code analysis and manipulation.

> Original source: **[Link](https://github.com/nikic/PHP-Parser)**.

## Code for API hook

Spinner hooks API calls. This folder contains code used by Spinner to hook library calls.

Detailed code can be found on **[`Hooklib`](syscall_hooklib)**.
