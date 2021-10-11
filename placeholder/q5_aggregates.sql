select categoryname, (select count(*) from product p, category c1 where p.categoryid==c1.id and c1.categoryname==c2.categoryname) count1,
round((select avg(unitprice) from product p, category c1 where p.categoryid==c1.id and c1.categoryname==c2.categoryname),2) avg,
(select min(unitprice) from product p, category c1 where p.categoryid == c1.id and c1.categoryname == c2.categoryname) min,
(select max(unitprice) from product p, category c1 where p.categoryid == c1.id and c1.categoryname == c2.categoryname) max,
(select sum(unitprice) from product p, category c1 where p.categoryid == c1.id and c1.categoryname == c2.categoryname and unitsonorder>=10 ) count2
from category c2 
order by c2.id;
