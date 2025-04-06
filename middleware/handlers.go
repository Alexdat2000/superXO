package main

import (
	"fmt"
	"log"
	"math/rand"
	"net/http"
)

func handleCalculate(w http.ResponseWriter, r *http.Request) {
	pastMoves := r.URL.Query().Get("moves")
	board, err := ParseBoard(pastMoves)
	if err != nil {
		w.WriteHeader(http.StatusBadRequest)
		_, _ = fmt.Fprintf(w, "Invalid moves: %v", pastMoves)
		return
	}
	newMoves, err := GetValidMoves(board, pastMoves[max(0, len(pastMoves)-2):])
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		log.Printf("Error getting valid moves: %v", err)
		return
	}

	var moves []string
	for idx, move := range newMoves {
		if move {
			moves = append(moves, IndexToString(idx))
		}
	}
	if len(moves) == 0 {
		log.Printf("No valid moves: %v", pastMoves)
		w.WriteHeader(http.StatusInternalServerError)
		return
	}
	move := moves[rand.Int()%len(moves)]
	w.WriteHeader(http.StatusOK)
	_, _ = w.Write([]byte(move))
}
