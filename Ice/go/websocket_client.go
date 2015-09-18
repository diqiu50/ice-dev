package main

import (
	"fmt"
	"golang.org/x/net/websocket"
	"log"
)

func main() {
	origin := "http://localhost/"
	url := "ws://localhost:3700/echo"
	ws, err := websocket.Dial(url, "", origin)
	if err != nil {
		log.Fatal(err)
	}
	if _, err := ws.Write([]byte("hello, xxxworld!\n")); err != nil {
		log.Fatal(err)
	}
	var msg = make([]byte, 512)
	var n int
	if n, err = ws.Read(msg); err != nil {
		log.Fatal(err)
	}
	fmt.Printf("Received: %s.\n", msg[:n])
}
