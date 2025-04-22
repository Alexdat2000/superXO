export function UpdateGameTimer(state) {
  const scoreboard = document
    .getElementById("move-log")
    .getElementsByTagName("tbody")[0];
  let row1 = scoreboard.getElementsByTagName("tr")[0];
  let row2 = scoreboard.getElementsByTagName("tr")[1];

  let timer1 =
    row1.getElementsByTagName("td")[row1.getElementsByTagName("td").length - 1];
  let minutes1 = Math.floor(state.time1left / (60 * 1000));
  let seconds1 = Math.floor(state.time1left / 1000) % 60;
  timer1.textContent =
    minutes1.toString().padStart(2, "0") +
    ":" +
    seconds1.toString().padStart(2, "0");
  let timer2 =
    row2.getElementsByTagName("td")[row2.getElementsByTagName("td").length - 1];
  let minutes2 = Math.floor(state.time2left / (60 * 1000));
  let seconds2 = Math.floor(state.time2left / 1000) % 60;
  timer2.textContent =
    minutes2.toString().padStart(2, "0") +
    ":" +
    seconds2.toString().padStart(2, "0");

  if (state.currentPlayer === "X") {
    timer1.classList.add("current-player");
  } else {
    timer2.classList.add("current-player");
  }
}
