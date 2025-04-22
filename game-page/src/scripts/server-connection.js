export async function GetGameFromServer(gameId) {
    try {
        const response = await fetch('/api/getGame?id=' + gameId);
        if (!response.ok) {
            alert(response.json()["error"]);
            return ""
        }
        return response.json();
    } catch (error) {
        alert(error.message);
        return "";
    }
}

export async function SendMoveToServer(gameId, move) {
    try {
        const response = await fetch('/api/place?id=' + gameId + "&move=" + move);
        if (!response.ok) {
            alert(response.json()["error"]);
        }
    } catch (error) {
        alert(error.message);
    }
}
