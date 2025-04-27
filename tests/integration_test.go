// файл integration_test.go
package main

import (
	"crypto/tls"
	"io"
	"net/http"
	"regexp"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

func SendRequest(t *testing.T, url, user_id string) (string, string) {
	tr := &http.Transport{
		TLSClientConfig: &tls.Config{InsecureSkipVerify: true},
	}
	client := &http.Client{
		Transport: tr,
		CheckRedirect: func(req *http.Request, via []*http.Request) error {
			return http.ErrUseLastResponse
		},
	}
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		t.Fatalf("%v", err)
	}

	cookie := &http.Cookie{
		Name:  "userId",
		Value: user_id,
	}
	req.AddCookie(cookie)

	resp, err := client.Do(req)
	if err != nil {
		t.Fatalf("%v", err)
	}
	defer resp.Body.Close()

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		t.Fatalf("%v", err)
	}
	return resp.Status, string(body)
}

func TestGameFlow(t *testing.T) {
	// Creating the game
	status, resp := SendRequest(t, "https://127.0.0.1/newGame?time=300:0", "aaaaaaaaaa")
	assert.Equal(t, "303 See Other", status)
	re := regexp.MustCompile(`<a href="\/game\?id=(.{10})">See Other<\/a>\.\n`)
	match := re.FindStringSubmatch(resp)
	assert.Equal(t, len(match), 2)
	gameId := match[1]

	// Only one player for now
	status, resp = SendRequest(t, "https://127.0.0.1/api/getGame?id="+gameId, "aaaaaaaaaa")
	assert.Equal(t, "200 OK", status)
	assert.Equal(t, `{"moves":"","error":"","game_status":"player1","game_initiated":false}`, resp)

	// Seconds player joins
	status, resp = SendRequest(t, "https://127.0.0.1/api/getGame?id="+gameId, "aaaaaaaaab")
	assert.Equal(t, "200 OK", status)
	assert.Equal(t, `{"moves":"","error":"","game_status":"player2","game_initiated":false}`, resp)
	status, resp = SendRequest(t, "https://127.0.0.1/api/getGame?id="+gameId, "aaaaaaaaab")
	assert.Equal(t, "200 OK", status)
	assert.True(t, strings.HasPrefix(resp, `{"moves":"","error":"","game_status":"player2","game_initiated":true,`))

	// Second player can't make a move
	status, resp = SendRequest(t, "https://127.0.0.1/api/place?id="+gameId+"&move=E5", "aaaaaaaaab")
	assert.Equal(t, "400 Bad Request", status)
	assert.Equal(t, `{"error":"Wrong turn"}`, resp)

	// First player makes a move
	status, resp = SendRequest(t, "https://127.0.0.1/api/place?id="+gameId+"&move=E5", "aaaaaaaaaa")
	assert.Equal(t, "200 OK", status)
	assert.Equal(t, ``, resp)
	// Check the move
	status, resp = SendRequest(t, "https://127.0.0.1/api/getGame?id="+gameId, "aaaaaaaaaa")
	assert.Equal(t, "200 OK", status)
	assert.True(t, strings.HasPrefix(resp, `{"moves":"E5","error":"","game_status":"player1","game_initiated":true,`))

	// Join as a spectator
	status, resp = SendRequest(t, "https://127.0.0.1/api/getGame?id="+gameId, "aaaaaaaaac")
	assert.Equal(t, "200 OK", status)
	assert.True(t, strings.HasPrefix(resp, `{"moves":"E5","error":"","game_status":"spectator","game_initiated":true,`))

	// Try make a move as a spectator
	status, resp = SendRequest(t, "https://127.0.0.1/api/place?id="+gameId+"&move=E5", "aaaaaaaaac")
	assert.Equal(t, "400 Bad Request", status)
	assert.Equal(t, `{"error":"Player is not in the game"}`, resp)

	// Invalid cell
	status, resp = SendRequest(t, "https://127.0.0.1/api/place?id="+gameId+"&move=Q0", "aaaaaaaaab")
	assert.Equal(t, "400 Bad Request", status)
	assert.Equal(t, `{"error":"Move to non-existing cell: Q0"}`, resp)

	// Invalid move
	status, resp = SendRequest(t, "https://127.0.0.1/api/place?id="+gameId+"&move=A1", "aaaaaaaaab")
	assert.Equal(t, "400 Bad Request", status)
	assert.Equal(t, `{"error":"Invalid move"}`, resp)
}

func TestIncorrectScenarios(t *testing.T) {
	// Invalid player id
	status, resp := SendRequest(t, "https://127.0.0.1/newGame?time=300:0", "a")
	assert.Equal(t, "400 Bad Request", status)
	assert.Equal(t, `{"moves":"","error":"Invalid player id","game_status":"","game_initiated":false}`, resp)

	// Invalid game id
	status, resp = SendRequest(t, "https://127.0.0.1/api/getGame?id=notvalididforsure", "aaaaaaaaaa")
	assert.Equal(t, "400 Bad Request", status)
	assert.Equal(t, `{"moves":"","error":"Invalid game id","game_status":"","game_initiated":false}`, resp)

	// Non-existent game id (breaks the middleware)
	status, resp = SendRequest(t, "https://127.0.0.1/api/getGame?id=aaaaaaaaaa", "aaaaaaaaaa")
	assert.Equal(t, "404 Not Found", status)
	assert.Equal(t, `{"moves":"","error":"Game not found","game_status":"","game_initiated":false}`, resp)
}

// go test ./...
