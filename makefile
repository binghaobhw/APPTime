exe=main
obj=main.o TimeList.o TimeNode.o
$(exe):$(obj)
	g++ -o $(exe) $(obj)
main.o:TimeList.h TimeNode.h
	g++ -c main.cpp
TimeList.o:TimeNode.h
	g++ -c TimeList.cpp
TimeNode.o:
	g++ -c TimeNode.cpp
.PHONY:clean
clean:
	rm $(exe) $(obj)
