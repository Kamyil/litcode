-- Schema for the SQL track: a small e-commerce company.
-- Only primary keys / uniques / FKs exist up front. Indexes are your job (see the labs).
SET client_min_messages = warning;
DROP TABLE IF EXISTS bookings, events, order_items, orders, products, customers, employees CASCADE;

CREATE EXTENSION IF NOT EXISTS pg_stat_statements;
CREATE EXTENSION IF NOT EXISTS pageinspect;
CREATE EXTENSION IF NOT EXISTS pg_trgm;
CREATE EXTENSION IF NOT EXISTS btree_gist;

CREATE TABLE customers (
    id          integer PRIMARY KEY,
    email       text NOT NULL UNIQUE,
    name        text NOT NULL,
    country     char(2) NOT NULL,
    plan        text NOT NULL CHECK (plan IN ('free', 'pro', 'enterprise')),
    referred_by integer REFERENCES customers (id),
    created_at  timestamptz NOT NULL
);

CREATE TABLE products (
    id       integer PRIMARY KEY,
    name     text NOT NULL,
    category text NOT NULL,
    price    numeric(10, 2) NOT NULL CHECK (price > 0),
    stock    integer NOT NULL,
    attrs    jsonb NOT NULL DEFAULT '{}'
);

CREATE TABLE orders (
    id          bigint PRIMARY KEY,
    customer_id integer NOT NULL REFERENCES customers (id),
    status      text NOT NULL CHECK (status IN ('pending', 'paid', 'shipped', 'cancelled', 'refunded')),
    created_at  timestamptz NOT NULL,
    shipped_at  timestamptz,
    total       numeric(12, 2) NOT NULL DEFAULT 0
);

CREATE TABLE order_items (
    order_id   bigint NOT NULL REFERENCES orders (id),
    product_id integer NOT NULL REFERENCES products (id),
    quantity   integer NOT NULL CHECK (quantity > 0),
    unit_price numeric(10, 2) NOT NULL,
    PRIMARY KEY (order_id, product_id)
);

CREATE TABLE employees (
    id         integer PRIMARY KEY,
    name       text NOT NULL,
    manager_id integer REFERENCES employees (id),
    department text NOT NULL,
    title      text NOT NULL,
    salary     integer NOT NULL,
    hired_at   date NOT NULL
);

CREATE TABLE events (
    id          bigint PRIMARY KEY,
    customer_id integer NOT NULL,
    type        text NOT NULL,
    payload     jsonb NOT NULL,
    occurred_at timestamptz NOT NULL
);

CREATE TABLE bookings (
    id     integer PRIMARY KEY,
    room   text NOT NULL,
    guest  text NOT NULL,
    during tstzrange NOT NULL
);
