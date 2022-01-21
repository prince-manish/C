CC=gcc

CFLAGS=-Wall

OBJDIR=build
$(shell mkdir -p $(OBJDIR))

all: osmanager

$(OBJDIR)/process.o:	process/process.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/thread.o:		thread/thread.c 
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/queue.o:		queue/queue.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/process_manager.o:	process_manager/process_manager.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/main.o:		main.c 
	$(CC) $(CFLAGS) -c $^ -o $@

osmanager_o = $(addprefix $(OBJDIR)/, main.o thread.o queue.o process.o process_manager.o)

osmanager:	$(osmanager_o)
	$(CC) $^ $(CFLAGS) -o $@

clean:
	rm -rf osmanager build/
