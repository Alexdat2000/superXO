import { Board } from "./board-logic";
import { UpdateBoard } from "./board-layout";
import { UpdateGameTimer } from "./game-timer";
import { GetGameFromServer, SendMoveToServer } from "./server-connection";

let state = null;

class RemoteBoard extends Board {
  MakeMove(coord) {
    if (!this.availableMoves[coord.index]) {
      return;
    }
    if (this.currentPlayer == "X") {
      this.time1at += this.time_delta;
      this.time2at = Date.now();
    } else {
      this.time2at += this.time_delta;
      this.time1at = Date.now();
    }
    this.Place(coord);
    this.gameState = "server";
    UpdateBoard(this);
    SendMoveToServer(this.gameId, coord.str);
    if (!this.HasWinner()) {
      waitForMoveLoop();
    }
  }

  UpdateTimer() {
    if (this.gameState === "init") {
      return;
    }
    if (this.currentPlayer == "X") {
      const new_time = Date.now();
      this.time1left = Math.max(0, this.time1left - (new_time - this.time1at));
      this.time1at = new_time;
      if (this.time1left == 0) {
        this.winner = "O";
        UpdateBoard(this);
      } else {
        UpdateGameTimer(this);
      }
    } else {
      const new_time = Date.now();
      this.time2left = Math.max(0, this.time2left - (new_time - this.time2at));
      this.time2at = new_time;
      if (this.time2left == 0) {
        this.winner = "X";
        UpdateBoard(this);
      } else {
        UpdateGameTimer(this);
      }
    }
  }
}

export function StartRemoteGame(gameId) {
  state = new RemoteBoard("");
  state.gameId = gameId;
  UpdateBoard(state);
  gameInitiationLoop(gameId);

  let timerInterval = setInterval(() => {
    if (state.HasWinner()) {
      clearInterval(timerInterval);
    }
    state.UpdateTimer();
  }, 100);
}

function gameInitiationLoop() {
  GetGameFromServer(state.gameId).then((game) => {
    if (game["game_initiated"] === false) {
      setTimeout(gameInitiationLoop, 300);
      return;
    }
    state.Extend(game["moves"]);
    state.time1at = game["time_1_at"];
    state.time1left = game["time_1_left"];
    state.time2at = game["time_2_at"];
    state.time2left = game["time_2_left"];
    if (game["game_status"] === "spectator") {
      state.gameState = "spectator";
      UpdateBoard(state);
      spectatorLoop();
    } else if (game["game_status"] === "player1") {
      if (state.moves.length % 4 === 0) {
        state.gameState = "player";
        UpdateBoard(state);
      } else {
        state.gameState = "server";
        UpdateBoard(state);
        if (!state.HasWinner()) {
          waitForMoveLoop();
        }
      }
    } else if (game["game_status"] === "player2") {
      if (state.moves.length % 4 === 2) {
        state.gameState = "player";
        UpdateBoard(state);
      } else {
        state.gameState = "server";
        UpdateBoard(state);
        if (!state.HasWinner()) {
          waitForMoveLoop();
        }
      }
    } else {
      alert("Unknown game status: " + game["game_status"]);
    }
  });
}

function spectatorLoop() {
  GetGameFromServer(state.gameId).then((game) => {
    if (game["moves"].length >= state.moves.length + 2) {
      state.Extend(game["moves"]);
      state.time1at = game["time_1_at"];
      state.time1left = game["time_1_left"];
      state.time2at = game["time_2_at"];
      state.time2left = game["time_2_left"];
      UpdateBoard(state);
    }
    if (!state.HasWinner()) {
      setTimeout(spectatorLoop, 300);
    }
  });
}

function waitForMoveLoop() {
  GetGameFromServer(state.gameId).then((game) => {
    if (game["moves"].length < state.moves.length + 2) {
      setTimeout(waitForMoveLoop, 300);
      return;
    }
    state.time1at = game["time_1_at"];
    state.time1left = game["time_1_left"];
    state.time2at = game["time_2_at"];
    state.time2left = game["time_2_left"];
    state.Extend(game["moves"]);
    state.gameState = "player";
    UpdateBoard(state);
  });
}
