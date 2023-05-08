-- User

CREATE TABLE IF NOT EXISTS `user`(
    `id`       INTEGER  PRIMARY KEY,
    `email`    TEXT     NOT NULL UNIQUE,
    `username` TEXT     NOT NULL UNIQUE,
    `password` TEXT     NOT NULL,
    `role`     INTEGER  NOT NULL DEFAULT 1
);

-- Place

CREATE TABLE IF NOT EXISTS `place`(
    `id`        INTEGER PRIMARY KEY,
    `owner_id`  INTEGER NOT NULL,
    `address`   TEXT    NOT NULL,
    `latitude`  REAL    NOT NULL,
    `longitude` REAL    NOT NULL,

    FOREIGN KEY(`owner_id`)
        REFERENCES user(`id`)
);

-- Offer

CREATE TABLE IF NOT EXISTS `offer`(
    `id`            INTEGER PRIMARY KEY,
    `place_id`      INTEGER NOT NULL,
    `date_from`     DATE    NOT NULL,
    `date_to`       DATE    NOT NULL,
    `description`   TEXT    NOT NULL,
    `price`         INTEGER NOT NULL,

    FOREIGN KEY(`place_id`)
        REFERENCES place(`id`)
);

-- Indexes

CREATE INDEX IF NOT EXISTS `idx_place_owner_id`
    ON place(`owner_id`);

CREATE INDEX IF NOT EXISTS `idx_offer_place_id`
    ON offer(`place_id`);

-- FTS

CREATE VIRTUAL TABLE IF NOT EXISTS `user_fts`
    USING fts5(`user_id`, `user_email`, `user_username`, tokenize="porter unicode61");

CREATE VIRTUAL TABLE IF NOT EXISTS `place_fts`
    USING fts5(`place_id`, `place_address`, `owner_id`, `owner_email`, `owner_username`, tokenize="porter unicode61");

CREATE VIRTUAL TABLE IF NOT EXISTS `offer_fts`
    USING fts5(`offer_id`, `offer_description`, `place_id`, `place_address`, `owner_id`, `owner_email`, `owner_username`, tokenize="porter unicode61");

-- Triggers

CREATE TRIGGER IF NOT EXISTS after_insert_user AFTER INSERT ON `user`
BEGIN
INSERT INTO `user_fts`(`user_id`, `user_email`, `user_username`)
VALUES (new.`id`, new.`email`, new.`username`);
END;

CREATE TRIGGER IF NOT EXISTS after_update_user AFTER UPDATE ON `user`
BEGIN
REPLACE INTO `user_fts`(ROWID, `user_id`, `user_email`, `user_username`)
VALUES (ROWID, new.`id`, new.`email`, new.`username`);
END;

CREATE TRIGGER IF NOT EXISTS after_delete_user AFTER DELETE ON `user`
BEGIN
-- Remove the user from FTS table
DELETE FROM `user_fts`
WHERE `user_fts`.`user_id` = old.`id`;
-- Remove dangling places
DELETE FROM `place`
WHERE `owner_id` = old.`id`;
END;

CREATE TRIGGER IF NOT EXISTS after_insert_place AFTER INSERT ON `place`
BEGIN
INSERT INTO `place_fts`(`place_id`, `place_address`, `owner_id`, `owner_email`, `owner_username`)
SELECT
    new.`id` AS `place_id`,
    new.`address` AS `place_address`,
    new.`owner_id`,
    `user_fts`.`user_email` AS `owner_email`,
    `user_fts`.`user_username` AS `owner_username`
FROM `user_fts`
WHERE `user_fts`.`user_id` = new.`owner_id`;
END;

CREATE TRIGGER IF NOT EXISTS after_update_place AFTER UPDATE ON `place`
BEGIN
    REPLACE INTO `place_fts`(ROWID, `place_id`, `place_address`, `owner_id`, `owner_email`, `owner_username`)
SELECT
    ROWID,
    new.`id` AS `place_id`,
    new.`address` AS `place_address`,
    new.`owner_id`,
    `user_fts`.`user_email` AS `owner_email`,
    `user_fts`.`user_username` AS `owner_username`
FROM `user_fts`
WHERE `user_fts`.`user_id` = new.`owner_id` AND `place_id`;
END;

CREATE TRIGGER IF NOT EXISTS after_delete_place AFTER DELETE ON `place`
BEGIN
-- Remove the place from FTS table
DELETE FROM `place_fts`
WHERE `place_fts`.`place_id` = old.id;
-- Remove dangling offers
DELETE FROM `offer`
WHERE `place_id` = old.`id`;
END;

CREATE TRIGGER IF NOT EXISTS after_insert_offer AFTER INSERT ON `offer`
BEGIN
INSERT INTO `offer_fts`(`offer_id`, `offer_description`, `place_id`, `place_address`, `owner_id`, `owner_email`, `owner_username`)
SELECT
    new.`id` AS `offer_id`,
    new.`description` AS `offer_description`,
    new.`place_id`,
    `place_address`,
    `owner_id`,
    `owner_email`,
    `owner_username`
FROM `place_fts`
WHERE `place_fts`.`place_id` = new.`place_id`;
END;

CREATE TRIGGER IF NOT EXISTS after_update_offer AFTER UPDATE ON `offer`
BEGIN
    REPLACE INTO `offer_fts`(ROWID, `offer_id`, `offer_description`, `place_id`, `place_address`, `owner_id`, `owner_email`, `owner_username`)
SELECT
    ROWID,
    new.`id` AS `offer_id`,
    new.`description` AS `offer_description`,
    new.`place_id`,
    `place_address`,
    `owner_id`,
    `owner_email`,
    `owner_username`
FROM `place_fts`
WHERE `place_fts`.`place_id` = new.place_id;
END;

CREATE TRIGGER IF NOT EXISTS after_delete_offer AFTER DELETE ON `offer`
BEGIN
DELETE FROM `offer_fts`
WHERE `offer_fts`.`place_id` = old.`id`;
END;
