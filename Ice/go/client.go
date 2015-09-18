package main

import (
	"fmt"
	"net"
	"os"
)


func main() {

	conn, err := net.ListenUDP("udp", &net.UDPAddr{IP: net.IPv4zero, Port: 0})
	if err != nil {
		fmt.Println("Error: ", err)
		os.Exit(0);
	}

	serverAddr:= net.UDPAddr{
		Port: 9229,
		IP: net.ParseIP("127.0.0.1"),
	}

	buff := []byte("ssssss")
	len, err := conn.WriteToUDP(buff, &serverAddr);
	if err != nil {
		fmt.Println("Error: ", err);
	} else {
		fmt.Println("send: ", string(buff), len);
	}

	defer conn.Close()
}
