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
        this.DenyMove()
        if (this.HasWinner()) {
            this.AllowMove();
            UpdateBoard(this);
            return;
        }
        UpdateBoard(this);

        GetMoveFromServer(this.moves).then(move => {
            if (move === "") {
                alert("Server error")
            }
            this.Place(StringToCoord(move));
            if (this.HasWinner()) {
                UpdateBoard(this);
                return;
            }
            this.AllowMove();
            UpdateBoard(this);
        })
    }
}

export function StartRemoteGame() {
    let state = new RemoteBoard("", true)
    UpdateBoard(state);
}
