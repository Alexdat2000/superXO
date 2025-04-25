package main

import (
	"flag"
	"log"
	"net/http"
	"os"

	"github.com/gorilla/handlers"
	"github.com/gorilla/mux"

	gamesApi "github.com/Alexdat2000/superXO/middleware/games"
)

func main() {
	listen := flag.String("port", ":8080", "Port to listen on")
	flag.Parse()

	router := mux.NewRouter()

	router.HandleFunc("/health", func(w http.ResponseWriter, r *http.Request) {
		w.WriteHeader(http.StatusOK)
		_, _ = w.Write([]byte("OK"))
	})

	// router.HandleFunc("/calculate", handleCalculate)
	router.HandleFunc("/getGame", gamesApi.HandleGetGame)
	router.HandleFunc("/newGame", gamesApi.HandleNewGame)
	router.HandleFunc("/place", gamesApi.HandlePlace)

	gamesApi.InitDB()

	log.Printf("Listening on %s", *listen)
	loggedRouter := handlers.LoggingHandler(os.Stdout, router)
	err := http.ListenAndServe("0.0.0.0"+*listen, loggedRouter)
	if err != nil {
		log.Fatalf("Error starting server: %v", err)
	}
}
