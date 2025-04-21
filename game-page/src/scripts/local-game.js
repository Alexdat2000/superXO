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
    let state = new LocalBoard("", 300 * 1000)
    state.gameState = "player"
    UpdateBoard(state);

    let timerInterval = setInterval(() => {
        if (state.HasWinner()) {
            clearInterval(timerInterval);
        }
        state.UpdateTimer();
    }, 100);
}
