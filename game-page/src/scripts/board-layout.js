import cross from "../assets/cross.svg";
import circle from "../assets/circle.svg";
import moveOrange from "../assets/move-orange.svg";
import moveBlue from "../assets/move-blue.svg";

import { CoordinatesToCoord, IndexToCoord, StringToCoord } from "./coord";
import { UpdateMoveLog } from "./move-log";
import { UpdateGameTimer } from "./game-timer";

export function UpdateBoard(state) {
  console.log(state);
  UpdateMoveLog(state.moves);
  UpdateGameTimer(state);

  const chessboard = document.getElementById("chessboard");
  chessboard.innerHTML = "";
  document.getElementById("chessboard").style.filter = "";
  for (let row = 0; row < 9; row++) {
    for (let col = 0; col < 9; col++) {
      const cell = document.createElement("div");
      cell.className = "cell";
      cell.id = "cell-" + CoordinatesToCoord(row, col).str;

      if (row === 0) {
        const textElement = document.createElement("div");
        textElement.className = "column-letter";
        textElement.textContent = String.fromCharCode(65 + col);
        cell.appendChild(textElement);
      }
      if (col === 0) {
        const textElement = document.createElement("div");
        textElement.className = "row-number";
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
    if (fieldValue === " ") {
      continue;
    }
    const image = document.createElement("img");
    image.classList.add("player-mark");
    if (fieldValue === "X") {
      image.src = cross;
      image.alt = "Cross";
    } else if (fieldValue === "O") {
      image.src = circle;
      image.alt = "Circle";
    }
    if (state.GetLastMove() !== null && i === state.GetLastMove().index) {
      cell.classList.add("last-move");
      image.classList.add("last-placed");
    }
    cell.appendChild(image);
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

  const status_div = document.getElementById("game-status");
  if (state.winner == "X") {
    status_div.textContent = "Player 1 won!";
  } else if (state.winner == "O") {
    status_div.textContent = "Player 2 won!";
  } else if (state.winner == "D") {
    status_div.textContent = "It's a draw!";
  } else if (state.gameState == "spectator") {
    status_div.textContent = "You are spectating";
  } else if (state.gameState == "init") {
    status_div.textContent = "Waiting for opponent to join";
  } else if (state.gameState == "server") {
    status_div.textContent = "Waiting for opponent's move";
  } else if (state.gameState == "player") {
    if (state.gameMode == "local") {
      if (state.currentPlayer == "X") {
        status_div.textContent = "Player 1 turn";
      } else {
        status_div.textContent = "Player 2 turn";
      }
    } else {
      status_div.textContent = "Your turn";
    }
  } else {
    return;
  }
  if (state.gameState === "spectator" || state.HasWinner()) {
    return;
  }
  if (state.gameState === "init" || state.gameState === "server") {
    document.getElementById("chessboard").style.filter = "brightness(0.9)";
    return;
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
  cells.forEach((cell) => {
    if (!cell.classList.contains("dead-area")) {
      cell.addEventListener("click", () => {
        state.MakeMove(StringToCoord(cell.id.substring(5)));
      });
    }
  });
}
