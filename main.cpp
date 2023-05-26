#include "mbed.h"
#include "WIZnetInterface.h"

#define ECHO_SERVER_PORT   5005
// Network interface
//NetworkInterface *net;
WIZnetInterface net;
// Socket demo
int main() {
    int remaining;
    int rcount;
    char *p;
    char *buffer = new char[256];
    nsapi_size_or_error_t result;
    

    // Bring up the ethernet interface
    printf("Mbed OS Socket example\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif
    net.init();
    result = net.connect();
    const char *ip = net.get_ip_address();
    const char *netmask = net.get_netmask();
    const char *gateway = net.get_gateway();
    printf("IP address: %s\n", ip ? ip : "None");
    printf("Netmask: %s\n", netmask ? netmask : "None");
    printf("Gateway: %s\n", gateway ? gateway : "None");

    TCPSocket socket;
    result = socket.open(&net);
    if (result != 0) {
        printf("Error! socket.open() returned: %d\n", result);
    }
   
    
    #if 1
    result = socket.connect("api.ipify.org", 80);
    
    if (result != 0) {
        printf("Error! socket.connect() returned: %d\n", result);
    }

    // Send a simple http request
    char sbuffer[] = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n";
    nsapi_size_t size = strlen(sbuffer);

    // Loop until whole request send
    while(size) {
        result = socket.send(sbuffer+result, size);
        if (result < 0) {
            printf("Error! socket.connect() returned: %d\n", result);
            goto disconnect;
        }
        size -= result;
        printf("sent %d [%.*s]\n", result, strstr(sbuffer, "\r\n")-sbuffer, sbuffer);
    }

    // Receieve an HTTP response and print out the response line
    remaining = 256;
    rcount = 0;
    p = buffer;

    result = socket.recv(p, remaining);
    /*
    // modified source..a bit strnage...
    while (0 < (r = socket.recv(p, remaining))) {
        p += r;
        rcount += r;
        remaining -= r;
    }
    */
    if (result < 0) {
        printf("Error! socket.recv() returned: %d\n", result);
        goto disconnect;
    }

    p += result;
    rcount += result;
    remaining -= result;

    printf("recv %d [%.*s]\n", rcount, strstr(buffer, "\r\n")-buffer, buffer);

    // The api.ipify.org service also gives us the device's external IP address
    p = strstr(buffer, "\r\n\r\n")+4;
    printf("External IP address: %.*s\n", rcount-(p-buffer), p);
    delete[] buffer;

disconnect:
    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    net.disconnect();
    printf("Done\n");
#endif
}
