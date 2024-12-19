//
// Created by lorenzo on 11/12/24.
//

#ifndef GETTFTP_H
#define GETTFTP_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 516 // 512 bytes for data + 4 bytes for TFTP header
#define DATA_SIZE 512 // Taille maximale des données dans un paquet DATA

void usage(const char *);
int getTFTP(int, char *);

#endif //GETTFTP_H
