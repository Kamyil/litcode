# 255 · Lab: Isolation Levels and Anomalies (two psql panes)

> **Difficulty:** Hard · **Topics:** READ COMMITTED, REPEATABLE READ, SERIALIZABLE, lost update, write skew, retries · **Mode:** two psql sessions side by side (`lc tmux 255`)

Every interviewer who asks about databases asks this. Do each scenario by typing in the panes in
the order shown (A1, B1, A2...). Predict the outcome **before** each step. Answers: `lc solution 255`.

## Setup (either pane)

```sql
DROP TABLE IF EXISTS accounts, doctors;
CREATE TABLE accounts (id int PRIMARY KEY, owner text, balance int NOT NULL);
INSERT INTO accounts VALUES (1, 'alice', 100), (2, 'bob', 100);
CREATE TABLE doctors (name text PRIMARY KEY, on_call bool NOT NULL);
INSERT INTO doctors VALUES ('house', true), ('wilson', true);
```

## 1. READ COMMITTED: non-repeatable reads (the default level)

| step | Pane A | Pane B |
|---|---|---|
| A1 | `BEGIN;` `SELECT balance FROM accounts WHERE id = 1;` | |
| B1 | | `UPDATE accounts SET balance = 50 WHERE id = 1;` |
| A2 | `SELECT balance FROM accounts WHERE id = 1;` | |
| A3 | `COMMIT;` | |

- **Q1.** What did A see at A1 and A2? Is there ever a *dirty read* in Postgres?

## 2. Lost update (read-modify-write in the app)

Reset: `UPDATE accounts SET balance = 100;`

| step | Pane A | Pane B |
|---|---|---|
| A1 | `BEGIN;` `SELECT balance FROM accounts WHERE id = 1;` → app computes 100 + 10 | |
| B1 | | `BEGIN;` `SELECT balance FROM accounts WHERE id = 1;` → app computes 100 + 20 |
| A2 | `UPDATE accounts SET balance = 110 WHERE id = 1;` | |
| B2 | | `UPDATE accounts SET balance = 120 WHERE id = 1;` (blocks!) |
| A3 | `COMMIT;` | |
| B3 | | (unblocks) `COMMIT;` |

- **Q2.** Final balance? Which deposit got lost? Give **three** fixes.

Try fix 1, atomic update: both sessions run `UPDATE accounts SET balance = balance + 10 WHERE id = 1;`.
Try fix 2, `SELECT ... FOR UPDATE` at A1/B1. Try fix 3: repeat the original scenario with
`BEGIN ISOLATION LEVEL REPEATABLE READ;` in both panes.

- **Q3.** With REPEATABLE READ, what error does B get at B2 (after A commits)? What must the application do?

## 3. REPEATABLE READ: a stable snapshot

| step | Pane A | Pane B |
|---|---|---|
| A1 | `BEGIN ISOLATION LEVEL REPEATABLE READ;` `SELECT sum(balance) FROM accounts;` | |
| B1 | | `INSERT INTO accounts VALUES (3, 'carol', 500);` |
| A2 | `SELECT sum(balance) FROM accounts;` `SELECT count(*) FROM accounts;` | |
| A3 | `COMMIT;` | |

- **Q4.** Did A see carol (a *phantom*)? The SQL standard allows phantoms at REPEATABLE READ. Does Postgres?

## 4. Write skew: the anomaly REPEATABLE READ doesn't stop

Rule: at least one doctor must stay on call. Each doctor checks, then goes off call.

| step | Pane A (house) | Pane B (wilson) |
|---|---|---|
| A1 | `BEGIN ISOLATION LEVEL REPEATABLE READ;` `SELECT count(*) FROM doctors WHERE on_call;` → 2, OK to leave | |
| B1 | | `BEGIN ISOLATION LEVEL REPEATABLE READ;` `SELECT count(*) FROM doctors WHERE on_call;` → 2 |
| A2 | `UPDATE doctors SET on_call = false WHERE name = 'house';` | |
| B2 | | `UPDATE doctors SET on_call = false WHERE name = 'wilson';` |
| A3 | `COMMIT;` | |
| B3 | | `COMMIT;` |

`SELECT * FROM doctors;`

- **Q5.** What happened to the invariant? Why didn't either transaction block or fail?

Reset (`UPDATE doctors SET on_call = true;`) and repeat with `BEGIN ISOLATION LEVEL SERIALIZABLE;`.

- **Q6.** Which transaction fails, at which step, with what SQLSTATE? How does Postgres detect it
  without locking everything? Name a non-SERIALIZABLE fix.

## 5. Read-only anomaly-free reporting

- **Q7.** You need a consistent report across 10 queries on a busy system without blocking writers.
  Which isolation level, and what's `SERIALIZABLE READ ONLY DEFERRABLE` for?

## Cleanup

```sql
DROP TABLE accounts, doctors;
```
