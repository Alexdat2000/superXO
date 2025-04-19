package games

import (
	"fmt"
	"io"
	"net/http"
)

func GetBestMove(moves, bot string) string {
	resp, err := http.Get("http://solver:18080/calculate?moves=" + moves + "&bot=" + bot)
	if err != nil {
		fmt.Println("Error making request to solver:", err)
		return ""
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		fmt.Println("Error reading response from solver:", err)
		return ""
	}

	return string(body)
}
