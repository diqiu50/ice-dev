package main

import "golang.org/x/net/websocket"
import "github.com/shirou/gopsutil/cpu"
import "github.com/shirou/gopsutil/mem"
import "github.com/shirou/gopsutil/disk"
import "github.com/shirou/gopsutil/net"

import (
	"net/http"
	"fmt"
	"math"
	"time"
	"strconv"
)

func echo(ws *websocket.Conn) {

	var req string;
	if err := websocket.Message.Receive(ws, &req); err != nil {
		fmt.Println(err);
		return
	}
	fmt.Println("recive: " + req);

	for {
		var resp = getSysinfo();
		if err := websocket.Message.Send(ws, resp); err != nil {
			fmt.Println(err);
			break;
		}
	}
}

func main() {
	http.Handle("/", http.FileServer(http.Dir("webpage")))
	http.Handle("/echo", websocket.Handler(echo))
	err := http.ListenAndServe("192.168.99.184:3700", nil)
	if err != nil {
		fmt.Println(err);
	}
}

func getSysinfo() string{
	var io_r_bytes int64 = 0;
	var io_w_bytes int64 = 0;
	rlst3, _ := disk.DiskIOCounters()
	for k, v := range rlst3 {
		if len(k) == 3 {
			io_r_bytes += int64(v.ReadBytes)
			io_w_bytes += int64(v.WriteBytes)
		}

	}

	var net_io_s_bytes int64 = 0;
	var net_io_r_bytes int64 = 0;
	rlst4, _ := net.NetIOCounters(false)
	net_io_s_bytes += int64(rlst4[0].BytesSent);
	net_io_r_bytes += int64(rlst4[0].BytesRecv);

	rslt1, _ := cpu.CPUPercent(1*time.Second, false);
	cpu := strconv.FormatFloat(rslt1[0], 'f', 2, 64);

	rslt2, _ := mem.VirtualMemory();
	mem := strconv.FormatFloat(rslt2.UsedPercent, 'f', 2, 64);

	rlst3, _ = disk.DiskIOCounters()
	for k, v := range rlst3 {
		if len(k) == 3 {
			io_r_bytes -= int64(v.ReadBytes)
			io_w_bytes -= int64(v.WriteBytes)
		}
	}
	diskio_r := strconv.FormatFloat(math.Abs(float64(io_r_bytes))/(90*1024*1024/100), 'f', 2, 64);
	diskio_w := strconv.FormatFloat(math.Abs(float64(io_w_bytes))/(90*1024*1024/100), 'f', 2, 64);

	rlst4, _ = net.NetIOCounters(false)
	net_io_s_bytes -= int64(rlst4[0].BytesSent);
	net_io_r_bytes -= int64(rlst4[0].BytesRecv);
	netio_s := strconv.FormatFloat(math.Abs(float64(net_io_s_bytes))/(90*1024*1024/100), 'f', 2, 64);
	netio_r := strconv.FormatFloat(math.Abs(float64(net_io_r_bytes))/(90*1024*1024/100), 'f', 2, 64);

	str := "{"
	str += "\"cpu_usage\":\"" + cpu + "\","
	str += "\"mem_usage\":\"" + mem + "\","
	str += "\"disk_usage_read\":\"" + diskio_r + "\","
	str += "\"disk_usage_write\":\"" + diskio_w + "\","
	str += "\"net_usage_send\":\"" + netio_s + "\","
	str += "\"net_usage_recv\":\"" + netio_r + "\""
	str += "}"
	return str
}

