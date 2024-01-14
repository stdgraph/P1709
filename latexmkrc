# $pdflatex = 'pdflatex -shell-escape %R';
$pdflatex = 'pdflatex -shell-escape';
$pdf_mode = 1; # TeX -> PDF;
$dvi_mode = 0;
$postscript_mode = 0;
@default_files = ('D1709R4.tex');
$clean_ext = "bbl nav out snm log blg synctex.gz aux lox dvi xdv";
