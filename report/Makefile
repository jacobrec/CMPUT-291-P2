SECS=$(wildcard sections/*.tex)
build: clean
	make report.pdf
	make pclean

%.pdf: %.tex $(SECS)
	pdflatex $<

%_dot.png: %.dot
	dot $< -Tpng > $@

clean: pclean
	rm *.pdf -f

pclean:
	rm *.log -f
	rm *.aux -f
	rm *_dot.png -f


.PHONY: clean build
