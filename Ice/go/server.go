package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	addr := net.UDPAddr{
		Port: 9229,
		IP: net.ParseIP("127.0.0.1"),
	}

	conn, err := net.ListenUDP("udp", &addr)
	if err != nil {
		fmt.Println("Error: ", err)
		os.Exit(0);
	} else
	{
		fmt.Println("Started.")
	}
	defer conn.Close()

	buff := make([]byte, 1024)
	for {
		len, remoteaddr, err := conn.ReadFromUDP(buff)
		if err != nil {
			fmt.Println("Error: ", err)
			os.Exit(0);
		}
		fmt.Printf("Read a message from %v %s %d\n", remoteaddr, buff, len)
	}


}
