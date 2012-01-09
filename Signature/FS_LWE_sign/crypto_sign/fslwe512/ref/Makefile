CC = g++
CFLAGS = -O3 -Wall -g
INCLUDES = -I ~/sw/include -I /usr/local/include
LIBS = -lm -lntl -lgmp -lcrypto -lrt -lcpptest
LIBINC = -L ~/sw/lib -L /usr/include/lib -L /usr/lib -L /usr/local/lib

OBJ = aux.o fs_lwe_sign.o test.o
OBJ2 = aux.o fs_lwe_sign.o TestSuite.o
OUT = test
OUT2 = TestSuite


# link
all:    $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LIBS) $(LIBINC)

# build object files
%.o:    %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< 

# link test 
test: $(OBJ2)
	$(CC) -o $(OUT2) $(OBJ2) $(LIBS) $(LIBINC)

# make clean
clean:
	-rm -f $(OUT)
	-rm -f $(OUT2)
	-rm -f *.o
