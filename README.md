# RIE++
Copyright (C) 2026 Ronald Nidera\
Licensed under the GNU GPLv3

A byte-compiled general-purpose programming language compiler written in C23 and C++23.
With its libraries (soon to be!) written in the language itself.


## What's this?
This is the compiler for RIE++, right now it is currently in the stage where I am
writing the frontend, and the tool only uses basic one-file-at-a-time lexing and
it doesn't even compile anything yet, it just tokenizes stuff and prints values to  
stdout. But soon enough, once we hit v0.01, we will be implementing some primitive  
language features, such as printing, variable conversions (like string to integer),
exceptions, etc.

## Beyond UNIX-like systems
Currently, there is no guarantee that it works on Windows, but porting attempts  
will start as soon as we hit v0.20 when the language, the features and its tools are
mature enough.

## Goals?
When we reach v0.20, porting attempts to Windows are going to start
once tools and the language is stable enough.
At v0.10 features such as type-inference,
At v0.05, I will add imports and file visibility (public v. private)
