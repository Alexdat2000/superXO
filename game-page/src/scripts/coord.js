class Coord {
    constructor(index) {
        this.index = index;
        this.row = Math.floor(index / 9);
        this.col = index % 9;
        this.str = String.fromCharCode(65 + index % 9) + String.fromCharCode(49 + index / 9);
    }

    inSubBoard() {
        return Math.floor(this.row / 3) * 3 + Math.floor(this.col / 3)
    }

    toSubBoard() {
        return (this.row % 3) * 3 + (this.col % 3);
    }

    toSubBoardRow() {
        return this.row % 3
    }

    toSubBoardCol() {
        return this.col % 3
    }
}

export function IndexToCoord(index) {
    return new Coord(index)
}

export function CoordinatesToCoord(row, col) {
    return new Coord(row * 9 + col);
}

export function StringToCoord(str) {
    return new Coord(str.charCodeAt(0) - 65 + (str.charCodeAt(1) - 49) * 9);
}
