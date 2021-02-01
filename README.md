# hcs-logger

This C++17 library is very much like log4c++. Yet, hcs-logger strives to be minimal and concise 
with no further dependencies.

In general, everything stays out of the way. On default only `Critical`
and `Warning` are enabled. So this is totally sufficient:
```c++
#include <headcode/logger/logger.hpp>

using namespace headcode::logger;

int main(int argc, char ** argv) {
    Warning{} << "Hello World!";
    return 0;
}
```

To enable debug messages right from the start:
```c++
#include <headcode/logger/logger.hpp>

using namespace headcode::logger;

int main(int argc, char ** argv) {
    Logger::GetLogger()->SetBarrier(Level::kDebug);
    Debug{} << "Hello World!";
    return 0;
}
```

That said, `hcs-logger` can do some more things:

Features:

* Differentiate between log sources or systems and turn each on or off

E.g. you might have a system "network" and "database" in your code base. Seen
from hcs-logger these are all different `Logger` instances.

Example:
```c++
// turn off all messages from "network"
headcode::logger::GetLogger("network")->SetBarrier(headcode::logger::Level::kSilent);
// ... but turn all message from "database" on:
headcode::logger::GetLogger("database")->SetBarrier(headcode::logger::Level::kDebug);
...
// This will not be shown
headcode::logger::Debug("network") << "Got a debug message while doing network stuff...";
// This will be printed
headcode::logger::Debug("database") << "Doing database stuff now...";
```

* Loggers (aka logging subsystems) are hierarchical ordered with the dot `.` delimiter.

Example: turn off each and every message but warnings and critical, yet also debug for a subsystem:
```c++
headcode::logger::GetLogger()->SetBarrier(headcode::logger::Level::kWarning);
headcode::logger::GetLogger("app.network")->SetBarrier(headcode::logger::Level::kSilent);
headcode::logger::GetLogger("app.network.incoming")->SetBarrier(headcode::logger::Level::kDebug);
```

* Loggers can have any numbers of sinks associated with additional barriers.

Example: different log levels for different outputs:
```c++
// logfile: "my_app.log" (but only info, warnings and errors)
auto file_sink = std::make_shared<headcode::logger::FileSink>("my_app.log");
file_sink->SetBarrier(headcode::logger::Level::kInfo);
// and log to stderr too (including debug stuff)
auto console_sink = std::make_shared<headcode::logger::ConsoleSink>();
console_sink->SetBarrier(headcode::logger::Level::kDebug);

// set sink: only 1 sink
headcode::logger::GetLogger()->SetSink(file_sink);
// add sink: add another one
headcode::logger::GetLogger()->AddSink(console_sink);
// set allowed maximum log level, before passing on to sinks
headcode::logger::GetLogger()->SetBarrier(headcode::logger::Level::kDebug);
```

* Different formatting for different sinks (e.g. console, file, syslog, ...)



## Philosophy

All `headcode.space` software follows these directives:

1. Provide very **high quality** C++ software. That's the topmost goal.

2. Provide appealing C++ software, i.e. software which is **easily read and understood**. 
   Second goal.

3. Provide software with **super minimal public interfaces**. I try to really provide the absolute 
   minimum without any bloat. The users of the software should only include the main header file 
   (e.g. `#include <headcode/logger/logger.hpp>`) and nothing else. These are self-contained and 
   rely only on C++ standard headers. No additional 3rd party headers required. Third goal.
  
4. Provide libraries and binaries with **little to no runtime dependencies**. Yet, linkage might
   require additional libraries (sometimes from `headcode.space` sometimes else). Whenever
   possible I strive to go for the *static libraries*, because a) the resulting binaries when
   linked against these libraries have little to no runtime dependency and can be installed 
   directly with a mere `copy` and b) they are smaller too, since the linkers tend to strip off 
   stuff which is not needed.

I'm by no means perfect. There's always room for improvements and there are sure still bugs.
If you have any suggestions please drop in an email at https://gitlab.com/headcode.space/logger/-/issues.


## The API

**TBD**


## Usage example

```c++
#include <headcode/logger/logger.hpp>

using namespace headcode::logger;

void foo() {
    Debug{} << "This is a debug message of foo, but routed to the main logger.";
    Debug{"foo"} << "This is a debug message of foo, yet addressed to the 'foo' logger.";
    Debug{"foo.child"} << "This is a debug message of foo.child.";
}


void bar() {
    Critical{} << "bar() made a critical. The number is: " << 42;
    Warn{} << "Pressure raising... to " << 3.1415;
    Info{} << "All ok.";
    Debug{} << "Object at 1337dead has an invalid value of 0xgibberish.";
}


int main(int argc, char ** argv) {

    // only critical and warnings of bar() are pushed, foo() remains silent
    foo();
    bar();
    
    // activate also debug messages: raise the barrier to debug ==> everything is pushed
    Logger::GetLogger()->SetBarrier(Level::kDebug);
    
    foo();
    bar();
    
    // now turn off ALL messages associated with logger "foo" but keep "foo.child" alive for debug.
    Logger::GetLogger("foo")->SetBarrier(Level::kSilent);
    Logger::GetLogger("foo.child")->SetBarrier(Level::kDebug);
    
    foo();
    bar();
    
    // tell 'foo.child' to use the same as 'foo' (which is kSilent) again.
    Logger::GetLogger("foo.child")->SetBarrier(Level::kUndefined);

    foo();
    bar();

    return 0;
}
```

**TBD**

## Project layout

```
.
├── 3rd                         3rd party buildtime libraries needed (likely as git submodules).
├── cmake                       CMake additional files.
├── include                     Public header files. Add the path to this folder to your C++ search path.
│   └── headcode                
│       └── logger              Include this: <headcode/logger/logger.hpp>      <== This is it.
├── src                         Main sources.
│   └── lib                     Static libhcs-logger.a sources.
├── test                        Tests.
│   ├── benchmark               Benchmark tests.
│   ├── shared                  Shared test data files.
│   └── unit                    Unit tests.     <== Read this for documentation and How-Tos.
├── tools                       Various tools for run-time or build-time.
│   ├── docker                  Docker builder image definitions: Dockerfiles for various platforms to build.
│   └── package                 Package related files.
├── Changes.md                  Changes file.
├── CMakeLists.txt              The overall CMakeLists.txt.
├── Doxyfile                    Doxgen API documentation configuration.
├── LICENSE.txt                 The software license.
└── README.md                   This file.
```

## Build

### Dependencies

- cmake
- gcc (with g++) or clang (witch clang++)
- git
- make
- doxygen (with graphviz)
- [googletest](https://github.com/google/googletest) (as git submodule)

When cloning this project execute the following to clone submodules as well:

```bash
$ git submodule init
$ git submodule update
```

or simply clone with the `--recurse-submodule` option:
```bash
$ git clone --recurse-submodules
```

#### Native build

hcs-logger is a [cmake](https://cmake.org) project with out-of-source builds in
a dedicated folder, usually labeled "build".

```bash
$ mkdir build && cd build
$ cmake ..
$ make
```

## Test

After compilation run ctest
```bash
$ cd build
$ ctest
```
Or
```bash
$ cd build
$ make test
```

_Note: Please check the [test files](test/unit/)  for documentation. 
The tests are easy to read and tell you how the code is intended to be used._ 

### Test Coverage

You may also run in-deep test coverage profiling. For this, you have to turn on profiling mode:
```bash
$ cd build
$ cmake -D PROFILING_MODE_ENABLED=on ..
```

Then compile as usual and run the tests. After the tests make the `run-gcovr` target: 
```bash
$ make test
$ make run-gcovr
```

This will give you the test coverage on stdout as well as:
* `gcovr-coverage.info`:  this is the coverage info file created by gcovr
* `gcovr-report.xml`: this is the gcovr report file in xml
* `coverge-html`: this is the folder in which detailed html info of collected coverage resides
  (open up the file `coverage-html/index.html` in a browser of your choice)

in the build folder.


## Installable package creation

This project supports the creation of `DEB` and `RPM` files. This is done by specifying
the `CPACK_GENERATOR` while configuring the project.

To create an installable `DEB`:
```bash
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=Release -D CPACK_GENERATOR=DEB ..
...
$ make
...
$ make package
```

To create an installable `RPM`:
```bash
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=Release -D CPACK_GENERATOR=RPM ..
...
$ make
...
$ make package
```


## Notable guidelines

* Coding Guidelines: https://google.github.io/styleguide/cppguide.html
* How (not) to write git commit messages: https://www.codelord.net/2015/03/16/bad-commit-messages-hall-of-shame/
* How to version your software: https://semver.org/
* How to write a clever "Changes" file: https://keepachangelog.com/en/1.0.0/
* Folder Convention: https://github.com/KriaSoft/Folder-Structure-Conventions

---

Copyright (C) 2021 headcode.space e.U.  
Oliver Maurhart <info@headcode.space>  
[https://headcode.space](https://www.headcode.space)  
