DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version011_To_Version012` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version011_To_Version012`()
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

    IF actual_db_version = 11 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';
      
      /* TRIEDA-1471: Reestruturar associação de disciplinas com salas */
      DROP TABLE IF EXISTS `trieda`.`disciplinas_salas`;
      DROP TABLE IF EXISTS `trieda`.`disciplinas_grupos_sala`;
      CREATE TABLE  `trieda`.`disciplinas_salas` (
        `dis_id` bigint(20) NOT NULL,
        `sal_id` bigint(20) NOT NULL,
        KEY `FK_DIS_SAL_DIS_ID` (`dis_id`),
        KEY `FK_DIS_SAL_SAL_ID` (`sal_id`),
        CONSTRAINT `FK_DIS_SAL_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`),
        CONSTRAINT `FK_DIS_SAL_SAL_ID` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      CREATE TABLE  `trieda`.`disciplinas_grupos_sala` (
        `dis_id` bigint(20) NOT NULL,
        `grs_id` bigint(20) NOT NULL,
        KEY `FK_DIS_GRS_DIS_ID` (`dis_id`),
        KEY `FK_DIS_GRS_GRS_ID` (`grs_id`),
        CONSTRAINT `FK_DIS_GRS_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`),
        CONSTRAINT `FK_DIS_GRS_GRS_ID` FOREIGN KEY (`grs_id`) REFERENCES `grupos_sala` (`grs_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      INSERT INTO `trieda`.`disciplinas_salas` (`dis_id`,`sal_id`) SELECT `dis_id`, `salas` FROM `trieda`.`curriculos_disciplinas_salas`
      	INNER JOIN `trieda`.`curriculos_disciplinas` ON `cdi_id` = `curriculo_disciplinas`;
      INSERT INTO `trieda`.`disciplinas_grupos_sala` (`dis_id`,`grs_id`) SELECT `dis_id`, `grupos_sala` FROM `trieda`.`grupos_sala_curriculo_disciplinas`
      	INNER JOIN `trieda`.`curriculos_disciplinas` ON `cdi_id` = `curriculo_disciplinas`;
      DROP TABLE IF EXISTS `trieda`.`curriculos_disciplinas_salas`;
      DROP TABLE IF EXISTS `trieda`.`grupos_sala_curriculo_disciplinas`;

      
      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (12);
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

CALL convertDBFrom_Version011_To_Version012();