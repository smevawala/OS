all: fifotest

fifotest: sem.o fifo.o fifotest.c
	gcc sem.o fifo.o fifotest.c

fifo: sem.o
	gcc sem.o fifo.c



sem: