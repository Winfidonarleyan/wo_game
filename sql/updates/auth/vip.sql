DROP TABLE IF EXISTS `account_vip`;
CREATE TABLE `account_vip` (
  `id` int(11) unsigned NOT NULL,
  `expires` int(10) unsigned NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;