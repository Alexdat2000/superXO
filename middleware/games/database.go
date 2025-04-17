package games

import (
	"database/sql"
	"fmt"
	"log"
	"os"
	"time"

	_ "github.com/lib/pq"
)

var DB *sql.DB

func getDBConnectionString() string {
	host := os.Getenv("DB_HOST")
	port := os.Getenv("DB_PORT")
	user := os.Getenv("DB_USER")
	password := os.Getenv("DB_PASSWORD")
	dbname := os.Getenv("DB_NAME")
	sslmode := os.Getenv("DB_SSL_MODE")

	return fmt.Sprintf("host=%s port=%s user=%s password=%s dbname=%s sslmode=%s",
		host, port, user, password, dbname, sslmode)
}

func InitDB() {
	connStr := getDBConnectionString()

	maxRetries := 10
	var err error
	for i := 0; i < maxRetries; i++ {
		DB, err = sql.Open("postgres", connStr)
		if err != nil {
			log.Printf("Failed to open DB connection: %v", err)
			time.Sleep(time.Second)
			continue
		}

		err = DB.Ping()
		if err == nil {
			break
		}

		log.Printf("Failed to ping DB (attempt %d/%d): %v", i+1, maxRetries, err)
		time.Sleep(time.Second)
	}
	if err != nil {
		log.Fatalf("Failed to connect to the database after %d attempts: %v", maxRetries, err)
	}
	log.Println("Connected to the database!")
}
