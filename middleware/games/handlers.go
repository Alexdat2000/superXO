package games

import (
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
)

type GetGameResponce struct {
	Moves      string `json:"moves"`
	Error      string `json:"error"`
	GameStatus string `json:"game_status"`
}

func returnError(w http.ResponseWriter, code int, err string) {
	w.WriteHeader(code)
	msg, _ := json.Marshal(GetGameResponce{Error: err})
	_, _ = fmt.Fprintf(w, string(msg))
}

func HandleGetGame(w http.ResponseWriter, r *http.Request) {
	gameId := r.URL.Query().Get("id")
	if gameId == "" {
		returnError(w, http.StatusBadRequest, "No game id provided")
		return
	}
	if len(gameId) != 10 {
		returnError(w, http.StatusBadRequest, "Invalid game id")
		return
	}
	playerId := r.URL.Query().Get("player_id")
	if len(playerId) != 10 {
		returnError(w, http.StatusBadRequest, "Invalid player id")
		return
	}

	game, err := getGame(gameId)
	if errors.Is(err, GameNotFoundError) {
		returnError(w, http.StatusNotFound, "Game not found")
		return
	} else if err != nil {
		returnError(w, http.StatusInternalServerError, "Error getting game: "+err.Error())
		return
	}

	if game.Player1 == playerId {
		msg, _ := json.Marshal(GetGameResponce{Moves: game.Moves, GameStatus: "player1"})
		_, _ = fmt.Fprintf(w, string(msg))
	} else if game.Player2 == playerId {
		msg, _ := json.Marshal(GetGameResponce{Moves: game.Moves, GameStatus: "player2"})
		_, _ = fmt.Fprintf(w, string(msg))
	} else if game.Player2 == "" {
		err := matchmake(gameId, playerId)
		if errors.Is(err, OpponentAlreadyFound) {
			msg, _ := json.Marshal(GetGameResponce{Moves: game.Moves, GameStatus: "spectator"})
			_, _ = fmt.Fprintf(w, string(msg))
		} else if err != nil {
			returnError(w, http.StatusInternalServerError, "Error matching: "+err.Error())
		} else {
			msg, _ := json.Marshal(GetGameResponce{Moves: game.Moves, GameStatus: "player2"})
			_, _ = fmt.Fprintf(w, string(msg))
		}
	} else {
		msg, _ := json.Marshal(GetGameResponce{Moves: game.Moves, GameStatus: "spectator"})
		_, _ = fmt.Fprintf(w, string(msg))
	}
}

func HandleNewGame(w http.ResponseWriter, r *http.Request) {
	player1 := r.URL.Query().Get("player_id")
	if player1 == "" {
		w.WriteHeader(http.StatusBadRequest)
		msg, _ := json.Marshal(GetGameResponce{Error: "No player_id provided"})
		_, _ = fmt.Fprintf(w, string(msg))
		return
	}
	id := generateRandomString(10)
	err := createGame(id, player1)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		msg, _ := json.Marshal(GetGameResponce{Error: "Error creating game: " + err.Error()})
		_, _ = fmt.Fprintf(w, string(msg))
		return
	}
	http.Redirect(w, r, "/game?id="+id, http.StatusSeeOther)
}
