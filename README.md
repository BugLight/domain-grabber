# Domain grabber
It is a simple brute-force utility intended to obtain domain names list.

## Usage
Domain grabber checks availability of domains using http requests. Available
domains are put in the output file. It is possible to check domain names from
dictionary.

|Option|Purpose|
|------|-------|
|-o    |Output path|
|-d    |Dictionary path (optional)|
|-v    |Verbose console output|
|-h    |Help|


## Requirements
* libcurl
* ISO C11 compiler

## Build
Follow this simple steps:
1. Use `autoconf` to produce configure script
2. `./configure`
3. `make`
