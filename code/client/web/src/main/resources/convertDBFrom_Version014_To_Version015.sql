DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version014_To_Version015` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version014_To_Version015`()
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

    IF actual_db_version = 14 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';
      
      /* TRIEDA-1713: Criar funcionalidade de geração de demanda */
      DROP TABLE IF EXISTS `trieda`.`curriculos_disciplinas_disciplinas_prerequisitos`;
      DROP TABLE IF EXISTS `trieda`.`curriculos_disciplinas_disciplinas_corequisitos`;
      DROP TABLE IF EXISTS `trieda`.`curriculos_disciplinas_alunos`;
      CREATE TABLE  `trieda`.`curriculos_disciplinas_disciplinas_prerequisitos` (
        `cdi_id` bigint(20) NOT NULL,
        `dis_id` bigint(20) NOT NULL,
        KEY `FK_CDI_DIS_PRE_CDI_ID` (`cdi_id`),
        KEY `FK_CDI_DIS_PRE_DIS_ID` (`dis_id`),
        CONSTRAINT `FK_CDI_DIS_PRE_CDI_ID` FOREIGN KEY (`cdi_id`) REFERENCES `curriculos_disciplinas` (`cdi_id`),
        CONSTRAINT `FK_CDI_DIS_PRE_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      CREATE TABLE  `trieda`.`curriculos_disciplinas_disciplinas_corequisitos` (
        `cdi_id` bigint(20) NOT NULL,
        `dis_id` bigint(20) NOT NULL,
        KEY `FK_CDI_DIS_CO_CDI_ID` (`cdi_id`),
        KEY `FK_CDI_DIS_CO_DIS_ID` (`dis_id`),
        CONSTRAINT `FK_CDI_DIS_CO_CDI_ID` FOREIGN KEY (`cdi_id`) REFERENCES `curriculos_disciplinas` (`cdi_id`),
        CONSTRAINT `FK_CDI_DIS_CO_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      CREATE TABLE  `trieda`.`curriculos_disciplinas_alunos` (
        `cdi_id` bigint(20) NOT NULL,
        `aln_id` bigint(20) NOT NULL,
        KEY `FK_CDI_ALN_CDI_ID` (`cdi_id`),
        KEY `FK_CDI_ALN_ALN_ID` (`aln_id`),
        CONSTRAINT `FK_CDI_ALN_CDI_ID` FOREIGN KEY (`cdi_id`) REFERENCES `curriculos_disciplinas` (`cdi_id`),
        CONSTRAINT `FK_CDI_ALN_ALN_ID` FOREIGN KEY (`aln_id`) REFERENCES `alunos` (`aln_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      ALTER TABLE  `trieda`.`alunos` ADD COLUMN `aln_periodo` INTEGER AFTER `aln_formando`;
      ALTER TABLE  `trieda`.`disciplinas` ADD COLUMN `dis_carga_horaria` INTEGER AFTER `dis_usa_domingo`;
      ALTER TABLE  `trieda`.`curriculos_disciplinas` ADD COLUMN `cdi_maturidade` INTEGER AFTER `cdi_periodo`;


      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (15);
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

CALL convertDBFrom_Version014_To_Version015();