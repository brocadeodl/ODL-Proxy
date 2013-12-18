MAKEFILE := $(lastword $(MAKEFILE_LIST))

include $(WAVE_HOME)/source/Makefile.tools

ALL_YANG_FILES := $(wildcard *.yang)
ALL_YIN_FILES  := $(patsubst %.yang, %.xml, $(ALL_YANG_FILES))
ALL_H_FILES    := $(patsubst %.yang, %.h,   $(ALL_YANG_FILES))

all: generatehfiles

generatehfiles: generatewysertags

generatewysertags: $(ALL_YIN_FILES)

%.xml: %.yang
	@echo "    Generating $@ from $<";  \
	$(PYANG) -p . -o $@ -f yin $<

clean:
	rm -rf *.xml;
	rm -rf *.h;
