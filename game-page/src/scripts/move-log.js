export function UpdateMoveLog(log) {
    const scoreboard = document.getElementById('move-log').getElementsByTagName('tbody')[0];

    scoreboard.innerHTML = '';
    let row1 = scoreboard.insertRow();
    let row2 = scoreboard.insertRow();

    let title1 = row1.insertCell()
    title1.classList.add('log-player1');
    title1.classList.add('log-title');
    title1.textContent = "Player 1";
    let title2 = row2.insertCell()
    title2.classList.add('log-player2');
    title2.classList.add('log-title');
    title2.textContent = "Player 2";

    for (let i = 0; i < 10; i++) {
        let cell1 = row1.insertCell();
        cell1.classList.add('log-cell');
        cell1.classList.add('log-player1');
        let cell2 = row2.insertCell();
        cell2.classList.add('log-cell');
        cell2.classList.add('log-player2');
    }

    let len = Math.min(10, Math.floor(log.length / 2))
    for (let i = 0; i < len; i++) {
        let move = log.slice(log.length - len * 2, log.length - len * 2 + 2);
        if (i % 2 === 0) {
            let cell = row1.getElementsByTagName('td')[i + 1];
            cell.textContent = move;
        } else {
            let cell = row2.getElementsByTagName('td')[i + 1];
            cell.textContent = move;
        }
    }
}
