import cross from "../assets/cross.svg"
import circle from "../assets/circle.svg"
import moveOrange from "../assets/move-orange.svg"
import moveBlue from "../assets/move-blue.svg"

import {calcDeadArea, getAvailableMoves, place} from "./game-logic";

export function update_field(fieldString, currentPlayer) {
    const chessboard = document.getElementById("chessboard");
    chessboard.innerHTML = "";
    // Generate the 9x9 chessboard dynamically
    for (let row = 0; row < 9; row++) {
        for (let col = 0; col < 9; col++) {
            const cell = document.createElement("div");
            cell.className = "cell";
            cell.id = `cell-${row}-${col}`; // Assign unique ID to each cell
            chessboard.appendChild(cell);
        }
    }

    if (fieldString.length !== 81) {
        console.error("Field string must be 81 characters long.");
        return;
    }

    const cells = document.querySelectorAll(".cell");
    for (let i = 0; i < cells.length; i++) {
        const cell = cells[i];
        const fieldValue = fieldString[i];
        cell.innerHTML = ""; // Clear existing content

        if (fieldValue === "X") {
            const crossImage = document.createElement("img");
            crossImage.src = cross;
            crossImage.alt = "Cross";
            crossImage.classList.add("tic-tac-toe-image");
            cell.appendChild(crossImage);
        } else if (fieldValue === "O") {
            const circleImage = document.createElement("img");
            circleImage.src = circle;
            circleImage.alt = "Circle";
            circleImage.classList.add("tic-tac-toe-image");
            cell.appendChild(circleImage);
        }
    }

    const deadArea = calcDeadArea()
    for (let rowStart = 0; rowStart <= 6; rowStart += 3) {
        for (let colStart = 0; colStart <= 6; colStart += 3) {
            for (let i = 0; i < 3; i++) {
                for (let j = 0; j < 3; j++) {
                    const index = (rowStart + i) * 9 + (colStart + j);
                    if (deadArea[rowStart + colStart / 3] === " ") {
                        continue;
                    }
                    cells[index].classList.add("dead-area");
                    if (deadArea[rowStart + colStart / 3] === "X") {
                        cells[index].classList.add("dead-area-orange");
                    } else if (deadArea[rowStart + colStart / 3] === "O") {
                        cells[index].classList.add("dead-area-blue");
                    }
                }
            }
        }
    }

    const availableMoves = getAvailableMoves();
    for (let i = 0; i < cells.length; i++) {
        const cell = cells[i];

        if (availableMoves[i]) {
            const crossImage = document.createElement("img");
            if (currentPlayer === "X") {
                crossImage.src = moveOrange;
            } else {
                crossImage.src = moveBlue;
            }
            crossImage.alt = "can move";
            crossImage.classList.add("tic-tac-toe-image");
            cell.classList.add("available");
            cell.appendChild(crossImage);
        }
    }

    cells.forEach(cell => {
        cell.addEventListener("click", () => {
            if (cell.classList.contains("dead-area")) {
                return; // Do nothing if the cell has the "dead-area" class
            }
            // cellName should be in the format "cell-2-7"
            const parts = cell.id.split('-');
            const row = parseInt(parts[1]);
            const col = parseInt(parts[2]);
            place(row, col);
        });
    });
}
