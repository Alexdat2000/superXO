package games

import (
	"database/sql"
	"errors"
	"log"
	"time"

	boardApi "github.com/Alexdat2000/superXO/middleware/board"
)

var GameNotFoundError = errors.New("Game not found")
var OpponentAlreadyFoundError = errors.New("Opponent already found")
var NoAccessToPlaceError = errors.New("No access to place")
var WrongTurnError = errors.New("Wrong turn")
var InvalidMoveError = errors.New("Invalid move")

type Game struct {
	ID        string
	Player1   sql.NullString
	Player2   sql.NullString
	Moves     string
	CreatedAt time.Time
	LastMove  time.Time
}

func getGame(gameId, playerId string) (*Game, string, error) {
	query := `
        SELECT id, player1, player2, moves, created_at, last_move
        FROM games
        WHERE id = $1
    `

	row := DB.QueryRow(query, gameId)

	var game Game
	err := row.Scan(
		&game.ID,
		&game.Player1,
		&game.Player2,
		&game.Moves,
		&game.CreatedAt,
		&game.LastMove,
	)
	if !game.Player1.Valid {
		log.Fatalf("Player1 for game %s is empty", gameId)
	}
	if err != nil {
		if err == sql.ErrNoRows {
			return nil, "", GameNotFoundError
		}
		return nil, "", err
	}

	// Check if player is in the game and try to matchmake
	if game.Player1.String == playerId {
		return &game, "player1", nil
	}
	if game.Player2.Valid && game.Player2.String == playerId {
		return &game, "player2", nil
	}
	if !game.Player2.Valid {
		err := matchmake(gameId, playerId)
		if errors.Is(err, OpponentAlreadyFoundError) {
			return &game, "spectator", nil
		} else if err != nil {
			return nil, "", err
		} else {
			return &game, "player2", nil
		}
	} else {
		return &game, "spectator", nil
	}
}

func createGame(gameId, playerId, bot string) error {
	if len(gameId) != 10 {
		return errors.New("Invalid game id: " + gameId)
	}
	if len(playerId) != 10 {
		return errors.New("Invalid player id: " + playerId)
	}
	if bot != "1" && bot != "2" && bot != "3" && bot != "" {
		return errors.New("Invalid bot")
	}
	if bot != "" {
		query := `
        INSERT INTO games (id, player1, player2)
        VALUES ($1, $2, $3)
    `
		_, err := DB.Exec(query, gameId, playerId, "bot"+bot)
		if err != nil {
			return err
		}
	} else {
		query := `
        INSERT INTO games (id, player1)
        VALUES ($1, $2)
    `
		_, err := DB.Exec(query, gameId, playerId)
		if err != nil {
			return err
		}
	}
	return nil
}

func matchmake(gameId, playerId string) error {
	query := `
	UPDATE games
	SET player2 = $1
	WHERE id = $2 AND (player2 IS NULL OR player2 = '')
`
	result, err := DB.Exec(query, playerId, gameId)
	if err != nil {
		return err
	}
	rowsAffected, err := result.RowsAffected()
	if err != nil {
		return err
	}
	if rowsAffected == 0 {
		return OpponentAlreadyFoundError
	}
	return nil
}

func place(gameId, playerId, move string) (*boardApi.Board, error) {
	tx, err := DB.Begin()
	if err != nil {
		return nil, err
	}

	queryGet := `
        SELECT id, player1, player2, moves, created_at, last_move
        FROM games
        WHERE id = $1
    `
	row := tx.QueryRow(queryGet, gameId)

	var game Game
	err = row.Scan(
		&game.ID,
		&game.Player1,
		&game.Player2,
		&game.Moves,
		&game.CreatedAt,
		&game.LastMove,
	)
	if err != nil {
		tx.Rollback()
		if err == sql.ErrNoRows {
			return nil, GameNotFoundError
		}
		return nil, err
	}
	if !game.Player1.Valid || !game.Player2.Valid {
		tx.Rollback()
		return nil, errors.New("Can't place in not started game")
	}
	if (playerId != game.Player1.String) && (playerId != game.Player2.String) {
		tx.Rollback()
		return nil, NoAccessToPlaceError
	}
	if (len(game.Moves)%4 == 0 && playerId != game.Player1.String) ||
		(len(game.Moves)%4 == 2 && playerId != game.Player2.String) {
		tx.Rollback()
		return nil, WrongTurnError
	}

	board := boardApi.NewBoard(game.Moves)
	err = board.Place(boardApi.StringToCoord(move))
	if err != nil {
		println(game.Moves, move)
		tx.Rollback()
		return nil, InvalidMoveError
	}

	queryUpdate := `
		UPDATE games
		SET moves = moves || $1,
		last_move = CURRENT_TIMESTAMP
		WHERE id = $2
	`
	_, err = tx.Exec(queryUpdate, move, gameId)
	if err != nil {
		tx.Rollback()
		return nil, err
	}
	return board, tx.Commit()
}
