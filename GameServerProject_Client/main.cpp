#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "../protocol.h"

sf::TcpSocket sock;

bool isFail = true;

void send_packet(void* packet);
void recv_packet();

void process_packet(char* packet);

int main()
{
    sf::Socket::Status socket_status = sock.connect("127.0.0.1", PORT_NUM);
    if (socket_status != sf::Socket::Done) {
        std::wcout << L"서버와 연결할 수 없습니다.\n";
        return 0;
    }
    
    while (isFail)
    {
        char name[NAME_SIZE];
        std::cout << "================ Login ================\n";
        std::cout << "Input name: ";
        std::cin >> name;
        
        CS_LOGIN_PACKET login_pack;
        login_pack.size = sizeof(login_pack);
        login_pack.type = CS_LOGIN;
        strcpy(login_pack.name, name);
        
        send_packet(&login_pack);
        
        recv_packet();
    }
}

void send_packet(void* packet)
{
    unsigned char *p = reinterpret_cast<unsigned char*>(packet);
    size_t sent = 0;
    sock.send(packet, p[0], sent);
}

void recv_packet()
{
    char recv_buf[BUF_SIZE];
        size_t recv_bytes;

        auto ret = sock.receive(recv_buf, BUF_SIZE, recv_bytes);
    
        if (ret == sf::Socket::Error) std::wcout << L"Recv err";
    
        if (ret != sf::Socket::NotReady) {
            char* ptr = recv_buf;
            static size_t in_packet_size = 0;
            static size_t saved_packet_size = 0;
            static char packet_buffer[BUF_SIZE];

            while (0 != recv_bytes) {
                if (0 == in_packet_size) in_packet_size = ptr[0];
                if (recv_bytes + saved_packet_size >= in_packet_size) {
                    memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
                    process_packet(packet_buffer);
                    ptr += in_packet_size - saved_packet_size;
                    recv_bytes -= in_packet_size - saved_packet_size;
                    in_packet_size = 0;
                    saved_packet_size = 0;
                } else {
                    memcpy(packet_buffer + saved_packet_size, ptr, recv_bytes);
                    saved_packet_size += recv_bytes;
                    recv_bytes = 0;
                }
            }
        }
}

void process_packet(char* packet)
{
    switch (packet[1])
    {
            
    }
}
