import { Board } from "./board-logic";
import {UpdateBoard} from "./board-layout";

class LocalBoard extends Board {
    MakeMove(coord) {
        if (!this.availableMoves[coord.index]) {
            return
        }
        this.Place(coord);
        UpdateBoard(this);
    }
}

export function StartLocalGame() {
    let state = new LocalBoard("", true)
    UpdateBoard(state);
}
