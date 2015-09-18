package main

import "golang.org/x/net/websocket"

import (
	"net/http"
	"fmt"
	"time"
	"strconv"
)

func echo(ws *websocket.Conn) {

	var req string;
	if err := websocket.Message.Receive(ws, &req); err != nil {
		fmt.Println(err);
		return
	}

	i := 0;
	for {
		time.Sleep(1*time.Second);
		var resp = req + " " + strconv.Itoa(i);
		i++;
		fmt.Println(resp);
		if err := websocket.Message.Send(ws, resp); err != nil {
			fmt.Println(err);
			break;
		}
	}
}

func main() {
	http.Handle("/", http.FileServer(http.Dir(".")))
	http.Handle("/echo", websocket.Handler(echo))
	err := http.ListenAndServe("192.168.99.184:3700", nil)
	if err != nil {
		fmt.Println(err);
	}
}

