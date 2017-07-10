#define _BSD_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>

static void ip_adressen(const char *ip_addr) {
	struct in_addr ip;
	char *ip_str;
	
	if (inet_aton(ip_addr, &ip) == 0)
		return;
	
	printf("\n *** Gesamtteil der IP-Adresse *** \n");
	printf("IP-Adresse                               : %s\n", ip_addr);
	printf("IP-Adresse als 32-bit Wert - inet_aton() : %u\n", ip.s_addr);

	ip_str = inet_ntoa(ip);
	if (ip_str == NULL)
		return;

	printf("IP-Adresse string - inet_ntoa()          : %s\n", ip_str);
}


static void ip_netzwerk(const char *ip_addr) {
	struct in_addr ip, net_ip;
	unsigned long int net, net_num;
		
	if (inet_aton(ip_addr, &ip) == 0)
		return;
	
	net = inet_netof(ip);
	if (net == 0)
		return;
	
	printf("\n *** Netzwerkteil der IP-Adresse *** \n");
	printf("Netzwerk-Teil der Adresse `inet_netof()` : %ld\n", net);
	
	net_ip.s_addr = net_num = ntohl(inet_netof(ip) << 8);
	printf("--->%s", inet_ntoa(net_ip));
	printf("--> (als 32-bit Wert)   : %lu\n", net_num);
	
	net_ip = inet_makeaddr(htonl(net_num), 0);
	printf("inet_makeaddr()-> (als 32-bit Wert)      : %d\n", net_ip.s_addr);
}

static void ip_lokal(const char *ip_addr) {
	struct in_addr ip, lokal_ip;
	unsigned long int lokal_num;
		
	if (inet_aton(ip_addr, &ip) == 0)
		return;

	printf("\n *** Lokaler Teil der IP-Adresse *** \n");

	lokal_ip.s_addr = lokal_num = ntohl(inet_lnaof(ip));
	printf("--->%s", inet_ntoa(lokal_ip));
	printf("--> (als 32-bit Wert)     : %lu\n", lokal_num);
	
	lokal_ip = inet_makeaddr(0, htonl(lokal_num));
	printf("inet_makeaddr()-> (als 32-bit Wert)    : %u\n", lokal_ip.s_addr);
}


static void ip_zusammen(const char *ip_addr) {
	struct in_addr ip, lokal_ip, net_ip, zusammen_ip;
	unsigned long int lokal_num, net_num;
		
	if (inet_aton(ip_addr, &ip) == 0)
		return;

	printf("\n *** Lokale Teile und Netzwerkteil zusammensetzen *** \n");

	lokal_ip.s_addr = lokal_num = ntohl(inet_lnaof(ip));
	printf("Lokaler Teil: ");
	printf("%21s = ", inet_ntoa(lokal_ip));
	printf("(als 32-bit Wert : %lu\n", lokal_num);
	
	printf("Netzwerk Teil: ");
	net_ip.s_addr = net_num = ntohl(inet_netof(ip) << 8);
	printf("%20s = ", inet_ntoa(net_ip));
	printf("(als 32-bit Wert : %lu\n", net_num);
	
	zusammen_ip = inet_makeaddr(htonl(net_num),
				    htonl(lokal_num));
	printf("Alles wieder mit inet_makeaddr() zusamenfuegt: ");
	printf("\n%s = %u\n", inet_ntoa(zusammen_ip), zusammen_ip.s_addr);
}




int main(int argc, char** argv) {
	char *ptr;
	
	if (argc != 2)
		printf("Verwendung %s IP-Adresse\n", *argv);
	else {
		ptr = argv[1];
		ip_adressen(ptr);
		ip_netzwerk(ptr);
		ip_lokal(ptr);
		ip_zusammen(ptr);
	}

	return EXIT_SUCCESS;
}
