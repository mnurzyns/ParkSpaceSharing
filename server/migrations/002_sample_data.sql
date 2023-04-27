INSERT INTO `user`(`email`, `username`, `password`, `role`)
VALUES
    ('bob@example.com', 'bob', 'passw0', 1),
    ('tom@example.com', 'tom', 'passw1', 1),
    ('admin@example.com', 'admin', 'admin', 0);

INSERT INTO `place`(`owner_id`, `address`, `latitude`, `longitude`)
VALUES
    (1, '123 Main St', 37.7749, -122.4194),
    (2, '456 Oak St', 37.7749, -122.4194);

INSERT INTO offer(`place_id`, `description`, `price`, `start_date`, `end_date`)
VALUES
    (1, 'Reserved parking spot in a gated lot.', 10000, '2018-01-01', '2018-01-31'),
    (2, 'Covered parking spot in a garage.', 20000, '2018-02-01', '2018-02-28');