
udp: udp/server_udp udp/client_udp

udp/server_udp: udp/server_udp.c
	$(CC) udp/server_udp.c -o udp/server_udp

udp/client_udp: udp/client_udp.c
	$(CC) udp/client_udp.c -o udp/client_udp


.PHONY: clean cleanudp
clean: cleanudp

cleanudp: 
	rm -f udp/server_udp udp/client_udp