%.o : %.c 
	gcc -g -c $< -o $@  

%.o : %.cpp 
	gcc -I/usr/include/boost -g -std=gnu++11 -c $< -o $@  

objs = GisToRadar.o main.o


all: CoordTrans


CoordTrans: $(objs)   
	g++ -g -o CoordTrans $(objs) -lboost_program_options -ludunits2 -lm
	
.PHONY: clean
clean:
	rm *.o CoordTrans 
