# 012 · Best Time to Buy and Sell Stock

> **Difficulty:** Easy · **Topics:** sliding window, running min · **Target:** O(n)

`prices[i]` is the price on day `i`. Buy on one day and sell on a later day. Return the
maximum profit, or `0` if no profit is possible.

Part two, `maxProfitUnlimited(prices)`: you may complete as many transactions as you like
(but hold at most one share at a time).

## Examples

```
maxProfit([7, 1, 5, 3, 6, 4])           -> 5   (buy 1, sell 6)
maxProfit([7, 6, 4, 3, 1])              -> 0
maxProfitUnlimited([7, 1, 5, 3, 6, 4])  -> 7   (1->5, 3->6)
```
