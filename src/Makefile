MAKE     = make
LIBDIRS  = S800 Utilities DDASEvent LendaEvent LendaSettings LendaPacker LendaFilter
COREDIR  = Core
#PROGRAMS = Evt2Tree RawEventLoop

LIBS     = $(addprefix -l, $(LIBDIRS))
LIBFILES = $(addsuffix .so, $(addprefix ../lib/lib, $(LIBDIRS)))
OBJFILES = $(shell find * -name "*.o")
PROGFILES= $(addprefix ../bin/, $(PROGRAMS))

.PHONY: all $(LIBDIRS) clean
all: all.subdirs
%.subdirs:
	@for subdir in $(LIBDIRS) $(COREDIR); do \
		echo "Make in " $$subdir;\
		(cd $$subdir && $(MAKE) $(basename $@) --no-print-directory) || exit; \
		echo;\
	done
	@echo
	@echo "R00TLe $(basename $@) has been built successfully"
	@echo
%.all:
	@cd $(basename $@) && $(MAKE) all
%.clean:
	@cd $(basename $@) && $(MAKE) clean
../lib/lib%.so:
	@cd $(subst lib,,$(basename $(notdir $@))) && $(MAKE) all
$(LIBFILES): $(OBJFILES)
$(PROGFILES): $(LIBFILES)
	@cd $(COREDIR) && $(MAKE) $(addprefix ../, $(basename $@))
clean: clean.subdirs

install:
	@../shell/Install.sh
