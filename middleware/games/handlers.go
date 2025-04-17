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
	if !validateId(gameId) {
		returnError(w, http.StatusBadRequest, "Invalid game id")
		return
	}
	playerIdCookie, err := r.Cookie("userId")
	if err != nil || !validateId(playerIdCookie.Value) {
		returnError(w, http.StatusBadRequest, "Invalid player id")
		return
	}
	playerId := playerIdCookie.Value

	game, status, err := getGame(gameId, playerId)
	if errors.Is(err, GameNotFoundError) {
		returnError(w, http.StatusNotFound, "Game not found")
	} else if err != nil {
		returnError(w, http.StatusInternalServerError, "Error getting game: "+err.Error())
	} else {
		msg, _ := json.Marshal(GetGameResponce{Moves: game.Moves, GameStatus: status})
		_, _ = fmt.Fprintf(w, string(msg))
	}
}

func HandleNewGame(w http.ResponseWriter, r *http.Request) {
	playerIdCookie, err := r.Cookie("userId")
	if err != nil || !validateId(playerIdCookie.Value) {
		returnError(w, http.StatusBadRequest, "Invalid player id")
		return
	}
	playerId := playerIdCookie.Value

	id := generateRandomString(10)
	err = createGame(id, playerId)
	if err != nil {
		returnError(w, http.StatusInternalServerError, "Error creating game: "+err.Error())
		return
	}
	http.Redirect(w, r, "/game?id="+id, http.StatusSeeOther)
}

func HandlePlace(w http.ResponseWriter, r *http.Request) {
	gameId := r.URL.Query().Get("game_id")
	if !validateId(gameId) {
		returnError(w, http.StatusBadRequest, "Invalid game id")
		return
	}
	move := r.URL.Query().Get("move")
	if !('A' <= move[0] && move[0] <= 'I' && '1' <= move[1] && move[1] <= '9') {
		returnError(w, http.StatusBadRequest, "Move to non-existing cell")
		return
	}
	playerIdCookie, err := r.Cookie("userId")
	if err != nil || !validateId(playerIdCookie.Value) {
		returnError(w, http.StatusBadRequest, "Invalid player id")
		return
	}
	playerId := playerIdCookie.Value

	err = place(gameId, playerId, move)
	if errors.Is(err, GameNotFoundError) {
		returnError(w, http.StatusNotFound, "Game not found")
	} else if errors.Is(err, NoAccessToPlaceError) {
		returnError(w, http.StatusBadRequest, "Player is not in the game")
	} else if errors.Is(err, WrongTurnError) {
		returnError(w, http.StatusBadRequest, "Wrong turn")
	} else if errors.Is(err, InvalidMoveError) {
		returnError(w, http.StatusBadRequest, "Invalid move")
	} else if err != nil {
		returnError(w, http.StatusInternalServerError, "Error placing move: "+err.Error())
	} else {
		w.WriteHeader(http.StatusOK)
	}
}
