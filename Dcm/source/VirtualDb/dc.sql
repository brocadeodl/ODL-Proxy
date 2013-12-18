BEGIN TRANSACTION;
PRAGMA foreign_keys = ON;

CREATE TABLE `datacenters` (
  `id`         char(20), --MOR
  `name`       char(20),
  `createTS`   int default '0',
  PRIMARY      KEY(`id`)
  );

CREATE TABLE `hosts` (
  `id`         char(20), --MOR
  `name`       char(20),
  `dc_id`      char(20),
  `createTS`   int default '0',
  PRIMARY      KEY(`id`),
  FOREIGN KEY(`dc_id`) REFERENCES datacenters(`id`) ON DELETE CASCADE
);

CREATE TABLE `vmnics` (
  `name`       char(20),
  `mac`        char(20),
  `host_id`    char(20),
  `iface`      char(20),
  `dvs`        char(20),--dvs or vswitch id
  `dvs_nn`     char(20),--to avoid a join
  `dc_id`      char(20), 
  `createTS`   int default '0',
  CONSTRAINT pk_VmnicId PRIMARY KEY(`name`, `host_id`),
  FOREIGN KEY(`host_id`) REFERENCES hosts(`id`) ON DELETE CASCADE
  );

CREATE       TABLE `vms`   (
  `id`         char(20),--MOR
  `name`       char(20),
  `host_id`    char(20),
  `host_nn`    char(20),-- to avoid a join
  `dc_id`      char(20),
  `createTS`   int  default '0',
  PRIMARY      KEY(`id`),
  FOREIGN KEY(`host_id`) REFERENCES hosts(`id`) ON DELETE CASCADE,
  FOREIGN KEY(`dc_id`) REFERENCES datacenters(`id`) ON DELETE CASCADE
);

CREATE TABLE `vnics` (
    `name` char(20),
--vnics have a key (4000, ...) that even if mac is the same the key is unique on vm
    `mac` char(20),
    `ip`  char(20),
    `dvpg_nn` char(20), --to avoid join
    `port_prof` char(20),
    `vm_id` char(20),
    `dvs` char(20), --dvs or vswitch id
    `createTS` int default '0',
    `dvpg_id` char(20), --pg or dvpg id
    `dc_id`      char(20),
    `status` int default '1',
    CONSTRAINT pk_VnicId PRIMARY KEY (`name`, `vm_id`),
    FOREIGN KEY(`dc_id`) REFERENCES datacenters(`id`) ON DELETE CASCADE,
    FOREIGN KEY (`vm_id`) REFERENCES vms (`id`) ON DELETE CASCADE
--    FOREIGN KEY (`dvs`) REFERENCES dvss(`id`)
);

--vmknics and console nics
CREATE TABLE `special_nics` (
  `device`     char(20),
   --host.config.network.[vnic | consoleVnic].device for vmknics/console nics
  `mac`        char(20),
  `ip`         char(20),
  `pg_nn`      char(20),-- host.config.network[vnic | consoleVnic].portgroup
  `port_prof`  char(20),-- to avoid a join
  `host_id`    char(20),
  `vs`         char (20),--vswitch name
  `vnic_type`  char(20),-- vmknic or console
  `dc_id`      char(20),
  `dvpg_id`    char(20), -- dvpgid in case of no pg_nn
  `status`      int default '1',
  CONSTRAINT pk_VnicId PRIMARY KEY(`device`, `host_id`),
  FOREIGN KEY(`host_id`) REFERENCES hosts(`id`) ON DELETE CASCADE,
  FOREIGN KEY(`dc_id`) REFERENCES datacenters(`id`) ON DELETE CASCADE,
  CONSTRAINT fk_VsId FOREIGN KEY (`vs`, `host_id`) REFERENCES vss (`name`, `host_id`) ON DELETE CASCADE,
  CONSTRAINT fk_SpecialPg FOREIGN KEY (`pg_nn`, `dc_id`) REFERENCES special_pgs (`pg_nn`, `dc_id`) ON DELETE CASCADE
  CONSTRAINT fk_DVPG FOREIGN KEY (`dvpg_id`, `dc_id`) REFERENCES dvportgroups (`id`, `dc_id`) ON DELETE CASCADE
  );

--vmknics and console nic portgroups
CREATE TABLE `special_pgs` (
    `pg_nn`      char(20),-- host.config.network[vnic | consoleVnic].portgroup
    `port_prof`  char(20),-- to avoid a join
     `host_id`     char(20),
	 `host_nn`     char(20),-- to avoid a join
	 `vs`          char(20),-- vswitch name
	 `vlan`        char(20),
    `dc_id`      char(20),
    `status`      int default '1',
    CONSTRAINT pk_SpecialPg PRIMARY KEY(`pg_nn`, `host_id`, `dc_id`),
    CONSTRAINT fk_VsId FOREIGN KEY (`vs`, `host_id`) REFERENCES vss (`name`, `host_id`) ON DELETE CASCADE,
    FOREIGN KEY(`dc_id`) REFERENCES datacenters(`id`) ON DELETE CASCADE
 );

CREATE TABLE `dvss` (
  `id`        char(20),--MOR
  `name`      char(20),
  `pgroup_nn` char(20),-- uplink pg only
  `dc_id`     char(20),
  `createTS`  int      default '0',
  `pgroup_id` char(20),-- uplink id
  PRIMARY     KEY(`id`),
  FOREIGN KEY(`dc_id`) REFERENCES datacenters(`id`) ON DELETE CASCADE
  );

CREATE TABLE `dvportgroups` (
  `id`       char(20),--MOR
  `name`     char(20),
  `dvs_nn`   char(20),-- to avoid a join
  `vlan`     char(20),
  `qos`      char(20),
  `createTS` int default '0',
  `dvs_id`   char(20),
  `dc_id`    char(20),
  `port_prof` char(20),
  `nrpk`       char(20),
  `dvpgtype`       char(20),
  `status`   int default '1',
  PRIMARY    KEY(`id`),
  FOREIGN KEY(`dvs_id`) REFERENCES dvss(`id`) ON DELETE CASCADE,
  FOREIGN KEY(`dc_id`) REFERENCES datacenters(`id`) ON DELETE CASCADE
);

CREATE TABLE `vss` (
  `name`     char(20),--config.network.vswitch.name
  `host`     char(20),-- to avoid a join
  `createTS` int default '0',
  `host_id`  char(20),
  `dc_id`      char(20),
  CONSTRAINT pk_VsId PRIMARY KEY (`name`, `host_id`),
  FOREIGN KEY(`host_id`) REFERENCES hosts(`id`) ON DELETE CASCADE
);

CREATE TABLE `networks` (
  `id`       char(20), --MOR
  `name`     char(20),
  `vlan`     char(20),-- configured vlan in pp
  `createTS` int default '0',
  `dc_id`     char(20),
  `port_prof` char(20),
  `status`   int default '1',
  PRIMARY    KEY(`id`),
  FOREIGN KEY(`dc_id`) REFERENCES datacenters(`id`) ON DELETE CASCADE
);

CREATE TABLE `pgs_hosts_vss` (
  `pg_id`       char(20),
  `host_id`     char(20),
  `host_nn`     char(20),-- to avoid a join
  `vs`          char(20),-- vswitch name
  `dc_id`      char(20),
  `vlan`        char(20),
  CONSTRAINT pk_PgHostId PRIMARY KEY (`pg_id`, `host_id`),
  CONSTRAINT fk_VsId FOREIGN KEY (`vs`, `host_id`) REFERENCES vss (`name`, `host_id`) ON DELETE CASCADE,
  FOREIGN KEY(`pg_id`) REFERENCES networks(`id`) ON DELETE CASCADE
);

CREATE VIEW `pgs_hosts_networks`
AS
SELECT v.id AS id, v.name AS name, n.vlan AS vlan, v.port_prof AS port_prof, n.host_nn AS host_nn, n.vs AS vs, n.dc_id AS dc_id
FROM pgs_hosts_vss AS n LEFT JOIN networks AS v
ON (n.pg_id = v.id) 
UNION
select pg_nn as id, pg_nn as name , vlan as vlan, port_prof as port_prof, host_nn as host_nn, vs as vs, dc_id as dc_id 
FROM special_pgs
ORDER BY name ASC;

CREATE VIEW `vms_vnics_vmkernel`
AS
SELECT v.dc_id AS dc_id, n.mac AS mac, v.name AS name,  n.dvpg_nn AS dvpg_nn, n.port_prof AS port_prof, "VNIC" AS type
FROM vnics AS n LEFT JOIN vms AS v
ON ((n.vm_id = v.id) and (n.dc_id=v.dc_id))
UNION
SELECT n.dc_id AS dc_id, n.mac AS mac, '-' AS name,  n.pg_nn AS dvpg_nn, n.port_prof AS port_prof, n.vnic_type AS type
FROM special_nics AS n
ORDER BY mac ASC;

CREATE VIEW `vms_vnics`
AS
SELECT v.id AS id, n.mac AS mac, v.name AS name, n.ip AS ip, v.host_nn AS host_nn, n.dvpg_nn AS dvpg_nn, n.port_prof as port_prof, n.dc_id as dc_id
FROM vnics AS n JOIN vms AS v
ON (n.vm_id = v.id) 
ORDER BY v.name ASC;

CREATE VIEW `dvss_vmnics`
AS
SELECT d.id AS id, d.dc_id AS dc_id, d.name AS name, d.pgroup_nn AS pgroup_nn, n.host AS host, n.pnic AS pnic, n.iface AS iface
From dvss AS d LEFT JOIN
    (SELECT v.name AS pnic, v.dvs AS dvs, v.iface AS iface, h.name AS host
    FROM vmnics as v join hosts as h
    ON (v.host_id = h.id)) AS n
ON (d.id = n.dvs) 
ORDER BY d.name ASC, host ASC;

CREATE VIEW `hosts_vmnics`
AS
SELECT h.id AS id, h.dc_id AS dc_id, h.name AS name, v.name AS vmnic, v.mac AS mac, v.dvs_nn AS dvs, v.iface AS iface
FROM hosts AS h JOIN vmnics AS v
WHERE (h.id = v.host_id ) 
ORDER BY h.name ASC;

CREATE VIEW `dc_view`
AS
select datacenters.id as id, datacenters.name as name, count(distinct hosts.id) as hcount, count(distinct vms.id) as vcount, count(distinct dvportgroups.id) as dvcount, count(distinct dvss.id) as dvsscount, count(distinct vss.name) as vsscount, count(distinct networks.name) as pgcount
from datacenters
join hosts on (datacenters.id=hosts.dc_id)
join vms on (datacenters.id=vms.dc_id)
left join dvportgroups on (datacenters.id=dvportgroups.dc_id)
left join dvss on (datacenters.id=dvss.dc_id)
join vss on (datacenters.id=vss.dc_id)
join networks on (datacenters.id=networks.dc_id)
group by datacenters.id;


CREATE VIEW `vss_vmnics`
AS
SELECT v.name AS name, v.host AS host, vm.name AS pnic, vm.iface AS iface,v.dc_id AS dc_id FROM vss AS v LEFT JOIN vmnics AS vm
  ON(v.name = vm.dvs_nn) AND (v.host_id = vm.host_id)
ORDER BY v.name ASC, host ASC;

COMMIT;

