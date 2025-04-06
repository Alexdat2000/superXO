package main

import "strconv"

type Mark byte

const (
	MarkEmpty Mark = iota
	MarkX
	MarkO
)

type Winner byte

const (
	WinnerNone Winner = iota
	WinnerX
	WinnerO
	WinnerDraw
)

func MarkToWinner(mark Mark) Winner {
	switch mark {
	case MarkX:
		return WinnerX
	case MarkO:
		return WinnerO
	default:
		return WinnerNone
	}
}

func StringToIndex(str string) int {
	return int(str[0]-'A') + int(str[1]-'1')*9
}

func IndexToString(index int) string {
	return string(rune('A'+index%9)) + strconv.Itoa(index/9+1)
}
func IndexToSubboard(idx int) int {
	return (idx/9)/3*3 + (idx%9)/3
}

func CoordinatesToSubboard(row, col int) int {
	return IndexToSubboard(row*9 + col)
}

func IndexToTarget(idx int) int {
	return (idx/9)%3*3 + idx%3
}

//func StringToSubBoard(str string) int {
//	return IndexToSubboard(StringToIndex(str))
//}

func StringToTarget(str string) int {
	return IndexToTarget(StringToIndex(str))
}

//func IndexToCoordinates(idx int) (int, int) {
//	return idx / 9, idx % 9
//}

func CoordinatesToIndex(row, col int) int {
	return row*9 + col
}
