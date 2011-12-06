all: paper.pdf

BuggyImplementation.png: BuggyImplementation.svg
	inkscape -z -eBuggyImplementation.png BuggyImplementation.svg

Correct.png: Correct.svg
	inkscape -z -eCorrect.png Correct.svg

disjoint.png: disjoint.svg
	inkscape -z -edisjoint.png disjoint.svg

IncompleteImplementation.png: IncompleteImplementation.svg
	inkscape -z -eIncompleteImplementation.png IncompleteImplementation.svg

Scope-Creep.png: Scope-Creep.svg
	inkscape -z -eScope-Creep.png Scope-Creep.svg

paper.pdf: paper.tex BuggyImplementation.png Correct.png disjoint.png IncompleteImplementation.png Scope-Creep.png WrightSources.bib
	pdflatex paper.tex
	bibtex paper.aux
	pdflatex paper.tex
	pdflatex paper.tex

clean: 
	rm -rf *.aux *.log *.pdf *.bbl *.blg
