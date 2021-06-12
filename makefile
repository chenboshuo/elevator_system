.PHONY :  clean help %.out docs
.ONESHELL: # Applies to every targets in the file!
## make help : show this message.
help :
	@grep -h -E '^##' ${MAKEFILE_LIST} | sed -e 's/## //g' \
		| column -t -s ':'

## make doxygen: generate doxygen docs
doxygen:
	cd docs
	doxygen

## make sphinx_html: generate sphinx docs
sphinx_html: doxygen
	cd docs
	make html

## make sphinx_pdf: generate sphinx docs using latex
sphinx_pdf: doxygen
	cd docs
	make latexpdf

## make doxygen_pdf: generate the docs pdf using doxygen
doxygen_pdf: doxygen
	cd docs/build/doxygen_latex
	make all
	# pdftk main.pdf ./doxygen_latex/refman.pdf cat output 1.pdf

## make main_report: gererate the main part of the report
main_report:
	make --directory=./docs/report main

## make report: combined report of the project
report: main_report doxygen_pdf
	pdftk docs/report/build/main.pdf \
	  docs/build/doxygen_latex/refman.pdf \
		cat output docs/report/build/report.pdf

## make main: Compile and run the main function file
main:
	make --directory=src main.out



## make clean: clean the temp files
clean:
	# git ls-files --others | xargs gio trash
	git clean -fXd
