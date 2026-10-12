# Static recompilation of Pushy II: the translated program (gen/, from
# superspider gen) and game.c, linked with libroger (the RISC OS runtime).
ROGER   ?= $(HOME)/stuff/libroger
CC      ?= gcc
CFLAGS  ?= -O2 -g -w -fno-strict-aliasing $(if $(TRACE),-DRECOMP_TRACE)
GEN     := $(wildcard gen/code*.c) gen/entries.c
OBJ     := $(GEN:.c=.o) game.o
LIBS    := $(shell pkg-config --libs sdl3) -lm -lpthread
INC     := -I$(ROGER)/include

pushy2: $(OBJ) $(ROGER)/libroger.a
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(ROGER)/libroger.a $(LIBS)

$(ROGER)/libroger.a: FORCE
	$(MAKE) -C $(ROGER) libroger.a

gen/%.o: gen/%.c $(ROGER)/include/roger_cpu.h gen/gen.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

game.o: game.c $(ROGER)/include/roger.h
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Coverage build (~/stuff/superspider/phase0): "make cov" builds pushy2-cov
# from separate *.cov.o objects; it writes $$RECOMP_COV (default cov.out).
COVDIR  ?= $(HOME)/stuff/superspider/phase0/cov
COVOBJ  := $(OBJ:.o=.cov.o) cov.cov.o

cov: pushy2-cov

pushy2-cov: $(COVOBJ) FORCE
	$(MAKE) -C $(ROGER) cov COV=$(COVDIR)
	$(CC) $(CFLAGS) -o $@ $(COVOBJ) $(ROGER)/libroger-cov.a $(LIBS)

%.cov.o: %.c $(ROGER)/include/roger_cpu.h $(COVDIR)/cov.h
	$(CC) $(CFLAGS) -DRECOMP_COV $(INC) -I$(COVDIR) -c -o $@ $<

cov.cov.o: $(COVDIR)/cov.c $(COVDIR)/cov.h
	$(CC) $(CFLAGS) -c -o $@ $<

SS  ?= $(HOME)/stuff/superspider
# !RunImage, whatever its ,load-exec suffix
IMAGE   := $(firstword $(wildcard ../game/!PushyII/!RunImage,*) ../game/!PushyII/!RunImage)

regen:
	cd ../analysis && SUPERSPIDER=$(SS) ./run.sh
	$(SS)/_build/default/bin/main.exe gen '$(IMAGE)' 8000 ../analysis/pushy2.json --c gen

# "make roots" merges the gap entries a --trace-gaps run logged
# (untranslated.log, or LOG=...) into ../analysis/pushy2.roots, so the next
# "make regen" translates them.
SS  ?= $(HOME)/stuff/superspider
LOG ?= untranslated.log

roots:
	python3 $(SS)/tools/gap_roots.py '$(IMAGE)' 8000 ../analysis/pushy2.roots $(LOG)

clean:
	rm -f gen/*.o game.o pushy2 pushy2-cov *.cov.o gen/*.cov.o

.PHONY: FORCE cov regen roots clean
