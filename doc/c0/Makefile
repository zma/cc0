filename=c0

pdf: ps
	ps2pdf ${filename}.ps

ps:	dvi
	dvips ${filename}.dvi

dvi:
	latex ${filename}
	bibtex ${filename}||true
	latex ${filename}
	latex ${filename}

read:
	evince ${filename}.pdf &

aread:
	acroread ${filename}.pdf &

clean:
	rm -f ${filename}.ps ${filename}.log ${filename}.pdf ${filename}.aux ${filename}.out ${filename}.dvi ${filename}.bbl ${filename}.blg
