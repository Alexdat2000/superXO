import { Board } from "./board-logic";
import { UpdateBoard } from "./board-layout";
import { GetMoveFromServer } from "./server-connection";
import { StringToCoord } from "./coord";

class RemoteBoard extends Board {
    MakeMove(coord) {
        if (!this.availableMoves[coord.index]) {
            return
        }
        this.Place(coord)
        if (this.HasWinner()) {
            this.gameState = "end";
            UpdateBoard(this);
            return;
        }
        this.gameState = "server";
        UpdateBoard(this);

        GetMoveFromServer(this.moves).then(move => {
            if (move === "") {
                alert("Server error")
            }
            this.Place(StringToCoord(move));
            if (this.HasWinner()) {
                this.gameState = "end";
                UpdateBoard(this);
                return;
            }
            this.gameState = "player";
            UpdateBoard(this);
        })
    }
}

export function StartRemoteGame(gameId) {
    let state = new RemoteBoard("")
    UpdateBoard(state);
}
