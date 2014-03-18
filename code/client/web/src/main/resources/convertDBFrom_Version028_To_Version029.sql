DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version028_To_Version029` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version028_To_Version029`()
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

    IF actual_db_version = 28 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      /* TRIEDA-1788: Motivos de Uso de Professores Virtuais */
      ALTER TABLE `trieda`.`professores_virtuais` ADD COLUMN `tco_id` BIGINT(20) AFTER `tit_id`,
        ADD CONSTRAINT `FK_TCO_ID_PRV` FOREIGN KEY `FK_TCO_ID_PRV` (`tco_id`) REFERENCES `tipos_contrato` (`tco_id`);
      DROP TABLE IF EXISTS `trieda`.`motivos_uso_prof_virtual`;
      CREATE TABLE  `trieda`.`motivos_uso_prof_virtual` (
        `mot_prv_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `mot_prv_motivo_uso` LONGTEXT NOT NULL,
        `prf_id` bigint(20),
        `version` INTEGER,
        PRIMARY KEY (`mot_prv_id`),
        KEY `FK_MOT_PRF_ID` (`prf_id`),
        CONSTRAINT `FK_MOT_PRF_ID` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      DROP TABLE IF EXISTS `trieda`.`dicas_eliminacao_prof_virtual`;
      CREATE TABLE  `trieda`.`dicas_eliminacao_prof_virtual` (
        `dic_prv_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `dic_prv_dica_eliminacao` LONGTEXT NOT NULL,
        `prf_id` bigint(20) NOT NULL,
        `version` INTEGER,
        PRIMARY KEY (`dic_prv_id`),
        KEY `FK_DIC_PRF_ID` (`prf_id`),
        CONSTRAINT `FK_DIC_PRF_ID` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      DROP TABLE IF EXISTS `trieda`.`atendimento_operacional_motivos_uso`;
      CREATE TABLE  `trieda`.`atendimento_operacional_motivos_uso` (
        `atp_id` bigint(20) NOT NULL,
        `mot_prv_id` bigint(20) NOT NULL,
        KEY `FK_ATP_MOT_ATP_ID` (`atp_id`),
        KEY `FK_ATP_MOT_MOT_PRV_ID` (`mot_prv_id`),
        CONSTRAINT `FK_ATP_MOT_ATP_ID` FOREIGN KEY (`atp_id`) REFERENCES `atendimento_operacional` (`atp_id`),
        CONSTRAINT `FK_ATP_MOT_MOT_PRV_ID` FOREIGN KEY (`mot_prv_id`) REFERENCES `motivos_uso_prof_virtual` (`mot_prv_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      DROP TABLE IF EXISTS `trieda`.`atendimento_operacional_dicas_eliminacao`;
      CREATE TABLE  `trieda`.`atendimento_operacional_dicas_eliminacao` (
        `atp_id` bigint(20) NOT NULL,
        `dic_prv_id` bigint(20) NOT NULL,
        KEY `FK_ATP_DIC_ATP_ID` (`atp_id`),
        KEY `FK_ATP_DIC_DIC_PRV_ID` (`dic_prv_id`),
        CONSTRAINT `FK_ATP_DIC_ATP_ID` FOREIGN KEY (`atp_id`) REFERENCES `atendimento_operacional` (`atp_id`),
        CONSTRAINT `FK_ATP_DIC_DIC_PRV_ID` FOREIGN KEY (`dic_prv_id`) REFERENCES `dicas_eliminacao_prof_virtual` (`dic_prv_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;

      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (29);
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

CALL convertDBFrom_Version028_To_Version029();