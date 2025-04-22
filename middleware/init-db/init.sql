CREATE TABLE IF NOT EXISTS games (
    id VARCHAR(10) NOT NULL PRIMARY KEY,
    player1 VARCHAR(10),
    player2 VARCHAR(10),
    moves TEXT NOT NULL DEFAULT '',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_move TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    time_base INT,
    time_delta INT,
    time_1_at BIGINT,
    time_1_left INT,
    time_2_at BIGINT,
    time_2_left INT
);
