all: matmul_nokml matmul

SOURCES = \
	main.c \
	matrix.c \
	base_matmult.c \
	block_matmult.c \
	intrinsic_matmult.c

matmul_nokml:
	gcc -g -march=armv8.2-a+fp16fml+simd -O3 -fopenmp $(SOURCES) -o matmul_nokml -DROOFLINE_EVENTS -I$(RFEVENTS_INCLUDE) -L$(RFEVENTS_LIB) -lrfevents

matmul:
	gcc -g -march=armv8.2-a+fp16fml+simd -O3 -fopenmp $(SOURCES) -o matmul -DROOFLINE_EVENTS -I$(RFEVENTS_INCLUDE) -L$(RFEVENTS_LIB) -lrfevents -DENABLE_KML -I$(KML_INCLUDE) -L$(KML_LIB) -lkblas

clean:
	rm -rf ./matmul ./matmul_nokml