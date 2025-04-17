import { Board } from "./board-logic";
import { UpdateBoard } from "./board-layout";

class LocalBoard extends Board {
    MakeMove(coord) {
        if (!this.availableMoves[coord.index]) {
            return
        }
        this.Place(coord);
        if (this.HasWinner()) {
            this.gameState = "end";
        }
        UpdateBoard(this);
    }
}

export function StartLocalGame() {
    let state = new LocalBoard("")
    state.gameState = "player"
    UpdateBoard(state);
}
