SolverDoc solverDoc.conf
del "TRIEDA.pdf" /q /f
pdflatex  "TRIEDA"
del *.aux
del *.log
del *.dvi
del *.bbl
del *.blg
del *.tcp
del *.bak
del *.tps
del *.out
