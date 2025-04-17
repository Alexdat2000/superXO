import "../styles/field.css"
import "../styles/styles.css"
import "../styles/move-log.css"

import { EnsureUserId } from "./cookie"
import { StartLocalGame } from "./local-game"
import { StartRemoteGame } from "./remote-game"
import { GetParameterByName } from "./utils"

window.onload = function () {
    EnsureUserId()
    if (GetParameterByName("id") === null) {
        StartLocalGame()
    } else {
        StartRemoteGame(GetParameterByName("id"))
    }
}
