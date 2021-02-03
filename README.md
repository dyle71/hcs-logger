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

The API is really small. There are

* `Event`: That's the prime log event.
* `Logger`: Objects of this class are the "administrators" of events. They examine events and redirect them to
  a number of `Sink`s.
* `Sink`: This is anything a event goes to. While working on an event, each `Sink` object uses a 
* `Formatter`: This class prepares the final output.


### Events

An event has a message, a log level and a Logger associated. Log levels define, if the event will
make it through the logger barriers and will finally end up in some log message.

Events are stream objects, i.e.

```c++
#include <headcode/logger/logger.hpp>
using namespace headcode::logger;
...
Event{Level::kDebug} << "This is a number: " << 42 << ". See?";
```
Anything you can do to `std::ostream` you can do to Events. However, the final message of
the event will be written, when the object gets out of scope.

There are these log levels:

* Debug (4): debug event.
* Info (3): some information to be shown to the user.
* Warning (2): an action could not be performed, but the situation should not happen under normal conditions. 
  Yet all is still fine. For now.
* Critical (1): an action has produces an invalid state. Probably loss of data ahead. Panic!

Then there are:

* Silent (0): this level will not pass any event.
* Undefined (-1): this is a level which defers to other barrier instances to check.

### Logger

Logger represent the logical subsystems of an application, e.g. you may have a frontend,
a network library and some database attached. In order to manage all these categories of events
one might turn on `Debug` only for the network part and remain all else in a somehow more quiet
condition.

Anytime you call `headcode::logger::GetLogger("foo")` will get the very same 
logger instance. If a logger for "foo" does not exist, one will be created.

All loggers do have a parent-child relationship, i.e. "foo" is the parent logger of "foo.bar" 
and "foo.baz". With this concept, one can fine tune the log event processing of groups
of subsystems, e.g. turn on all Debug for "network" and its children, but not for "database".
If the barrier value of a logger is *undefined* then the barrier value of the parent 
logger instance is used. The root logger does not accept *undefined* barrier.

The root logger (with no name) is the parent of all and will always be created.


### Sink

A sink is anything an event will be finally pushed. There are:

* FileSink: write into a file, aka a log-file.
* ConsoleSink: write to a terminal via `stderr`.
* SyslogSink: write to syslog.
* NullSink: consume events, like `/dev/null`.

A logger may have any number of sinks attached. One can write to three log files, the terminal 
and syslog in parallel. You can even attach two ConsoleSink if you want to.

A sink also has a barrier set like loggers. This is the "second firewall" for events to pass.
The rational is, that you may have a single event source (the Logger object) but may want to
pass on `Debug` events to a file, whereas `Critical` and `Warning` should also wind up in
the syslog, yet you refrain in sweeping the syslog with the very same `Debug` messages.

You can easily derive your own sink, e.g. a `NetworkSink` which passes on messages
via UDP, like this:

```c++
#include <headcode/logger/logger.hpp>

class NetworkSink : public headcode::logger::Sink {

private:
    std::string GetDescription_() const override {
        return "My new, shiny NetworkSink";
    }

    void Log_(Event const & event) override {
        // your code ...
        std::string log_message = Format(event);
        ...
    }
};

...
headcode::logger::Logger::GetLogger()->AddSink(std::make_shared<NetworkSink>());
```


### Formatter

Finally a `Formatter` object takes care to convert the event into the final message, to push
to the date storage.

Each sink has its own unique `Formatter`. Which you can change, of course.

You may also want to write you own `Formatters` and provide the event log of your liking,
if the `StandardFormatter` does not sport you.


### Example

The example below plays around with different logger instances and turns them
on or off. 

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
    
    // finally a multi-line example: each line will be prefixed
    Info() << "The quick brown\n" << "fox jumped over\n" << "a minimum of " << 1337 << "\nlazy dogs." << std::endl;

    return 0;
}
```


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

## Installation

I provide binary installation packages for some operating systems 
[here](https://gitlab.com/headcode.space/logger/-/packages).


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
