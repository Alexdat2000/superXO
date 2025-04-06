package main

import (
	"flag"
	"log"
	"net/http"
)

func main() {
	listen := flag.String("port", "8080", "Port to listen on")
	flag.Parse()

	log.Printf("Listening on %s", *listen)
	http.HandleFunc("/calculate", handleCalculate)
	err := http.ListenAndServe(*listen, nil)
	if err != nil {
		log.Fatalf("Error starting server: %v", err)
	}
}
