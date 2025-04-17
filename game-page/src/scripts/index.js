import "../styles/field.css"
import "../styles/styles.css"
import "../styles/move-log.css"

import {StartLocalGame} from "./local-game"
import {StartRemoteGame} from "./remote-game"

window.onload = function () {
    if (window.location.pathname.startsWith("/local")) {
        StartLocalGame()
    } else {
        StartRemoteGame()
    }
}
