export async function GetGameFromServer(gameId) {
  try {
    const response = await fetch("/api/getGame?id=" + gameId);
    if (!response.ok) {
      console.log(response.json()["error"]);
      return "";
    }
    return response.json();
  } catch (error) {
    console.log(error.message);
    return "";
  }
}

export async function SendMoveToServer(gameId, move) {
  try {
    const response = await fetch("/api/place?id=" + gameId + "&move=" + move);
    if (!response.ok) {
      console.log(response.json()["error"]);
    }
  } catch (error) {
    console.log(error.message);
  }
}
