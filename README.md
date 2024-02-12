# C++ Graph Library Proposal for the Standard C++ Library

Ths repository contains the documents (in latex form) for a proposed standard graph library. 
The documents are as follows (temporary numbers are given until official names have been assigned):

| Paper | Status | Description                     | 
| :-----| :------| :-------------------------------|
| P1709 | Retired| Original, split into following papers|
| D3126 | Active | Overview and Introduction       |
| D3127 | Future | Background and Terminology      |
| D3128 | Active | Algorithms                      |
| D3129 | Active | Views                           |
| D3130 | Active | Graph Container Interface       |
| D3131 | Active | Graph Container Implementations |
| D9903 | Active | Operators                       |
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
% make D3129.pdf
```

The intermediate files can all be cleaned up:
```
% make clean
```
