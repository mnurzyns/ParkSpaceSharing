INSERT INTO `user`(`email`, `phone`, `username`, `password`, `role`)
VALUES ('bob@example.com', '+48 555 000 111', 'bob',
        '$argon2id$v=19$m=65536,t=1,p=1$f2TGePdZVRZHs70X6ZH4xQ$bJEePpbCDj5x+E76AU/WG7l2NJdFo4B+olw01jtGSnc', -- password: passw0
        1),
       ('tom@example.com', '+48 555 000 222', 'tom',
        '$argon2id$v=19$m=65536,t=1,p=1$ckiPDphAdTCNFmgfTcY4tw$HDlGBWYluZ0YBxl2dPvY/GkGlspI8iSZnXr8Ze8D/pI', -- password: passw1
        1),
       ('admin@example.com', '+48 555 000 333', 'admin',
        '$argon2id$v=19$m=65536,t=1,p=1$rziInIOuKR9LrjujNEuCDQ$B4ZJZ4sV/fM614LVzNmwv3MWoVp3u5XImqbkee1H9g4', -- password: admin
        0);

INSERT INTO `place`(`owner_id`, `address`, `latitude`, `longitude`)
VALUES (1, '123 Main St', 37.7749, -122.4194),
       (2, '456 Oak St', 37.7749, -122.4194);

INSERT INTO offer(`place_id`, `description`, `price`, `date_from`, `date_to`)
VALUES (1, 'Parking spot in a gated lot.', 10000, 1514804400, 1517396400),
       (2, 'Parking spot in a garage.', 20000, 1517482800, 1519815600);