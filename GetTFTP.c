//
// Created by lorenzo on 11/12/24.
//

 #include "GetTFTP.h"

void usage(const char *progname) {
    fprintf(stderr, "Usage: %s <host> <file>\n", progname);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        usage(argv[0]);
    }

    const char *host = argv[1];
    const char *file = argv[2];

    struct addrinfo hints, *res;
    int sockfd;
    char buffer[BUFFER_SIZE];

    // Get server address info
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // Use IPv4
    hints.ai_socktype = SOCK_DGRAM; // Use UDP

    if (getaddrinfo(host, "69", &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    // Create socket
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        perror("socket");
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    // Construct RRQ packet manually
    buffer[0] = 0; // Opcode for RRQ
    buffer[1] = 1;
    int pos = 2;

    // Copy the filename
    memcpy(buffer + pos, file, strlen(file));
    pos += strlen(file);
    buffer[pos++] = 0; // Null terminator for filename

    // Copy the mode "octet"
    const char *mode = "octet";
    memcpy(buffer + pos, mode, strlen(mode));
    pos += strlen(mode);
    buffer[pos++] = 0; // Null terminator for mode

    int len = pos; // Total length of the RRQ packet

    // Debug: Write RRQ packet to terminal
    for (int i = 0; i < len; i++) {
        char hex[4];
        snprintf(hex, sizeof(hex), "%02x ", (unsigned char)buffer[i]);
        write(STDOUT_FILENO, hex, 3); // Write each byte in hex format
    }
    write(STDOUT_FILENO, "\n", 1); // Newline for better readability

    // Send RRQ packet
    if (sendto(sockfd, buffer, len, 0, res->ai_addr, res->ai_addrlen) == -1) { // sends the chain to the specified address
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    // Receive data single packet
    struct sockaddr_storage server_addr;
    socklen_t addr_len = sizeof(server_addr);
    ssize_t numbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
    if (numbytes == -1) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    // Print received data
    fwrite(buffer + 4, 1, numbytes - 4, stdout); // Skip TFTP header

    // Clean up
    freeaddrinfo(res);
    close(sockfd);

    return 0;
}