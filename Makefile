IDIR = ~/boost_1_55_0
CC=g++
CFLAGS=-I $(IDIR) -O3

ODIR=obj
LDIR=../lib

LIBS=$(IDIR)/stage/lib/libboost_date_time.so.1.55.0 $(IDIR)/stage/lib/libboost_random.so.1.55.0

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = exp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o:%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

exp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY:clean

clean:
	rm -f $(ODIR)/*.o
