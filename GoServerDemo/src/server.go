package main

import (
	"fmt"
	"net"
	"net/http"
	"os"
	"os/signal"
	"syscall"
	"time"
)

// 获取局域网IP地址
func getLocalIP() (string, error) {
	// 获取所有网络接口
	interfaces, err := net.Interfaces()
	if err != nil {
		return "", err
	}

	// 遍历所有网络接口
	for _, iface := range interfaces {
		// 排除回环接口和未启用的接口
		if iface.Flags&net.FlagLoopback == 0 && iface.Flags&net.FlagUp != 0 {
			addrs, err := iface.Addrs()
			if err != nil {
				return "", err
			}

			// 遍历接口的地址
			for _, addr := range addrs {
				// 检查是否是IPv4地址
				if ipnet, ok := addr.(*net.IPNet); ok && !ipnet.IP.IsLoopback() {
					if ipnet.IP.To4() != nil {
						return ipnet.IP.String(), nil
					}
				}
			}
		}
	}
	return "", fmt.Errorf("no LAN IP address found")
}

// 使用ASCII字符绘制主要数据表格
func drawTable(protocol, ip, port, homepage string) {
	url := "http://" + ip + ":" + port + "/"
	if homepage != "None" {
		url += homepage
	}
	fmt.Println("+---------------------------------------+")
	fmt.Printf("| %-37s |\n", url)
	fmt.Println("+---------------------------------------+")
	fmt.Println("| Property         | Value              |")
	fmt.Println("+------------------+--------------------+")
	fmt.Printf("| %-16s | %-18s |\n", "Protocol", protocol)
	fmt.Printf("| %-16s | %-18s |\n", "IP Address", ip)
	fmt.Printf("| %-16s | %-18s |\n", "Port", port)
	fmt.Printf("| %-16s | %-18s |\n", "Homepage", homepage)
	fmt.Println("+------------------+--------------------+")
}

// 显示本程序基本信息
func about(){
	fmt.Println("+---------------------------------------+")
	fmt.Println("|          Static File Server           |")
	fmt.Println("+---------------------------------------+")
	fmt.Println("|                 ABOUT                 |")
	fmt.Println("+---------------------------------------+")
	fmt.Printf("| %-16s   %-18s |\n", "Version:", "1.0")
	fmt.Printf("| %-16s   %-18s |\n", "Author:", "ZENG Lai")
	fmt.Printf("| %-16s   %-18s |\n", "Last Modified:", "2025-02-28")
	fmt.Println("+---------------------------------------+")
}

func main() {
	// 定义监听的端口
	port := 80

	// 设置静态文件目录（当前目录）
	fs := http.FileServer(http.Dir("."))
	http.Handle("/", fs)

	// 获取局域网IP地址
	ip, err := getLocalIP()
	if err != nil {
		fmt.Printf("Failed to get LAN IP address: %s\n", err)
		ip = "localhost" // 如果获取IP失败，默认使用localhost
	}

	// 获取当前目录下的主页文件（假设为index.html）
	homepage := "index.html"
	if _, err := os.Stat(homepage); os.IsNotExist(err) {
		homepage = "None" // 如果文件不存在，显示“None”
	}

	// 显示服务器程序信息
	about()

	// 绘制表格
	drawTable("HTTP", ip, fmt.Sprintf("%d", port), homepage)

	fmt.Println("Press Ctrl+C to stop the server")

	go func() {
		for {
			now := time.Now().Format("2006-01-02 15:04:05") // "2006-01-02 15:04:05": Go语言参考时间格式，相当于其他编程语言中的yyyy-MM-dd HH:mm:ss
			fmt.Printf("\rCurrent Time: %s", now)
			time.Sleep(1 * time.Second)
		}
	}()

	// 监听中断信号
	sigChan := make(chan os.Signal, 1)
	signal.Notify(sigChan, os.Interrupt, syscall.SIGTERM)
	go func() {
		<-sigChan
		fmt.Println("\nServer stopped")
		os.Exit(0)
	}()

	// 开始监听
	if err := http.ListenAndServe(fmt.Sprintf(":%d", port), nil); err != nil {
		fmt.Printf("Failed to start server: %s\n", err)
	}
}