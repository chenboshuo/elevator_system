.PHONY :  clean help %.out docs main_func
.ONESHELL: # Applies to every targets in the file!

## make main: Compile the main program
main_func:main.out

## make help : show this message.
help :
	@grep -h -E '^##' ${MAKEFILE_LIST} | sed -e 's/## //g' \
		| column -t -s ':'

%.out: %.ihx
	stcgal -P stc89 $<

## make *.ihx : generate the ihx files
# https://askubuntu.com/a/522776
%.ihx: %.c
	sdcc $<

## make clean: clean the temp files
clean:
	# git ls-files --others | xargs gio trash
	git clean -fXd
