package board

import (
	"fmt"
	"log"
)

type Coord struct {
	Index int
	Row   int
	Col   int
	Str   string
}

func NewCoord(index int) Coord {
	row := index / 9
	col := index % 9
	str := fmt.Sprintf("%c%c", 'A'+col, '1'+row)
	return Coord{
		Index: index,
		Row:   row,
		Col:   col,
		Str:   str,
	}
}

func (c Coord) InSubBoard() int {
	return (c.Row/3)*3 + (c.Col / 3)
}

func (c Coord) ToSubBoard() int {
	return (c.Row%3)*3 + (c.Col % 3)
}

func (c Coord) ToSubBoardRow() int {
	return c.Row % 3
}

func (c Coord) ToSubBoardCol() int {
	return c.Col % 3
}

func IndexToCoord(index int) Coord {
	return NewCoord(index)
}

func CoordinatesToCoord(row, col int) Coord {
	return NewCoord(row*9 + col)
}

func StringToCoord(s string) Coord {
	if len(s) != 2 {
		log.Fatal("invalid coordinate string length")
	}
	col := int(s[0] - 'A')
	row := int(s[1] - '1')
	index := row*9 + col
	return NewCoord(index)
}
