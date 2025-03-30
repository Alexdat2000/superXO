import {update_field} from "./field-layout";

let currentPlayer = "X"; // Start with X
let fieldString = " ".repeat(81); // Initialize the global field string
let lastMove = -1;

export function start() {
    update_field(fieldString, currentPlayer);
}

export function calcDeadArea() {
    let ans = ""
    for (let rowStart = 0; rowStart <= 6; rowStart += 3) {
        for (let colStart = 0; colStart <= 6; colStart += 3) {
            const area = [];
            for (let i = 0; i < 3; i++) {
                for (let j = 0; j < 3; j++) {
                    const index = (rowStart + i) * 9 + (colStart + j);
                    area.push(fieldString[index]);
                }
            }

            let dead = area.every(mark => mark !== " ") ? "D" : " "
            if (area[0] !== " " && area[0] === area[1] && area[0] === area[2]) {
                dead = area[0]
            }
            if (area[3] !== " " && area[3] === area[4] && area[3] === area[5]) {
                dead = area[3]
            }
            if (area[6] !== " " && area[6] === area[7] && area[6] === area[8]) {
                dead = area[6]
            }
            if (area[0] !== " " && area[0] === area[3] && area[0] === area[6]) {
                dead = area[0]
            }
            if (area[1] !== " " && area[1] === area[4] && area[1] === area[7]) {
                dead = area[1]
            }
            if (area[2] !== " " && area[2] === area[5] && area[2] === area[8]) {
                dead = area[2]
            }
            if (area[0] !== " " && area[0] === area[4] && area[0] === area[8]) {
                dead = area[0]
            }
            if (area[2] !== " " && area[2] === area[4] && area[2] === area[6]) {
                dead = area[2]
            }
            ans += dead
        }
    }
    return ans;
}

export function getAvailableMoves() {
    let ans = Array(81).fill(false);
    if (lastMove === -1) {
        ans.fill(true);
        return ans;
    }

    let deadArea = calcDeadArea();
    let wantRow = Math.floor(lastMove / 9) % 3;
    let wantCol = lastMove % 3;
    console.log(lastMove, wantRow, wantCol);
    if (deadArea[wantRow * 3 + wantCol] !== " ") {
        for (let row = 0; row < 9; row++) {
            for (let col = 0; col < 9; col++) {
                if (fieldString[row * 9 + col] === " " && deadArea[Math.floor(row / 3) * 3 + Math.floor(col / 3)] === " ") {
                    ans[row * 9 + col] = true
                }
            }
        }
    } else {
        for (let i = wantRow * 3; i < wantRow * 3 + 3; i++) {
            for (let j = wantCol * 3; j < wantCol * 3 + 3; j++) {
                if (fieldString[i * 9 + j] === " ") {
                    ans[i * 9 + j] = true
                }
            }
        }
    }
    console.log(ans);
    return ans
}

export function place(row, col) {
    const index = row * 9 + col;
    if (fieldString[index] === " " && calcDeadArea()[Math.floor(row / 3) * 3 + Math.floor(col / 3)] === " ") {
        fieldString = fieldString.substring(0, index) + currentPlayer + fieldString.substring(index + 1);
        lastMove = index;
        currentPlayer = currentPlayer === "X" ? "O" : "X";
        update_field(fieldString, currentPlayer);
    }
}
