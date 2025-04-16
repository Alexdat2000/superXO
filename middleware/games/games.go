package games

import (
	"database/sql"
	"errors"
	"log"
	"time"
)

var GameNotFoundError = errors.New("Game not found")

type Game struct {
	ID        string
	Player1   string
	Player2   string
	Moves     string
	CreatedAt time.Time
	LastMove  time.Time
}

func getGame(gameID string) (*Game, error) {
	query := `
        SELECT id, player1, player2, moves, created_at, last_move
        FROM games
        WHERE id = $1
    `

	row := DB.QueryRow(query, gameID)

	var game Game
	err := row.Scan(
		&game.ID,
		&game.Player1,
		&game.Player2,
		&game.Moves,
		&game.CreatedAt,
		&game.LastMove,
	)
	if game.Player1 == "" {
		log.Fatalf("Player1 for game %s is empty", gameID)
	}
	if err != nil {
		if err == sql.ErrNoRows {
			return nil, GameNotFoundError
		}
		return nil, err
	}

	return &game, nil
}

func createGame(gameID, player1 string) error {
	if len(gameID) != 10 {
		log.Fatalf("Invalid game id: %s", gameID)
	}
	if len(player1) != 10 {
		log.Fatalf("Invalid player id: %s", player1)
	}

	query := `
        INSERT INTO games (id, player1)
        VALUES ($1, $2)
    `
	_, err := DB.Exec(query, gameID, player1)
	if err != nil {
		return err
	}
	return nil
}

var OpponentAlreadyFound = errors.New("Opponent already found")

func matchmake(gameID, player2 string) error {
	query := `
	UPDATE games
	SET player2 = $1
	WHERE id = $2 AND (player2 IS NULL OR player2 = '')
`
	result, err := DB.Exec(query, player2, gameID)
	if err != nil {
		return err
	}
	rowsAffected, err := result.RowsAffected()
	if err != nil {
		return err
	}
	if rowsAffected == 0 {
		return OpponentAlreadyFound
	}
	return nil
}
