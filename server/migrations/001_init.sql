CREATE TABLE IF NOT EXISTS `user`(
    `id`       INTEGER  PRIMARY KEY,
    `username` TEXT     NOT NULL,
    `email`    TEXT     NOT NULL,
    `password` TEXT     NOT NULL,
    `token`    CHAR(50) NOT NULL,
    `admin`    BOOLEAN  NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS `parking_space`(
    `id`       INTEGER PRIMARY KEY,
    `user_id`  INTEGER NOT NULL, -- owner
    `location` TEXT    NOT NULL,

    FOREIGN KEY(`user_id`)
        REFERENCES users(`id`)
);

CREATE TABLE IF NOT EXISTS `offer`(
    `id`               INTEGER PRIMARY KEY,
    `parking_space_id` INTEGER NOT NULL,
    `description`      TEXT    NOT NULL,

    FOREIGN KEY(`parking_space_id`)
        REFERENCES parking_spaces(`id`)
);

CREATE INDEX IF NOT EXISTS `idx_parking_space_user_id`
    ON `parking_space`(`user_id`);

CREATE INDEX IF NOT EXISTS `idx_offer_parking_space_id`
    ON `offer`(`parking_space_id`);


INSERT INTO user(username, email, password, token, admin) VALUES("admin", "admin@admin", "admin", "admin", 1);