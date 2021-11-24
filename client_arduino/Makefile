DIST_FILES := index.html style.css

.PHONY: all
all: src/dist_files.cpp

.PHONY: clean
clean:
	rm -f src/dist_files.cpp

src/dist_files.cpp: $(patsubst %,dist/%,$(DIST_FILES))
	( \
		cd dist; \
		echo '// GENERATED FILE. DO NOT EDIT.'; \
		echo '#include <Arduino.h>'; \
		echo 'namespace dist_files {'; \
		for f in $(DIST_FILES); do \
			xxd -i $$f \
				| sed 's/unsigned char/char const/;s/unsigned int/size_t const/;s/\[\]/[] PROGMEM/'; \
		done; \
		echo '}' \
	) > $@
