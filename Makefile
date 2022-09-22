
dummy: 

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

icmp: icmp_exp/route

icmp_exp/route: icmp_exp/route

.PHONY: clean cleanudp cleantcp cleanicmp
clean: cleanudp cleantcp cleanicmp

cleanudp: 
	rm -f udp_exp/server_udp udp_exp/client_udp

cleantcp:
	rm -f tcp_exp/server_tcp tcp_exp/client_tcp

cleanicmp:
	rm -f icmp_exp/route