CREATE TABLE IF NOT EXISTS `user`(
    `id`       INTEGER  PRIMARY KEY,
    `email`    TEXT     NOT NULL UNIQUE,
    `username` TEXT     NOT NULL UNIQUE,
    `password` TEXT     NOT NULL,
    `role`     INTEGER  NOT NULL DEFAULT false
);

CREATE TABLE IF NOT EXISTS `place`(
    `id`        INTEGER PRIMARY KEY,
    `owner_id`  INTEGER NOT NULL,
    `address`   TEXT    NOT NULL,
    `latitude`  REAL    NOT NULL,
    `longitude` REAL    NOT NULL,

    FOREIGN KEY(`owner_id`)
        REFERENCES user(`id`)
);

CREATE TABLE IF NOT EXISTS `offer`(
    `id`            INTEGER PRIMARY KEY,
    `place_id`      INTEGER NOT NULL,
    `description`   TEXT    NOT NULL,
    `price`         INTEGER NOT NULL,
    `start_date`    DATE    NOT NULL,
    `end_date`      DATE    NOT NULL,

    FOREIGN KEY(`place_id`)
        REFERENCES place(`id`)
);

CREATE INDEX IF NOT EXISTS `idx_place_owner_id`
    ON place(`owner_id`);

CREATE INDEX IF NOT EXISTS `idx_offer_place_id`
    ON offer(`place_id`);

-- Insert sample users
INSERT INTO `user`(`email`, `username`, `password`, `role`)
VALUES
("bob@example.com", "bob", "passw0", 1),
("tom@example.com", "tom", "passw1", 1),
("admin@example.com", "admin", "admin", 0);

-- Insert sample parking spaces
INSERT INTO place(owner_id, address, latitude, longitude)
VALUES
(1, "123 Main St", 37.7749, -122.4194),
(2, "456 Oak St", 37.7749, -122.4194);

-- Insert sample offers
INSERT INTO offer(`place_id`, `description`, `price`, `start_date`, `end_date`)
VALUES
(1, "Reserved parking spot in a gated lot.", 10000, "2018-01-01", "2018-01-31"),
(2, "Covered parking spot in a garage.", 20000, "2018-02-01", "2018-02-28");