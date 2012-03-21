CC = g++
CFLAGS = -O3 -Wall -g
INCLUDES = -I ~/sw/include -I /usr/local/include
LIBS = -lm -lcrypto -lrt -lcpptest  -lgmp -lntl 
LIBINC =  -L /usr/include/lib -L /usr/lib -L /usr/local/lib

OBJ = aux.o fs_lwe_sign.o test.o cpucycles.o
OBJ2 = aux.o fs_lwe_sign.o TestSuite.o
OBJ3 = mpzX_test.o mpzX.o cpucycles.o
OUT = test
OUT2 = TestSuite
OUT3 = test_mpzX


# link
all:    $(OBJ)
	$(CC) -o $(OUT) $(OBJ) $(LIBS) $(LIBINC)

# build object files
%.o:    %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< 

# link test 
test: $(OBJ2)
	$(CC) -o $(OUT2) $(OBJ2) $(LIBS) $(LIBINC)

test_mpzX: $(OBJ3)
	$(CC) -o $(OUT3) $(OBJ3) $(LIBS) $(LIBINC)	

# make clean
clean:
	-rm -f aux.o fs_lwe_sign.o test.o TestSuite.o mpzX_test.o mpzX.o
