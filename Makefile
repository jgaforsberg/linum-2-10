CC=gcc
INSS=server
INSC=client


all: app_server app_client

app_server: server.c
	$(CC) server.c -o app_server

app_client: client.c
	$(CC) client.c -o app_client

install:
	cp app_server ./$(INSS)/
	cp app_client ./$(INSC)/
	rm -rf app_server app_client

uninstall:
	rm -rf ./$(INSS)/app_server ./$(INSC)/app_client

clean:
	rm -rf app_server app_client
