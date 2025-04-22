package games

import (
	"database/sql"
	"errors"
	"log"
	"strconv"
	"strings"
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
	TimeBase  sql.NullInt32
	TimeDelta sql.NullInt32
	Time1At   sql.NullInt64
	Time1Left sql.NullInt32
	Time2At   sql.NullInt64
	Time2Left sql.NullInt32
}

func getGame(gameId, playerId string) (*Game, string, error) {
	query := `
        SELECT id, player1, player2, moves, created_at, last_move, time_base, time_delta, time_1_at, time_1_left, time_2_at, time_2_left
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
		&game.TimeBase,
		&game.TimeDelta,
		&game.Time1At,
		&game.Time1Left,
		&game.Time2At,
		&game.Time2Left,
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

func createGame(gameId, playerId, bot, time string) error {
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
		return err
	} else if time != "" {
		parts := strings.Split(time, ":")
		if len(parts) != 2 {
			return errors.New("Invalid time")
		}
		base, err := strconv.Atoi(parts[0])
		if err != nil || base <= 0 || base > 60*60 {
			return errors.New("Invalid time")
		}
		add, err := strconv.Atoi(parts[1])
		if err != nil || add < 0 || add > 60 {
			return errors.New("Invalid time")
		}
		query := `
        INSERT INTO games (id, player1, time_base, time_delta)
        VALUES ($1, $2, $3, $4)
    `
		_, err = DB.Exec(query, gameId, playerId, base, add)
		return err
	} else {
		query := `
        INSERT INTO games (id, player1)
        VALUES ($1, $2)
    `
		_, err := DB.Exec(query, gameId, playerId)
		return err
	}
}

func matchmake(gameId, playerId string) error {
	var time_base, time_delta sql.NullInt32
	err := DB.QueryRow("SELECT time_base, time_delta FROM games WHERE id = $1", gameId).Scan(&time_base, &time_delta)
	if err != nil {
		return err
	}
	if !time_base.Valid || !time_delta.Valid {
		return errors.New("Invalid time for game")
	}

	query := `
	UPDATE games
	SET player2 = $1,
		time_base = $3,
		time_delta = $4,
		time_1_at = $5,
		time_2_at = $5,
		time_1_left = $3,
		time_2_left = $3
	WHERE id = $2 AND (player2 IS NULL OR player2 = '')
`
	result, err := DB.Exec(query, playerId, gameId, time_base.Int32*1000, time_delta.Int32*1000, time.Now().UnixMilli())
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
        SELECT id, player1, player2, moves, created_at, last_move, time_base, time_delta, time_1_at, time_1_left, time_2_at, time_2_left
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
		&game.TimeBase,
		&game.TimeDelta,
		&game.Time1At,
		&game.Time1Left,
		&game.Time2At,
		&game.Time2Left,
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
		tx.Rollback()
		return nil, InvalidMoveError
	}

	if !game.Time1Left.Valid {
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
	} else if len(game.Moves)%4 == 0 {
		queryUpdate := `
		UPDATE games
		SET moves = moves || $1,
		last_move = CURRENT_TIMESTAMP,
		time_1_left = $3,
		time_1_at = $4,
		time_2_at = $4
		WHERE id = $2
	`
		_, err = tx.Exec(queryUpdate, move, gameId, int64(game.Time1Left.Int32)-(time.Now().UnixMilli()-game.Time1At.Int64), time.Now().UnixMilli())
		if err != nil {
			tx.Rollback()
			return nil, err
		}
	} else {
		queryUpdate := `
		UPDATE games
		SET moves = moves || $1,
		last_move = CURRENT_TIMESTAMP,
		time_2_left = $3,
		time_1_at = $4,
		time_2_at = $4
		WHERE id = $2
	`
		_, err = tx.Exec(queryUpdate, move, gameId, int64(game.Time2Left.Int32)-(time.Now().UnixMilli()-game.Time2At.Int64), time.Now().UnixMilli())
		if err != nil {
			tx.Rollback()
			return nil, err
		}
	}

	return board, tx.Commit()
}
