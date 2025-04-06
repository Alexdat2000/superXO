package main

import (
	"errors"
	"fmt"
)

func checkWin(area [9]Mark, pos1, pos2, pos3 int) bool {
	return area[pos1] != MarkEmpty && area[pos1] == area[pos2] && area[pos1] == area[pos3]
}

func calcSubWinners(board [81]Mark) (ans [9]Winner) {
	for rowStart := 0; rowStart <= 6; rowStart += 3 {
		for colStart := 0; colStart <= 6; colStart += 3 {
			var area [9]Mark
			for i := 0; i < 3; i++ {
				for j := 0; j < 3; j++ {
					index := (rowStart+i)*9 + (colStart + j)
					area[i*3+j] = board[index]
				}
			}

			dead := WinnerNone
			if checkWin(area, 0, 1, 2) {
				dead = MarkToWinner(area[0])
			}
			if checkWin(area, 3, 4, 5) {
				dead = MarkToWinner(area[3])
			}
			if checkWin(area, 6, 7, 8) {
				dead = MarkToWinner(area[6])
			}
			if checkWin(area, 0, 3, 6) {
				dead = MarkToWinner(area[0])
			}
			if checkWin(area, 1, 4, 7) {
				dead = MarkToWinner(area[1])
			}
			if checkWin(area, 2, 5, 8) {
				dead = MarkToWinner(area[2])
			}
			if checkWin(area, 0, 4, 8) {
				dead = MarkToWinner(area[4])
			}
			if checkWin(area, 2, 4, 6) {
				dead = MarkToWinner(area[4])
			}
			filled := true
			for _, val := range area {
				if val == MarkEmpty {
					filled = false
					break
				}
			}
			if filled && dead == WinnerNone {
				dead = WinnerDraw
			}
			ans[rowStart+colStart/3] = dead
		}
	}
	return
}

func GetValidMoves(board [81]Mark, lastMove string) (ans [81]bool, err error) {
	if lastMove == "" {
		for i, mark := range board {
			if mark != MarkEmpty {
				err = errors.New("no last move with non-empty board")
				return
			}
			ans[i] = true
		}
		return
	}

	subWinners := calcSubWinners(board)
	if subWinners[StringToTarget(lastMove)] == WinnerNone {
		subRow := StringToTarget(lastMove) / 3
		subCol := StringToTarget(lastMove) % 3
		for i := subRow * 3; i < subRow*3+3; i++ {
			for j := subCol * 3; j < subCol*3+3; j++ {
				idx := CoordinatesToIndex(i, j)
				if board[idx] == MarkEmpty {
					ans[idx] = true
				}
			}
		}
	} else {
		for i := 0; i < 9; i++ {
			for j := 0; j < 9; j++ {
				ans[CoordinatesToIndex(i, j)] = board[CoordinatesToIndex(i, j)] == MarkEmpty && subWinners[CoordinatesToSubboard(i, j)] == WinnerNone
			}
		}
	}
	return
}

func ValidateMove(board [81]Mark, lastMove, curMove string) error {
	moves, err := GetValidMoves(board, lastMove)
	if err != nil {
		return err
	}
	if !('A' <= curMove[0] && curMove[0] <= 'I' && '1' <= curMove[1] && curMove[1] <= '9') {
		return errors.New("invalid move")
	}
	if moves[StringToIndex(curMove)] {
		return nil
	} else {
		return errors.New("move to restricted cell")
	}
}

func ParseBoard(moves string) ([81]Mark, error) {
	if len(moves)%2 != 0 {
		return [81]Mark{}, errors.New("length should be even")
	}

	board := [81]Mark{}
	cur := 1
	var lastMove string
	for i := 0; i < len(moves); i += 2 {
		curMove := moves[i : i+2]
		err := ValidateMove(board, lastMove, curMove)
		if err != nil {
			return board, errors.New(fmt.Sprintf("move %d is invalid: %v", i/2+1, err.Error()))
		}
		board[StringToIndex(curMove)] = Mark(cur)
		cur = 3 - cur
		lastMove = curMove
	}
	return board, nil
}
