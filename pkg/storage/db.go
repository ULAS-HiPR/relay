package storage

import (
	"database/sql"
	"log"

	_ "github.com/mattn/go-sqlite3"
)

var db *sql.DB

// InitDB initializes the SQLite database
func InitDB() error {
	var err error
	db, err = sql.Open("sqlite3", "./satx.db")
	if err != nil {
		return err
	}
	_, err = db.Exec("CREATE TABLE IF NOT EXISTS sensor_data (id INTEGER PRIMARY KEY, data TEXT)")
	if err != nil {
		return err
	}
	log.Println("Database initialized")
	return nil
}

// SaveData saves sensor data to the database
func SaveData(data map[string]string) error {
	tx, err := db.Begin()
	if err != nil {
		return err
	}
	stmt, err := tx.Prepare("INSERT INTO sensor_data (data) VALUES (?)")
	if err != nil {
		return err
	}
	defer stmt.Close()

	for key, value := range data {
		_, err = stmt.Exec(key + ": " + value)
		if err != nil {
			return err
		}
	}
	err = tx.Commit()
	if err != nil {
		return err
	}
	log.Println("Data saved locally")
	return nil
}
