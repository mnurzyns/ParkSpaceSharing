-- User

CREATE TABLE IF NOT EXISTS `user`
(
    id       INTEGER PRIMARY KEY,
    email    TEXT    NOT NULL UNIQUE,
    phone    TEXT    NOT NULL UNIQUE,
    username TEXT    NOT NULL UNIQUE,
    password TEXT    NOT NULL,
    role     INTEGER NOT NULL DEFAULT 1
);

-- Place

CREATE TABLE IF NOT EXISTS place
(
    id        INTEGER PRIMARY KEY,
    owner_id  INTEGER NOT NULL,
    address   TEXT    NOT NULL,
    latitude  REAL    NOT NULL,
    longitude REAL    NOT NULL,

    FOREIGN KEY (owner_id)
        REFERENCES user (id)
);

-- Offer

CREATE TABLE IF NOT EXISTS offer
(
    id          INTEGER PRIMARY KEY,
    place_id    INTEGER NOT NULL,
    date_from   DATE    NOT NULL,
    date_to     DATE    NOT NULL,
    description TEXT    NOT NULL,
    price       INTEGER NOT NULL,

    FOREIGN KEY (place_id)
        REFERENCES place (`id`)
);

-- Indexes

CREATE INDEX IF NOT EXISTS idx_place_owner_id
    ON place (owner_id);

CREATE INDEX IF NOT EXISTS idx_offer_place_id
    ON offer (place_id);

-- FTS

CREATE VIRTUAL TABLE IF NOT EXISTS user_fts
    USING fts5
(
    email,
    phone UNINDEXED,
    username,
    password UNINDEXED,
    role UNINDEXED,
    content='user',
    content_rowid='id',
    tokenize='porter unicode61'
);

CREATE VIRTUAL TABLE IF NOT EXISTS place_fts
    USING fts5
(
    address,
    latitude UNINDEXED,
    longitude UNINDEXED,
    owner_id UNINDEXED,
    content='place',
    content_rowid='id',
    tokenize='porter unicode61'
);

CREATE VIRTUAL TABLE IF NOT EXISTS offer_fts
    USING fts5
(
    place_id UNINDEXED,
    date_from UNINDEXED,
    date_to UNINDEXED,
    description,
    price UNINDEXED,
    content='offer',
    content_rowid='id',
    tokenize='porter unicode61'
);

-- Triggers

CREATE TRIGGER IF NOT EXISTS user_ai
    AFTER INSERT
    ON user
BEGIN
    INSERT INTO user_fts (rowid, email, username)
    VALUES (new.id, new.email, new.username);
END;

CREATE TRIGGER IF NOT EXISTS user_ad
    AFTER DELETE
    ON user
BEGIN
    -- Remove the user from FTS table
    INSERT INTO user_fts (user_fts, rowid, email, username)
    VALUES ('delete', old.id, old.email, old.username);
    -- Remove dangling places
    DELETE
    FROM place
    WHERE owner_id = old.id;
END;

CREATE TRIGGER IF NOT EXISTS user_au
    AFTER UPDATE
    ON user
BEGIN
    INSERT INTO user_fts (user_fts, rowid, email, username)
    VALUES ('delete', old.id, old.email, old.username);
    INSERT INTO user_fts (rowid, email, username)
    VALUES (new.id, new.email, new.username);
END;

CREATE TRIGGER IF NOT EXISTS place_ai
    AFTER INSERT
    ON place
BEGIN
    INSERT INTO place_fts (rowid, address, owner_id)
    VALUES (new.id, new.address, new.owner_id);
END;

CREATE TRIGGER IF NOT EXISTS place_ad
    AFTER DELETE
    ON place
BEGIN
    -- Remove the place from FTS table
    INSERT INTO place_fts (place_fts, rowid, address, owner_id)
    VALUES ('delete', old.id, old.address, old.owner_id);
    -- Remove dangling offers
    DELETE
    FROM offer
    WHERE offer.place_id = old.id;
END;

CREATE TRIGGER IF NOT EXISTS place_au
    AFTER UPDATE
    ON place
BEGIN
    INSERT INTO place_fts (place_fts, rowid, address, owner_id)
    VALUES ('delete', old.id, old.address, old.owner_id);
    INSERT INTO place_fts (rowid, address, owner_id)
    VALUES (new.id, new.address, new.owner_id);
END;

CREATE TRIGGER IF NOT EXISTS offer_ai
    AFTER INSERT
    ON offer
BEGIN
    INSERT INTO offer_fts (rowid, description, place_id)
    VALUES (new.id, new.description, new.place_id);
END;

CREATE TRIGGER IF NOT EXISTS offer_ad
    AFTER DELETE
    ON offer
BEGIN
    INSERT INTO offer_fts (offer_fts, rowid, description, place_id)
    VALUES ('delete', old.id, old.description, old.place_id);
END;

CREATE TRIGGER IF NOT EXISTS offer_au
    AFTER UPDATE
    ON offer
BEGIN
    INSERT INTO offer_fts (offer_fts, rowid, description, place_id)
    VALUES ('delete', old.id, old.description, old.place_id);
    INSERT INTO offer_fts (rowid, description, place_id)
    VALUES (new.id, new.description, new.place_id);
END;
