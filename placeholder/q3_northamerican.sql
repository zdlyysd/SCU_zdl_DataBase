select id,shipcountry,'NorthAmerica' as Place from 'order' where shipcountry in ('USA','Mexico','Canada') and id >= 15445
union
select id, shipcountry, 'OtherPlace' from 'order' where shipcountry not in ('USA','Mexico','Canda') and id >= 15445 order by id limit 20;
