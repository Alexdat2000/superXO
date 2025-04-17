import { Board } from "./board-logic";
import { UpdateBoard } from "./board-layout";
import { GetGameFromServer, SendMoveToServer } from "./server-connection";

let state = null;

class RemoteBoard extends Board {
    MakeMove(coord) {
        if (!this.availableMoves[coord.index]) {
            return
        }
        this.Place(coord)
        this.gameState = "server";
        UpdateBoard(this);
        SendMoveToServer(this.gameId, coord.str);
        if (!this.HasWinner()) {
            waitForMoveLoop();
        }
    }
}

export function StartRemoteGame(gameId) {
    state = new RemoteBoard("");
    state.gameId = gameId;
    UpdateBoard(state);
    gameInitiationLoop(gameId);
}

function gameInitiationLoop() {
    GetGameFromServer(state.gameId).then(game => {
        if (game["game_initiated"] === false) {
            setTimeout(gameInitiationLoop, 300);
            return
        }
        state.Extend(game["moves"]);
        if (game["game_status"] === "spectator") {
            state.gameState = "spectator";
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
    })
}

function spectatorLoop() {
    GetGameFromServer(state.gameId).then(game => {
        if (game["moves"].length >= state.moves.length + 2) {
            state.Extend(game["moves"]);
            UpdateBoard(state);
        }
        if (!state.HasWinner()) {
            setTimeout(spectatorLoop, 300);
        }
    })
}

function waitForMoveLoop() {
    GetGameFromServer(state.gameId).then(game => {
        if (game["moves"].length < state.moves.length + 2) {
            setTimeout(waitForMoveLoop, 300);
            return
        }
        state.Extend(game["moves"]);
        state.gameState = "player";
        UpdateBoard(state);
    })
}
