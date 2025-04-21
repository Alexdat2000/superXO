import { UpdateBoard } from "./board-layout";
import { UpdateGameTimer } from "./game-timer";
import {
    CoordinatesToCoord,
    StringToCoord,
} from "./coord";

const gameRows = [
    [0, 1, 2],
    [3, 4, 5],
    [6, 7, 8],
    [0, 3, 6],
    [1, 4, 7],
    [2, 5, 8],
    [0, 4, 8],
    [2, 4, 6],
]

export class Board {
    constructor(moves, time_limit = null) {
        // main info
        this.moves = moves
        this.gameState = "init"
        
        this.time1at = this.time2at = Date.now()
        this.time1left = this.time2left = time_limit

        // calculated
        this.currentPlayer = this.moves.length % 4 === 0 ? "X" : "O";
        this.calculateBoard()
        this.calculateSubBoards()
        this.calculateWinner()
        this.calculateAvailableMoves()
    }

    Extend(moves) {
        if (!moves.startsWith(this.moves)) {
            alert("Local and remote games are desynced");
            return
        }
        this.moves = moves
        this.currentPlayer = this.moves.length % 4 === 0 ? "X" : "O";
        this.calculateBoard()
        this.calculateSubBoards()
        this.calculateWinner()
        this.calculateAvailableMoves()
    }

    // Internal calculations
    calculateBoard() {
        this.board = " ".repeat(81); // Initialize the global field string
        let mark = "X"
        for (let i = 0; i < this.moves.length; i += 2) {
            const coord = StringToCoord(this.moves.substring(i, i + 2));
            this.board = this.board.substring(0, coord.index) + mark + this.board.substring(coord.index + 1);
            mark = mark === "X" ? "O" : "X";
        }
    }

    calculateAvailableMoves() {
        this.availableMoves = Array(81).fill(false);
        if (this.winner != " ") {
            return
        }
        if (this.GetLastMove() === null) {
            this.availableMoves.fill(true);
            return
        }

        const lastMove = this.GetLastMove();
        const tarSubBoard = lastMove.toSubBoard();
        if (this.subBoards[tarSubBoard] !== " ") {
            for (let row = 0; row < 9; row++) {
                for (let col = 0; col < 9; col++) {
                    const coord = CoordinatesToCoord(row, col);
                    if (this.board[row * 9 + col] === " " && this.subBoards[coord.inSubBoard()] === " ") {
                        this.availableMoves[coord.index] = true;
                    }
                }
            }
        } else {
            const edge = CoordinatesToCoord((lastMove.row % 3) * 3, (lastMove.col % 3) * 3);
            for (let row = edge.row; row < edge.row + 3; row++) {
                for (let col = edge.col; col < edge.col + 3; col++) {
                    const coord = CoordinatesToCoord(row, col);
                    if (this.board[coord.index] === " ") {
                        this.availableMoves[coord.index] = true
                    }
                }
            }
        }
    }

    calculateSubBoards() {
        this.subBoards = ""
        for (let rowStart = 0; rowStart <= 6; rowStart += 3) {
            for (let colStart = 0; colStart <= 6; colStart += 3) {
                let square = [];
                for (let i = 0; i < 3; i++) {
                    for (let j = 0; j < 3; j++) {
                        const index = (rowStart + i) * 9 + (colStart + j);
                        square.push(this.board[index]);
                    }
                }

                let winner = " "
                for (const row of gameRows) {
                    if (square[row[0]] !== " " && square[row[0]] === square[row[1]] && square[row[0]] === square[row[2]]) {
                        winner = square[row[0]]
                    }
                }
                if (winner === " " && square.every(mark => mark !== " ")) {
                    winner = "D"
                }
                this.subBoards += winner;
            }
        }
    }

    calculateWinner() {
        this.winner = " "
        for (const row of gameRows) {
            if (this.subBoards[row[0]] !== " "
                && this.subBoards[row[1]] !== "D"
                && this.subBoards[row[0]] === this.subBoards[row[1]]
                && this.subBoards[row[0]] === this.subBoards[row[2]]) {
                this.winner = this.subBoards[row[0]]
            }
        }
        if (this.winner === " " && [...this.subBoards].every(mark => mark !== " ")) {
            this.winner = "D"
        }
    }

    // Update state
    Place(coord) {
        if (this.currentPlayer == 'X') {
            this.time2at = Date.now();
        } else {
            this.time1at = Date.now();
        }
        this.UpdateTimer();
        if (this.availableMoves[coord.index] === false || this.HasWinner()) {
            return
        }
        this.moves += coord.str;
        this.board = this.board.substring(0, coord.index) + this.currentPlayer + this.board.substring(coord.index + 1);
        this.currentPlayer = this.currentPlayer === "X" ? "O" : "X";

        this.calculateSubBoards();
        this.calculateWinner();
        this.calculateAvailableMoves()
    }

    // Helpers
    GetLastMove() {
        if (this.moves === "") {
            return null
        } else {
            return StringToCoord(this.moves.substring(this.moves.length - 2))
        }
    }

    HasWinner() {
        return this.winner !== " ";
    }

    // Intrgration with game logic
    MakeMove(coord) {
        throw new Error("Shouldn't be used as a base class");
    }

    UpdateTimer() {
        if (this.currentPlayer == 'X') {
            const new_time = Date.now();
            this.time1left = Math.max(0, this.time1left - (new_time - this.time1at));
            this.time1at = new_time;
            if (this.time1left == 0) {
                this.winner = 'O';
                UpdateBoard(this);
            } else {
                UpdateGameTimer(this)
            }
        } else {
            const new_time = Date.now();
            this.time2left = Math.max(0, this.time2left - (new_time - this.time2at));
            this.time2at = new_time;
            if (this.time2left == 0) {
                this.winner = 'X';
                UpdateBoard(this);
            } else {
                UpdateGameTimer(this)
            }
        }
    }
}
