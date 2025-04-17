import "../styles/field.css"
import "../styles/styles.css"
import "../styles/move-log.css"

import { EnsureUserId } from "./cookie"
import { StartLocalGame } from "./local-game"
import { StartRemoteGame } from "./remote-game"

window.onload = function () {
    EnsureUserId()
    if (window.location.pathname.startsWith("/local")) {
        StartLocalGame()
    } else {
        StartRemoteGame()
    }
}
