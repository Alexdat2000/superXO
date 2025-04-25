package board

import (
	"errors"
	"strings"
)

var gameRows = [][]int{
	{0, 1, 2},
	{3, 4, 5},
	{6, 7, 8},
	{0, 3, 6},
	{1, 4, 7},
	{2, 5, 8},
	{0, 4, 8},
	{2, 4, 6},
}

type Board struct {
	Moves          string
	CurrentPlayer  string
	Board          []rune
	SubBoards      string
	Winner         rune
	AvailableMoves []bool
}

func NewBoard(moves string) *Board {
	b := &Board{
		Moves: moves,
	}
	b.CurrentPlayer = "X"
	if len(moves)%4 != 0 {
		b.CurrentPlayer = "O"
	}
	b.calculateBoard()
	b.calculateSubBoards()
	b.calculateWinner()
	b.calculateAvailableMoves()
	return b
}

func (b *Board) Extend(moves string) error {
	if !strings.HasPrefix(moves, b.Moves) {
		return errors.New("local and remote games are desynced")
	}
	b.Moves = moves
	if len(b.Moves)%4 == 0 {
		b.CurrentPlayer = "X"
	} else {
		b.CurrentPlayer = "O"
	}
	b.calculateBoard()
	b.calculateSubBoards()
	b.calculateWinner()
	b.calculateAvailableMoves()
	return nil
}

func (b *Board) calculateBoard() {
	b.Board = []rune(strings.Repeat(" ", 81))
	mark := 'X'
	for i := 0; i < len(b.Moves); i += 2 {
		coord := StringToCoord(b.Moves[i : i+2])
		b.Board[coord.Index] = mark
		if mark == 'X' {
			mark = 'O'
		} else {
			mark = 'X'
		}
	}
}

func (b *Board) calculateAvailableMoves() {
	b.AvailableMoves = make([]bool, 81)
	if b.Winner != ' ' {
		return
	}
	lastMove := b.GetLastMove()
	if lastMove == nil {
		for i := range b.AvailableMoves {
			b.AvailableMoves[i] = true
		}
		return
	}

	tarSubBoard := lastMove.ToSubBoard()
	if b.SubBoards[tarSubBoard] != ' ' {
		for row := 0; row < 9; row++ {
			for col := 0; col < 9; col++ {
				coord := CoordinatesToCoord(row, col)
				if b.Board[coord.Index] == ' ' && b.SubBoards[coord.InSubBoard()] == ' ' {
					b.AvailableMoves[coord.Index] = true
				}
			}
		}
	} else {
		edgeRow := (lastMove.Row % 3) * 3
		edgeCol := (lastMove.Col % 3) * 3
		for row := edgeRow; row < edgeRow+3; row++ {
			for col := edgeCol; col < edgeCol+3; col++ {
				coord := CoordinatesToCoord(row, col)
				if b.Board[coord.Index] == ' ' {
					b.AvailableMoves[coord.Index] = true
				}
			}
		}
	}
}

func (b *Board) calculateSubBoards() {
	var sb strings.Builder
	for rowStart := 0; rowStart <= 6; rowStart += 3 {
		for colStart := 0; colStart <= 6; colStart += 3 {
			square := make([]rune, 0, 9)
			for i := 0; i < 3; i++ {
				for j := 0; j < 3; j++ {
					index := (rowStart+i)*9 + (colStart + j)
					square = append(square, b.Board[index])
				}
			}

			winner := ' '
			for _, row := range gameRows {
				if square[row[0]] != ' ' &&
					square[row[0]] == square[row[1]] &&
					square[row[0]] == square[row[2]] {
					winner = square[row[0]]
					break
				}
			}
			if winner == ' ' {
				full := true
				for _, mark := range square {
					if mark == ' ' {
						full = false
						break
					}
				}
				if full {
					winner = 'D'
				}
			}
			sb.WriteRune(winner)
		}
	}
	b.SubBoards = sb.String()
}

func (b *Board) calculateWinner() {
	b.Winner = ' '
	for _, row := range gameRows {
		a, b1, c := b.SubBoards[row[0]], b.SubBoards[row[1]], b.SubBoards[row[2]]
		if a != ' ' && a != 'D' && a == b1 && a == c {
			b.Winner = rune(a)
			break
		}
	}
	if b.Winner == ' ' {
		full := true
		for _, mark := range b.SubBoards {
			if mark == ' ' {
				full = false
				break
			}
		}
		if full {
			b.Winner = 'D'
		}
	}
}

var ErrInvalidMove = errors.New("invalid move")

func (b *Board) Place(coord Coord) error {
	if !b.AvailableMoves[coord.Index] {
		return ErrInvalidMove
	}
	b.Moves += coord.Str
	b.Board[coord.Index] = rune(b.CurrentPlayer[0])
	if b.CurrentPlayer == "X" {
		b.CurrentPlayer = "O"
	} else {
		b.CurrentPlayer = "X"
	}
	b.calculateSubBoards()
	b.calculateWinner()
	b.calculateAvailableMoves()
	return nil
}

func (b *Board) GetLastMove() *Coord {
	if len(b.Moves) == 0 {
		return nil
	}
	coord := StringToCoord(b.Moves[len(b.Moves)-2:])
	return &coord
}

func (b *Board) HasWinner() bool {
	return b.Winner != ' '
}
