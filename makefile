SHELL := /bin/bash

src = src
obj = obj
bin = bin
program = program

inc = -I ${src}/includes/
compilerPath = ${obj}/compiler.mk

gpp_options = -std=c++2a -O2
gpp_options = -std=c++2a -g -include "iostream" -include "ANSI.h" # DEBUG

src_finder = find ${src} -iname "*.c" -or -iname "*.cpp"




.PHONY: all
all: ${bin}/${program}

# Run program
.PHONY: run
run: ./${bin}/${program}
	./${bin}/${program}

# .PHONY: ff
# ff: pic.bin
# 	# cat "pic.bin" | ffmpeg -vcodec rawvideo -f rawvideo -pix_fmt rgba -s 360x360 -i - -vframes 1 out.png -y
# 	ffmpeg -vcodec rawvideo -f rawvideo -pix_fmt rgba -s 360x360 -i "pic.bin" -vframes 1 -vcodec png out.png -y





# Remove generated files
.PHONY: clean
clean:
	rm -rf "${compilerPath}" "${obj}" "${bin}"
	@echo "Project cleaned."




# Create directories
${obj} ${bin}:
	mkdir "$@"




# Build makefile target dependencies on all source files
.PHONY: compiler
compiler:
	rm -f "${compilerPath}"
	@make "${compilerPath}" --no-print-directory
	@echo "Done."

${compilerPath}:
	@mkdir -p "$(shell dirname "${compilerPath}")"
	
	@echo "Generating makefile: ${compilerPath}"
	@bash -c '{													\
		printf "# Auto-generated file. Do not touch!\n\n" ;		\
		readarray -t src_files < <(${src_finder}) ;				\
		\
		while read -r line; do									\
			line="\$${obj}/$${line}" ;							\
			echo "$${line} | \$${bin}" ;						\
			echo -e "\t@basename \"\$$@\"" ;					\
			echo -e "\t@g++ \$$(filter %.c %.cpp %.inc, \$$^) \$${inc} -c \$${gpp_options} -o \$$@" ;	\
			obj_files+=( "$$(grep -oP "^.+\.o" <<<"$$line")" ) ;			\
		done < <(															\
			g++ -MM ${inc} "$${src_files[@]}" |								\
			perl -pe "s|\s*\\\\\n||g" ;										\
		) ;																	\
		\
		echo "" ;															\
		echo "\$${bin}/\$${program}: $${obj_files[@]} | \$${bin}" ;			\
		echo -e "\t@basename \"\$$@\"" ;									\
		echo -e "\t@g++ \$$(filter %.o, \$$^) \$${gpp_options} -o \$$@" ;	\
	} >"${compilerPath}"													\
	'

ifeq (,$(MAKECMDGOALS))
include ${compilerPath}
else ifneq (,$(filter all run ${obj}/%.o , $(MAKECMDGOALS)))
include ${compilerPath}
endif
