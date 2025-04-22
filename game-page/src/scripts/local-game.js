import { Board } from "./board-logic";
import { UpdateBoard } from "./board-layout";
import { GetParameterByName } from "./utils"

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
    const time = GetParameterByName("time");
    const parts = time.split(":");
    let base = 300;
    let add = 3;
    if (parts.length == 2) {
        base = parseInt(parts[0]);
        add = parseInt(parts[1]);
    }
    let state = new LocalBoard("", base * 1000, add * 1000)
    state.gameState = "player"
    UpdateBoard(state);

    let timerInterval = setInterval(() => {
        if (state.HasWinner()) {
            clearInterval(timerInterval);
        }
        state.UpdateTimer();
    }, 100);
}
