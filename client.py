import socket
import struct
import time

# Configuration
HOST = '127.0.0.1'
PORT = 12345

def send_order(sock, order_type, oid, price, qty, side):
    # Pack the data into binary format:
    # c (char), Q (uint64), Q (uint64), I (uint32), I (uint32)
    # Total = 1 + 8 + 8 + 4 + 4 = 25 bytes? Wait, C++ struct padding might make it 26 or more.
    # Our C++ struct used #pragma pack(1), so it is exactly 1 + 8 + 8 + 4 + 4 = 25 bytes.
    msg = struct.pack('=cQQII', order_type, oid, price, qty, side)
    sock.sendall(msg)

def main():
    print(f"Connecting to {HOST}:{PORT}...")
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    print("Connected!")

    # 1. Send Sell Order (Limit, ID 1, Price 100, Qty 10, Side 1=Sell)
    print("Sending: Sell 10 @ 100")
    send_order(s, b'L', 1, 100, 10, 1)
    time.sleep(1)

    # 2. Send Buy Order (Limit, ID 2, Price 100, Qty 5, Side 0=Buy)
    print("Sending: Buy 5 @ 100")
    send_order(s, b'L', 2, 100, 5, 0)
    time.sleep(1)

    # 3. Send Buy Order (Market, ID 3, Price 0, Qty 5, Side 0=Buy)
    print("Sending: Buy 5 (Market)")
    send_order(s, b'M', 3, 0, 5, 0)
    
    input("Press Enter to close...")
    s.close()

if __name__ == "__main__":
    main()