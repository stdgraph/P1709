

# List of document sources to build. 
SRCS	 	:= D3126_Overview D3127_Terminology D3128_Algorithms D9903 D3129_Views D3130_Container_Interface D9906 D9907
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

