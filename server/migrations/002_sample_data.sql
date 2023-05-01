INSERT INTO `user`(`email`, `username`, `password`, `role`)
VALUES
    ('bob@example.com', 'bob', 'passw0', 1),
    ('tom@example.com', 'tom', 'passw1', 1),
    ('admin@example.com', 'admin', 'admin', 0);

INSERT INTO `place`(`owner_id`, `address`, `latitude`, `longitude`)
VALUES
    (1, '123 Main St', 37.7749, -122.4194),
    (2, '456 Oak St', 37.7749, -122.4194);

INSERT INTO offer(`place_id`, `description`, `price`, `date_from`, `date_to`)
VALUES
    (1, 'Parking spot in a gated lot.', 10000, 1514804400, 1517396400),
    (2, 'Parking spot in a garage.', 20000, 1517482800, 1519815600);