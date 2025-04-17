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
