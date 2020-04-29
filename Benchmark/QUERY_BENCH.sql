SELECT customer.name, orderdate
FROM customer, orders
WHERE customer.custkey = orders.custkey;
