CC = c2bf/c2bf.native
CPP = cpp
PROCESSED = preprocessed.c

PROJECT_SOURCE_FILES ?= main.c mkdir.c del.c read.c cp.c mv.c write.c screenfetch.c append.c

RESULTS = $(patsubst %.c, out/%.bf, $(PROJECT_SOURCE_FILES))

out/%.bf: %.c
    # Call preprocessor because c2bf doesn't have one
	$(CPP) $< -o $(PROCESSED) -P
	$(CC) -o $@ $(PROCESSED)

all: $(RESULTS)

clean:
	rm $(PROCESSED)
	rm $(RESULTS)

