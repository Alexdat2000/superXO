import { Board } from "./board-logic";
import { UpdateBoard } from "./board-layout";
import { GetGameFromServer } from "./server-connection";

class RemoteBoard extends Board {
    MakeMove(coord) {
        if (!this.availableMoves[coord.index]) {
            return
        }
        this.Place(coord)
        this.gameState = "server";
        UpdateBoard(this);
        if (this.HasWinner()) {
            return;
        }
        SendMoveToServer(this.moves);
        waitForMoveLoop();
    }
}

export function StartRemoteGame(gameId) {
    let state = new RemoteBoard("");
    UpdateBoard(state);
    gameInitiationLoop(gameId);
}

function gameInitiationLoop(gameId) {
    GetGameFromServer(gameId).then(game => {
        console.log(game)
        if (game["game_initiated"] === false) {
            setTimeout(gameInitiationLoop, 300, gameId);
            return
        }
        let state = new RemoteBoard(game["moves"]);
        if (game["game_status"] === "spectator") {
            spectatorLoop(gameId);
        } else if (game["game_status"] === "player1") {
            if (state.moves.length % 4 === 0) {
                state.gameState = "player";
                UpdateBoard(state);
            } else {
                state.gameState = "server";
                UpdateBoard(state);
                waitForMoveLoop();
            }
        } else if (game["game_status"] === "player2") {
            if (state.moves.length % 4 === 2) {
                state.gameState = "player";
                UpdateBoard(state);
            } else {
                state.gameState = "server";
                UpdateBoard(state);
                waitForMoveLoop();
            }
        } else {
            alert("Unknown game status: " + game["game_status"]);
        }
    })
}

function spectatorLoop(gameId) {
    GetGameFromServer(gameId).then(game => {
        console.log(game)
        let state = new RemoteBoard(game["moves"]);
        state.gameState = "spectator";
        UpdateBoard(state);
        setTimeout(spectatorLoop, 300, gameId);
    })
}

function waitForMoveLoop() {
    GetGameFromServer(gameId).then(game => {
        console.log(game)
        if (game["moves"].length < state.moves.length + 2) {
            setTimeout(waitForMoveLoop, 300);
            return
        }
        let state = new RemoteBoard(game["moves"]);
        state.gameState = "player";
        UpdateBoard(state);
    })
}
