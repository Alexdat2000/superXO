package main

import (
	"flag"
	"log"
	"net/http"

	"github.com/gorilla/mux"
	"github.com/rs/cors"

	gamesApi "github.com/Alexdat2000/superXO/middleware/games"
)

func main() {
	listen := flag.String("port", ":8080", "Port to listen on")
	flag.Parse()

	router := mux.NewRouter()
	handler := cors.New(cors.Options{
		AllowedOrigins: []string{"*"},
		AllowedMethods: []string{"GET"},
	}).Handler(router)

	router.HandleFunc("/calculate", handleCalculate)
	router.HandleFunc("/getGame", gamesApi.HandleGetGame)
	router.HandleFunc("/newGame", gamesApi.HandleNewGame)

	gamesApi.InitDB()

	log.Printf("Listening on %s", *listen)
	err := http.ListenAndServe(*listen, handler)
	if err != nil {
		log.Fatalf("Error starting server: %v", err)
	}
}
