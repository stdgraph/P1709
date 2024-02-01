# P1709
P1709: C++ Graph Library Proposal

Ths repository contains the document D1709r5 (in latex form) for the proposal P1709r5: A standard graph library.

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
