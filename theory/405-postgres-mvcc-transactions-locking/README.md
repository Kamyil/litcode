# 405 · Quiz: MVCC, Transactions & Locking

> **Difficulty:** Hard · **Topics:** ACID, isolation, MVCC, vacuum, locks, deadlocks, replication basics · **Mode:** `lc solution 405`

Best after labs 254–256 and 259.

**1.** Define each letter of ACID and name the Postgres mechanism that implements it.

**2.** "Readers don't block writers and writers don't block readers." Explain how MVCC achieves this.
What's the price you pay for it?

**3.** What are the four SQL isolation levels and which anomalies each permits **in Postgres** (not in the standard)?

**4.** Write skew: give an example, and two ways to prevent it.

**5.** A transaction has been `idle in transaction` for 3 hours. List three problems it causes.

**6.** `SELECT ... FOR UPDATE` vs `FOR NO KEY UPDATE` vs `FOR SHARE` vs `FOR KEY SHARE`: what's the point of the weaker ones?
(Hint: foreign key checks.)

**7.** How does Postgres detect deadlocks, and what does it do? How should the app respond?

**8.** You run `ALTER TABLE big ADD COLUMN x int DEFAULT 0` on PG11+. Is it instant? What about
`ALTER TABLE big ALTER COLUMN y TYPE bigint`?

**9.** What's the difference between a row lock, a table-level lock, and an advisory lock? Give a use
case for advisory locks.

**10.** Explain optimistic vs pessimistic concurrency control with a concrete SQL example of each.

**11.** Why must applications using SERIALIZABLE be prepared to retry? Which SQLSTATE codes are retryable?

**12.** What's a connection pooler (PgBouncer) for, why are Postgres connections expensive, and what
breaks in transaction pooling mode?

**13.** Synchronous vs asynchronous replication: latency, durability, and what happens on failover.

**14.** What's transaction ID wraparound and how is it prevented?
