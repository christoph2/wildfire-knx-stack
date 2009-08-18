#
#  IMEX-WHERE-CLAUSE einfügen.
#
insert into imex_where_clause values(7,'project','device','where project_id = %ld');
insert into imex_where_clause values(8,'project','device_object','where device_id in ( select device_id from delta.device where project_id = %ld )');
insert into imex_where_clause values(9,'project','device_parameter','where device_id in ( select device_id from delta.device where project_id = %ld )');
insert into imex_where_clause values(10,'project','device_accessory','where device_id in ( select device_id from delta.device where project_id = %ld )');
insert into imex_where_clause values(11,'project','device_programming','where device_id in ( select device_id from delta.device where project_id = %ld )');
insert into imex_where_clause values(12,'project','subgroup_to_object','where subgroup_id in ( select subgroup_id from delta.subgroup key join delta.middlegroup key join delta.maingroup where project_id = %ld )');

commit;