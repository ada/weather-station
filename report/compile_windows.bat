:loop
if exist "./output" rd /q /s "./output"
mkdir output
pdflatex main.tex -output-directory output
bibtex .\output\main.aux
pdflatex main.tex -output-directory output
pdflatex main.tex -output-directory output
@output\main.pdf
@echo Do you want to compile again?
@pause
goto loop
