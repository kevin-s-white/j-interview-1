# Project Name
> j-interview-1 - "J" interview assignment.

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)
* [Features](#features)
* [To-do list](#TODO)
* [Status](#status)
* [Inspiration](#inspiration)
* [Contact](#contact)

## General info
I am intentionally omitting company-specific details about this assignment.

## Technologies
* Language: C++ 17
* Target OS: Windows 10 64-bit
* Target Build: x86 or x64, debug or release
* Tools: Microsoft Visual Studio 2019, Community Edition
* Tools download: https://visualstudio.microsoft.com/downloads/
* Thanks to Niels Lohmann for his single-include JSON library for C++!
* JSON library: https://github.com/nlohmann/json

## Setup
* Install Microsoft Visual Studio 2019, Community Edition
* Open Visual Studio 2019
* When prompted, select "Clone a repository"
* Repository: https://github.com/kevin-s-white/j-interview-1
* Select an empty folder for the "Path"
* Clone the repository
* Open the "Action.sln" solution file
* Build the solution
* Test by running \<project-location>\x64\Debug\Action.exe

## Code Examples

```cpp
#include "ActionStats.h"
//...
interview::CActionStats() actionStats;
/*
* Example add-action JSON string: {"action":"jump","time":100.0}
* The "action" tag may be followed by any action name; "jump"
* is merely an example. The "time" tag indicates units of time
* spent on the action.
*/
actionStats.AddAction(jsonString);
std::string statistics = actionStats.GetStats();
```

## Features
List of features
* Thread-safe implementation
* Adds are fully implemented
* Statistics are fully implemented
* Test cases written and passing

## TODO
* In order to avoid complicating the interview solution setup by adding an external test framework, I used a simple-minded run-test
macro for each of the test cases. Find a decent open-source framework and use it.
* Limit the time that the test program may run. While I am certain that the action statistics library will not deadlock as it is currently implemented, future modifications might accidentally break concurrency handling and result in deadlock. If the external test framework automation has the ability to kill a process that runs for "too long," a 5 second run limit on this program is more than enough time for it to complete without trouble.

## Status
Project is: _finished_

## Inspiration
If I get hired by "J", I get to do really cool stuff!

## Contact
Kevin White
<https://www.linkedin.com/in/ksw2018/>
