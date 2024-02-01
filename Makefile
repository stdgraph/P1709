

# List of document sources to build. 
SRCS	 	:= D9901 D9902 D9903 D9904 D9905 D9906 D9907 # D9908
TEX_SRCS 	:= $(addsuffix .tex, $(SRCS))
PDFS     	:= $(addsuffix .pdf, $(SRCS))


OUTPUT_DIR 	:= pdf
AUX_DIR		:= aux
LATEXMK  	:= latexmk
LATEXMK_FLAGS  	:= -output-directory=$(OUTPUT_DIR) -aux-directory=$(AUX_DIR)


%.pdf: %.tex
	$(LATEXMK) $(LATEXMK_FLAGS) $<

.PHONY: all
all: $(PDFS)


.PHONY: clean
clean:
	$(LATEXMK) $(LATEXMK_FLAGS) -C ${SRCS}

.PHONY: squeaky
squeaky: clean

