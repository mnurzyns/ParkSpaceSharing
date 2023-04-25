CREATE TABLE IF NOT EXISTS `user`(
    `id`       INTEGER  PRIMARY KEY,
    `email`    TEXT     NOT NULL UNIQUE,
    `username` TEXT     NOT NULL UNIQUE,
    `password` TEXT     NOT NULL,
    `role`     INTEGER  NOT NULL DEFAULT 1
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
