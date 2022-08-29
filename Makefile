
udp: udp/udps udp/udpc

udp/udps: udp/udps.c
	$(CC) udp/udps.c -o udp/udps

udp/udpc: udp/udpc.c
	$(CC) udp/udpc.c -o udp/udpc


.PHONY: clean cleanudp
clean: cleanudp

cleanudp: 
	rm -f udp/udps udp/udpc