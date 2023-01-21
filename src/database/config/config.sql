CREATE TABLE IF NOT EXISTS `user`(
    `id`       INTEGER PRIMARY KEY AUTOINCREMENT,
    `username` TEXT    NOT NULL,
    `password` TEXT    NOT NULL
);

CREATE TABLE IF NOT EXISTS `parking_space`(
    `id`       INTEGER PRIMARY KEY AUTOINCREMENT,
    `user_id`  INTEGER NOT NULL, -- owner
    `location` TEXT    NOT NULL,

    FOREIGN KEY(`user_id`)
        REFERENCES users(`id`)
);

CREATE TABLE IF NOT EXISTS `offer`(
    `id`               INTEGER PRIMARY KEY AUTOINCREMENT,
    `parking_space_id` INTEGER NOT NULL,
    `description`      TEXT    NOT NULL,

    FOREIGN KEY(`parking_space_id`)
        REFERENCES parking_spaces(`id`)
);

CREATE INDEX IF NOT EXISTS `idx_parking_space_user_id`
    ON `parking_space`(`user_id`);

CREATE INDEX IF NOT EXISTS `idx_offer_parking_space_id`
    ON `offer`(`parking_space_id`);

    

VACUUM
