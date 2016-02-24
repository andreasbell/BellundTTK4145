#pragma once

int server();
void connection_handler(int sock, struct sockaddr_in client);