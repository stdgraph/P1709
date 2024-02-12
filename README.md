# C++ Graph Library Proposal for the Standard C++ Library

Ths repository contains the documents (in latex form) for a proposed standard graph library. 
The documents are as follows (temporary numbers are given until official names have been assigned):

| Paper | Status | Description                     | 
| :-----| :------| :-------------------------------|
| P1709 | Retired| Original, split into following papers|
| P3126 | Active | Overview and Introduction       |
| P3127 | Active | Background and Terminology      |
| P3128 | Active | Algorithms                      |
| P3129 | Active | Views                           |
| P3130 | Active | Graph Container Interface       |
| P3131 | Active | Graph Container Implementations |
| D9903 | Future | Operators                       |
| D9907 | Future | Adaptors                        |


## Building

It is assumed you have a recent version of some TeX distribution installed.

A Makefile has build rules for all of the documents.

```
% make
```
will build them all.  Intermediate files are kept in the aux subdirectory.  Final pdf files are kept in the pdf subdirectory.


Individual pdfs can be built thusly:
```
% make D3128.pdf
```

The intermediate files can all be cleaned up:
```
% make clean
```
