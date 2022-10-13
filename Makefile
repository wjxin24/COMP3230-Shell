CC=gcc
DIRS=build bin
$(shell mkdir -p $(DIRS))

EXE=bin/add\
 	bin/segfault\
  	bin/loopf\
   	bin/forever\
    bin/cmdarg\
	bin/inproc\
	bin/outproc\
	bin/relay

MYSHELL=build/myshell.o\
		build/readline.o\
		build/signal.o\
		build/parse.o\
		build/exec.o\
		build/processlist.o\
		bin/myshell

all: $(MYSHELL) $(EXE)



build/myshell.o: src/myshell.c
	$(CC) -c $<
	mv myshell.o build

build/readline.o: src/readline.c
	$(CC) -c $<
	mv readline.o build

build/signal.o: src/signal.c
	$(CC) -c $<
	mv signal.o build

build/parse.o: src/parse.c
	$(CC) -c $<
	mv parse.o build

build/exec.o: src/exec.c
	$(CC) -c $<
	mv exec.o build

build/processlist.o: src/processlist.c
	$(CC) -c $<
	mv processlist.o build	

bin/myshell: build/myshell.o build/readline.o build/signal.o build/parse.o build/exec.o build/processlist.o
	$(CC) $^ -o $@

bin/add: Utility/add-loop.c
	$(CC) $< -o $@

bin/segfault: Utility/segfault.c
	$(CC) $< -o $@

bin/loopf: Utility/loop.c
	$(CC) $< -o $@
    
bin/forever: Utility/loopever.c
	$(CC) $< -o $@
    
bin/cmdarg: Utility/cmdarg.c
	$(CC) $< -o $@
  
bin/inproc: Utility/inProc.c
	$(CC) $< -o $@

bin/outproc: Utility/outProc.c
	$(CC) $< -o $@

bin/relay: Utility/relay.c
	$(CC) $< -o $@

clean:
	rm -r bin build
