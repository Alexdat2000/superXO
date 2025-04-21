import { Board } from "./board-logic";
import { UpdateBoard } from "./board-layout";

class LocalBoard extends Board {
    MakeMove(coord) {
        if (!this.availableMoves[coord.index]) {
            return
        }
        if (this.currentPlayer == 'X') {
            this.time1at += this.time_delta;
            this.time2at = Date.now();
        } else {
            this.time2at += this.time_delta;
            this.time1at = Date.now();
        }
        this.UpdateTimer();
        this.Place(coord);
        if (this.HasWinner()) {
            this.gameState = "end";
        }
        UpdateBoard(this);
    }
}

export function StartLocalGame() {
    let state = new LocalBoard("", 300 * 1000, 2000)
    state.gameState = "player"
    UpdateBoard(state);

    let timerInterval = setInterval(() => {
        if (state.HasWinner()) {
            clearInterval(timerInterval);
        }
        state.UpdateTimer();
    }, 100);
}
