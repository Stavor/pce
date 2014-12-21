CC=gcc
MPICC=mpicc
CXX=g++
MPICXX=mpicxx
CFLAGS=-O3 -Wall -std=gnu99 -openmp -lrt 
CXXFLAGS=-O3 -Wall -fopenmp
LDFLAGS=-lm -lrt -fopenmp

all: compare compare_bfs gen_RMAT_serial graphs_reference graphs_reference_fb gen_RMAT_mpi 

gen_RMAT_serial: gen_RMAT_serial.o bfs_serial_reference.o
	$(CXX) $^ -o $@ $(LDFLAGS)

gen_RMAT_mpi: gen_RMAT_mpi.c 
	$(MPICXX) $^ -o $@ -lrt

graphs_reference: main.o sssp_serial_reference.o bfs_serial_reference.o
	$(CXX) $^ -o $@ $(LDFLAGS)

# write your own file
graphs_reference_fb: main.o graphs_reference_fb.o bfs_serial_reference.o
	$(CXX) $^ -o $@ $(LDFLAGS)

compare: compare.o
	$(CXX) $^ -o $@ 

compare_bfs: compare_bfs.o
	$(CXX) $^ -o $@ 

.cpp.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $<

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o gen_RMAT_serial gen_RMAT_mpi graphs_reference compare compare_bfs graphs_reference_fb

