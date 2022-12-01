CC = mpicc
PROGS = pi piV1

all: $(PROGS)

clean:
	rm -f $(PROGS)