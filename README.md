crc32
=====

crc32 implementation in a myriad of languages.

Totally Unscientific Results
============================

In order of speed:

* C++ (optimized)
* C (optimized)
* Java
* Go
* C (no optimizations)
* Node
* C++ (no optimizations)
* Lisp (compiled)
* Ruby
* Python
* Lisp (interpreted)

Run with a 40MB randomly generated file.


### C
_i686-apple-darwin11-llvm-gcc-4.2 (GCC) 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2336.11.00)_

```
No optimizations: 0m0.244s
With optimizations (-O3): 0m0.136s
```

### C++
_i686-apple-darwin11-llvm-g++-4.2 (GCC) 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2336.11.00)_

```
No optimizations: 0m1.675s
With optimizations (-O3): 0m0.128s
```

### Go
_go version go1.0.3_

```
Go: 0m0.220s
```

### Java
_java version "1.7.0_15"_

```
Java: 0m0.150s
```

### Lisp
_GNU CLISP 2.49 (2010-07-07)_

```
Interpreted: 0m38.779s
Compiled: 0m11.788s
```

### Javascript
_node v0.8.8_

```
Node: 0m0.715s
```

### Python
_Python 3.3.0_

```
Python: 0m22.483s
```

### Ruby
_ruby 1.9.3p327 (2012-11-10 revision 37606) [x86_64-darwin12.2.0]_

```
Ruby: 0m16.289s
```
