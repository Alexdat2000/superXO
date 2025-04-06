export async function GetMoveFromServer(moves) {
    try {
        const response = await fetch('http://localhost:2052/calculate?moves=' + moves);
        if (!response.ok) {
            console.error(response.text);
            return "";
        }
        const data = await response
        return data.text();
    } catch (error) {
        console.error(error.message); // Handle any errors
        return "";
    }
}
