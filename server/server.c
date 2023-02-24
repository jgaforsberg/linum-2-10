#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	int simple_socket = 0, returnStatus = 0;
	struct sockaddr_in server_adress;
	char requested_id[255];
	if(argc < 2) {
		printf("Användning: app_server <Port>\n");
		exit(1);
	}
	simple_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(simple_socket == -1) {
		printf("Socket EJ OK\n");
		exit(1);
	}
	bzero(&server_adress, sizeof(server_adress));
	server_adress.sin_family = AF_INET;
	server_adress.sin_port = htons(atoi(argv[1])); /* porten tillskrivs argv[1] värde */
	server_adress.sin_addr.s_addr = htonl(INADDR_ANY); /* localhost */
	returnStatus = bind(simple_socket, (struct sockaddr *)&server_adress, sizeof(server_adress));
	if(returnStatus == 0) {
		printf("Bind OK\n");
	}
	else {
		printf("Bind EJ OK\n");
		close(simple_socket);
		exit(1);
	}
	returnStatus = listen(simple_socket, 5); /* antal köande anslutningar = 5 */
	if(returnStatus == 0) {
		printf("Listen OK\n");
	}else {
		printf("Listen EJ OK\n");
		close(simple_socket);
		exit(1);
	}
	while(1) {
		struct sockaddr_in client_name = { 0 };
		int child_socket = 0;
		int client_name_length = sizeof(client_name);
		child_socket = accept(simple_socket, (struct sockaddr *)&client_name, (socklen_t *)&client_name_length);
		if(child_socket == -1) {
			printf("Klientanslutning misslyckades\n");
			close(simple_socket);
			exit(1);
		}
		printf("Klient ansluten\n");
		recv(child_socket, requested_id, sizeof(requested_id), 0);
		printf("Klientmeddelande: %s", requested_id);
		char line[256];
		char student_info[256];
		FILE *file = fopen("students.txt", "r");
		while(fgets(line, sizeof(line), file))
		{			
			char* pnr = strtok(line, ":");
			char* name = strtok(NULL, ":");
			char* grade = strtok(NULL, ":");
			if (strcmp(pnr, requested_id) == 0)
			{
				sprintf(student_info, "Pnr: %s\nNamn: %s\nBetyg: %s", pnr, name, grade);
				send(child_socket, student_info, strlen(student_info), 0);
			}
			else
			{
				sprintf(student_info, "Ingen match\n");
				send(child_socket, student_info, strlen(student_info), 0);
			}
		}
		fclose(file);
		printf("Klient frånkopplad\n");
		printf("************************************\n");
		close(child_socket);
	}
	close(simple_socket);
	return 0;
}
