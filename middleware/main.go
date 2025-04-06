package main

import (
	"flag"
	"log"
	"net/http"
	"strconv"
)

func main() {
	port := flag.Int("port", 8080, "Port to listen on")
	flag.Parse()

	log.Printf("Listening on %d", *port)
	http.HandleFunc("/calculate", handleCalculate)
	err := http.ListenAndServe(":"+strconv.Itoa(*port), nil)
	if err != nil {
		log.Fatalf("Error starting server: %v", err)
	}
}
