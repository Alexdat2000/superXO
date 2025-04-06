import cross from "../assets/cross.svg"
import circle from "../assets/circle.svg"
import moveOrange from "../assets/move-orange.svg"
import moveBlue from "../assets/move-blue.svg"

import {Place} from "./board-logic";
import {CoordinatesToCoord, IndexToCoord, StringToCoord} from "./coord";

/**
 * @param {Board} state - The name to greet.
 */
export function UpdateBoard(state) {
    const chessboard = document.getElementById("chessboard");
    chessboard.innerHTML = "";
    for (let row = 0; row < 9; row++) {
        for (let col = 0; col < 9; col++) {
            const cell = document.createElement("div");
            cell.className = "cell";
            cell.id = "cell-" + CoordinatesToCoord(row, col).str;

            if (row === 0) {
                const textElement = document.createElement('div');
                textElement.className = "column-letter"
                textElement.textContent = String.fromCharCode(65 + col);
                cell.appendChild(textElement);
            }
            if (col === 0) {
                const textElement = document.createElement('div');
                textElement.className = "row-number"
                textElement.textContent = String.fromCharCode(49 + row);
                cell.appendChild(textElement);
            }
            chessboard.appendChild(cell);
        }
    }

    if (state.board.length !== 81) {
        console.error("Field string must be 81 characters long.");
        return;
    }

    const cells = document.querySelectorAll(".cell");
    for (let i = 0; i < cells.length; i++) {
        const cell = cells[i];
        const fieldValue = state.board[i];

        const image = document.createElement("img");
        if (fieldValue === "X") {
            image.src = cross;
            image.alt = "Cross";
        } else if (fieldValue === "O") {
            image.src = circle;
            image.alt = "Circle";
        }
        cell.appendChild(image);
        if (state.GetLastMove() !== null && i === state.GetLastMove().index) {
            cell.classList.add("last-move");
        }
    }

    // color dead areas
    for (let index = 0; index < 81; index++) {
        const coord = IndexToCoord(index);
        const subBoard = coord.inSubBoard();
        if (state.subBoards[subBoard] === " ") {
            continue;
        }
        cells[index].classList.add("dead-area");
        if (state.subBoards[subBoard] === "X") {
            cells[index].classList.add("dead-area-orange");
        } else if (state.subBoards[subBoard] === "O") {
            cells[index].classList.add("dead-area-blue");
        }
    }

    if (!state.moveAvailable) {
        // TODO: darken the board
        return
    }

    // put available move hints
    for (let index = 0; index < 81; index++) {
        if (!state.availableMoves[index]) {
            continue;
        }
        const cell = cells[index];
        const crossImage = document.createElement("img");
        if (state.currentPlayer === "X") {
            crossImage.src = moveOrange;
        } else {
            crossImage.src = moveBlue;
        }
        crossImage.alt = "can move";
        crossImage.classList.add("tic-tac-toe-image");
        cell.classList.add("available");
        cell.appendChild(crossImage);
    }

    // make cells clickable
    cells.forEach(cell => {
        if (!cell.classList.contains("dead-area")) {
            cell.addEventListener("click", () => {
                Place(StringToCoord(cell.id.substring(5)));
            });
        }
    });
}
