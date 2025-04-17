package games

import (
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
)

type GetGameResponce struct {
	Moves         string `json:"moves"`
	Error         string `json:"error"`
	GameStatus    string `json:"game_status"`
	GameInitiated bool   `json:"game_initiated"`
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
		w.WriteHeader(http.StatusOK)
		msg, _ := json.Marshal(GetGameResponce{
			Moves:         game.Moves,
			GameStatus:    status,
			GameInitiated: game.Player1.Valid && game.Player2.Valid,
		})
		_, _ = fmt.Fprintf(w, string(msg))
	}
}

type MaybeErrorResponce struct {
	Error string `json:"error"`
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
		w.WriteHeader(http.StatusInternalServerError)
		msg, _ := json.Marshal(MaybeErrorResponce{Error: "Error creating game: " + err.Error()})
		_, _ = fmt.Fprintf(w, string(msg))
		return
	}
	http.Redirect(w, r, "/game?id="+id, http.StatusSeeOther)
}

func HandlePlace(w http.ResponseWriter, r *http.Request) {
	gameId := r.URL.Query().Get("id")
	if !validateId(gameId) {
		returnError(w, http.StatusBadRequest, "Invalid game id")
		return
	}
	move := r.URL.Query().Get("move")
	if !('A' <= move[0] && move[0] <= 'I' && '1' <= move[1] && move[1] <= '9') {
		w.WriteHeader(http.StatusBadRequest)
		msg, _ := json.Marshal(MaybeErrorResponce{Error: "Move to non-existing cell: " + move})
		_, _ = fmt.Fprintf(w, string(msg))
		return
	}
	playerIdCookie, err := r.Cookie("userId")
	if err != nil || !validateId(playerIdCookie.Value) {
		w.WriteHeader(http.StatusBadRequest)
		msg, _ := json.Marshal(MaybeErrorResponce{Error: "Invalid player id"})
		_, _ = fmt.Fprintf(w, string(msg))
		return
	}
	playerId := playerIdCookie.Value

	err = place(gameId, playerId, move)
	if errors.Is(err, GameNotFoundError) {
		w.WriteHeader(http.StatusNotFound)
		msg, _ := json.Marshal(MaybeErrorResponce{Error: "Game not found"})
		_, _ = fmt.Fprintf(w, string(msg))
	} else if errors.Is(err, NoAccessToPlaceError) {
		w.WriteHeader(http.StatusBadRequest)
		msg, _ := json.Marshal(MaybeErrorResponce{Error: "Player is not in the game"})
		_, _ = fmt.Fprintf(w, string(msg))
	} else if errors.Is(err, WrongTurnError) {
		w.WriteHeader(http.StatusBadRequest)
		msg, _ := json.Marshal(MaybeErrorResponce{Error: "Wrong turn"})
		_, _ = fmt.Fprintf(w, string(msg))
	} else if errors.Is(err, InvalidMoveError) {
		returnError(w, http.StatusBadRequest, "Invalid move")
		w.WriteHeader(http.StatusBadRequest)
		msg, _ := json.Marshal(MaybeErrorResponce{Error: "Wrong turn"})
		_, _ = fmt.Fprintf(w, string(msg))
	} else if err != nil {
		returnError(w, http.StatusInternalServerError, "Error placing move: "+err.Error())
	} else {
		w.WriteHeader(http.StatusOK)
	}
}
