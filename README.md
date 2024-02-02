# C++ Graph Library Proposal for the Standard C++ Library

Ths repository contains the documents (in latex form) for a proposed standard graph library. 
The documents are as follows (temporary numbers are given until official names have been assigned):

| Paper | Status | Description                     | 
| :-----| :------| :-------------------------------|
| P1709 | Retired| Original, split into following papers|
| D9901 | Active | Overview and Introduction       |
| D9902 | Active | Algorithms                      |
| D9903 | Active | Operators                       |
| D9904 | Active | Views                           |
| D9905 | Active | Graph Container Interface       |
| D9906 | Active | Graph Container Implementations |
| D9907 | Future | Adaptors                        |
| D9908 | Future | Background and Terminology      |


## Building

It is assumed you have a recent version of some TeX distribution installed.

A Makefile has build rules for all of the documents.

```
% make
```
will build them all.  Intermediate files are kept in the aux subdirectory.  Final pdf files are kept in the pdf subdirectory.


Individual pdfs can be built thusly:
```
% make D9901.pdf
```

The intermediate files can all be cleaned up:
```
% make clean
```
