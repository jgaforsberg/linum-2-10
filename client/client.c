#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <netinet/in.h>

int main(int argc, char* argv[]) {
	int network_socket = 0, conn = 0;
	struct sockaddr_in server_adress;
	char query[56];
	char buffer[4096] = "";

	if(argc < 3) {
		printf("Användning: app_client <IP-adress|server> <Port>\n");
		exit(1);
	}

	network_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(network_socket == -1) {
		printf("Socket missyckades\n");
		exit(1);
	}

	bzero(&server_adress, sizeof(server_adress));
	server_adress.sin_family = AF_INET;
	server_adress.sin_port = htons(atoi(argv[2]));
	server_adress.sin_addr.s_addr = inet_addr(argv[1]);

	conn = connect(network_socket, (struct sockaddr *)&server_adress, sizeof(server_adress));
	if(conn == 0) {
		printf("Ansluten\n");
	}
	else {
		printf("Kan ej ansluta till server\n");
		exit(1);
	}
	// användarinput
	printf("Ange personnummer att söka: ");
	scanf("%s", query);
	printf("Query: %s", query); /* print för att kontrollera char array*/
	// meddelande till server
	send(network_socket, query, strlen(query), 0);
	// meddelande från server
	conn = recv(network_socket, buffer, sizeof(buffer), 0);
	if(conn > 0) {
		printf("\nSvar: %s\n", buffer); /* skriv ut meddelande från server */
	}
	else {
		printf("Fel. Exitkod: %d\n", conn);
		close(network_socket);
		exit(1);
	}

	close(network_socket);
	return 0;
}
