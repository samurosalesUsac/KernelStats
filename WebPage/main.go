package main

import (
    "fmt"
    "log"
    "os"
    "strconv"
    "net/http"
    "github.com/gorilla/websocket"
    "io/ioutil"
    "syscall"
)

var upgrader = websocket.Upgrader{
    ReadBufferSize:  1024,
    WriteBufferSize: 1024,
}

func homePage(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "Home Page")
}

func reader(conn *websocket.Conn) {
    for {
    // read in a message
        messageType, p, err := conn.ReadMessage()
        if err != nil {
            log.Println(err)
            return
        }
    // print out that message for clarity
        
        if(string(p) != "-1"){
            //fmt.Println("gasg",string(p))
            i, err := strconv.Atoi(string(p))
            if err != nil {
                fmt.Println("File reading error", err)
                return
            }
            killProcess(i)

        }else{
            fmt.Println(string(p))
        }
        
       
        data, err := ioutil.ReadFile("/proc/cpu_201403904")
        if err != nil {
            fmt.Println("File reading error", err)
            return
        }
        data2, err2 := ioutil.ReadFile("/proc/memo_201403904")
        if err2 != nil {
            fmt.Println("File2 reading error", err2)
            return
        }
   
        data3 := string(data) + string(data2)

        // fmt.Println(string(data3))

        if err := conn.WriteMessage(messageType, []byte(string(data3))); err != nil {
            log.Println(err)
            return
        }

    }
}

func wsEndpoint(w http.ResponseWriter, r *http.Request) {

    upgrader.CheckOrigin = func(r *http.Request) bool { return true }

    // upgrade this connection to a WebSocket
    // connection
    ws, err := upgrader.Upgrade(w, r, nil)
    if err != nil {
        log.Println(err)
    }
    // helpful log statement to show connections
    log.Println("Client Connected")

    reader(ws)

}

func setupRoutes() {
    http.HandleFunc("/", homePage)
    http.HandleFunc("/ws", wsEndpoint)
}

func main() {
    fmt.Println("Hello World")
    
    setupRoutes()
    log.Fatal(http.ListenAndServe(":8080", nil))
}

func killProcess(pid int) error {
	process, err := os.FindProcess(pid)
	if err != nil {
		return err
	}

	err = process.Signal(syscall.Signal(0)) // if nil then is ok to kill

	if err != nil {
		return err
	}

	err = process.Kill()

	if err != nil {
		return err
	}

	return nil
}