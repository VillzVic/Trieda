DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version008_To_Version009` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version008_To_Version009`()
BEGIN
  DECLARE status_code INT;
  DECLARE count_table_db_version INT;
  DECLARE actual_db_version INT;
  DECLARE msg_status VARCHAR(500);

  /**
   * exit handler for anything that goes wrong during execution. This will
   * ensure the any subsequent DML statements are rolled back
   */
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    SET status_code = -1;
    ROLLBACK;
    SELECT status_code AS status_code, msg_status AS msg_status;
  END;

  SET status_code = 0;
  SET msg_status = 'Inicio da Transacao';

  /* start the transaction - so that anything that follows will be atomic */
  START TRANSACTION;

    /* verifica em que versao a base de dados se encontra */
    SET msg_status = 'Inicio verificacao qual a versao da base de dados';
    SET @schema = 'trieda';
    SET @table_name = 'db_version';
    SELECT COUNT(*) INTO count_table_db_version FROM information_schema.tables WHERE table_schema = @schema AND table_name LIKE @table_name;
    IF count_table_db_version < 1 THEN
      CREATE TABLE `trieda`.`db_version` (
        `db_version` INTEGER UNSIGNED NOT NULL,
        PRIMARY KEY (`db_version`)
      ) ENGINE = InnoDB;
      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (0);
    END IF;
    SET actual_db_version = (SELECT MAX(`db_version`) FROM `trieda`.`db_version`);
    SET msg_status = 'Versao da base de dados identificada';

    IF actual_db_version = 8 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';
      
      /* TRIEDA-1617: Permitir regras de equivalências por curso */
      RENAME TABLE `trieda`.`equivalencias` TO `equivalencias_old`;
      CREATE TABLE  `trieda`.`equivalencias` (
     	`eqv_id` bigint(20) NOT NULL,
        `dis_cursou_id` bigint(20) NOT NULL,
        `dis_elimina_id` bigint(20) NOT NULL,
        `eqv_geral` BIT(1),
        `version` INTEGER,
        PRIMARY KEY (`eqv_id`),
        KEY `FK_EQV_DIS_CURSOU_ID` (`dis_cursou_id`),
        KEY `FK_EQV_DIS_ELIMINA_ID` (`dis_elimina_id`),
        CONSTRAINT `FK_EQV_DIS_CURSOU_ID` FOREIGN KEY (`dis_cursou_id`) REFERENCES `disciplinas` (`dis_id`),
        CONSTRAINT `FK_EQV_DIS_ELIMINA_ID` FOREIGN KEY (`dis_elimina_id`) REFERENCES `disciplinas` (`dis_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      CREATE TABLE  `trieda`.`cursos_equivalencias` (
        `cur_id` bigint(20) NOT NULL,
        `eqv_id` bigint(20) NOT NULL,
        PRIMARY KEY (`cur_id`,`eqv_id`),
        KEY `FK_CUR_EQV_CUR_ID` (`cur_id`),
        KEY `FK_CUR_EQV_EQV_ID` (`eqv_id`),
        CONSTRAINT `FK_CUR_EQV_CUR_ID` FOREIGN KEY (`cur_id`) REFERENCES `cursos` (`cur_id`),
        CONSTRAINT `FK_CUR_EQV_EQV_ID` FOREIGN KEY (`eqv_id`) REFERENCES `equivalencias` (`eqv_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      INSERT INTO `trieda`.`equivalencias` (`eqv_id`,`dis_cursou_id`,`dis_elimina_id`, `eqv_geral`, `version`) SELECT `eqv_id`, `dis_cursou_id`, `elimina`, b'1', 0 FROM `trieda`.`equivalencias_old` 
      	INNER JOIN `trieda`.`equivalencias_elimina` ON `eqv_id` = `eliminada_por`;
      DROP TABLE IF EXISTS `trieda`.`equivalencias_elimina`;
      DROP TABLE IF EXISTS `trieda`.`equivalencias_old`;
      
      
      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (9);
      SET msg_status = 'Base de dados compativel com script de conversao, conversao realizada';
    ELSE
      SET msg_status = 'Base de dados nao compativel com script de conversao, conversao nao efetuada';
    END IF;
    
  /* if status_code of 0, then commit, else roll back */
  IF (status_code = 0) THEN
    COMMIT;
  ELSE
    ROLLBACK;
  END IF;
    
  SELECT status_code AS status_code, msg_status AS msg_status;
END $$

DELIMITER;

CALL convertDBFrom_Version008_To_Version009();