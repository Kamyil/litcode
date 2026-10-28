SELECT id, name, price
FROM products
WHERE category = 'books'
  AND stock > 0
  AND NOT (attrs @> '{"discontinued": true}')
ORDER BY price DESC, id
LIMIT 10;
