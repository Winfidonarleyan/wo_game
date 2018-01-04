DROP TABLE IF EXISTS `character_enchant_visual`;
CREATE TABLE `character_enchant_visual` (
  `itemGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `visualEntry` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`itemGuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;