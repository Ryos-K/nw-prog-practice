
udp: udp_exp/server_udp udp_exp/client_udp

udp_exp/server_udp: udp_exp/server_udp.c
	$(CC) udp_exp/server_udp.c -o udp_exp/server_udp

udp_exp/client_udp: udp_exp/client_udp.c
	$(CC) udp_exp/client_udp.c -o udp_exp/client_udp

tcp: tcp_exp/server_tcp tcp_exp/client_tcp

tcp_exp/server_tcp: tcp_exp/server_tcp.c
	$(CC) tcp_exp/server_tcp.c -o tcp_exp/server_tcp

tcp_exp/client_tcp: tcp_exp/client_tcp.c
	$(CC) tcp_exp/client_tcp.c -o tcp_exp/client_tcp

.PHONY: clean cleanudp cleantcp
clean: cleanudp cleantcp

cleanudp: 
	rm -f udp_exp/server_udp udp_exp/client_udp

cleantcp:
	rm -f tcp_exp/server_tcp tcp_exp/client_tcp