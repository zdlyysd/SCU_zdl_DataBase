select distinct shipname, substr(shipname,0,instr(shipname,'-')) as first_part from 'order' where shipname like '%-%' order by shipname;
