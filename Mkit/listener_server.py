import socket


def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # Socket TCP/IP

    server_address = ("192.168.21.253", 7777)
    server_socket.bind(server_address)

    server_socket.listen(1)

    print(f"Waiting on {server_address[0]}:{server_address[1]}")

    while True:
        client_socket, client_address = server_socket.accept()
        print(f"Established {client_address}")

        try:
            data = client_socket.recv(1024)  # 1024 bytes
            print(f"Received data : {data.decode()}")

        finally:
            client_socket.close()


if __name__ == "__main__":
    start_server()
