DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version029_To_Version030` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version029_To_Version030`()
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

    IF actual_db_version = 29 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      DROP TABLE IF EXISTS `trieda`.`aulas_turmas`;
      DROP TABLE IF EXISTS `trieda`.`aulas`;
      DROP TABLE IF EXISTS `trieda`.`turmas_alunos`;
      DROP TABLE IF EXISTS `trieda`.`turmas`;

      CREATE TABLE  `trieda`.`turmas` (
        `tur_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `tur_nome` VARCHAR(255) NOT NULL,
        `tur_parcial` BIT(1) NOT NULL,
        `dis_id` bigint(20),
        `cen_id` bigint(20),
        `version` INTEGER,
        PRIMARY KEY (`tur_id`),
        KEY `FK_TUR_DIS_ID` (`dis_id`),
        KEY `FK_TUR_CEN_ID` (`cen_id`),
        CONSTRAINT `FK_TUR_DIS_ID` FOREIGN KEY (`dis_id`) REFERENCES `disciplinas` (`dis_id`),
        CONSTRAINT `FK_TUR_CEN_ID` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;

      CREATE TABLE  `trieda`.`turmas_alunos` (
        `tur_id` bigint(20) NOT NULL,
        `ald_id` bigint(20) NOT NULL,
        KEY `FK_TUR_ALD_TUR_ID` (`tur_id`),
        KEY `FK_TUR_ALD_ALD_ID` (`ald_id`),
        CONSTRAINT `FK_TUR_ALD_TUR_ID` FOREIGN KEY (`tur_id`) REFERENCES `turmas` (`tur_id`),
        CONSTRAINT `FK_TUR_ALD_ALD_ID` FOREIGN KEY (`ald_id`) REFERENCES `alunos_demanda` (`ald_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;

      CREATE TABLE  `trieda`.`aulas` (
        `aul_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `aul_qtd_cred_teoricos` INTEGER NOT NULL,
        `aul_qtd_cred_praticos` INTEGER NOT NULL,
        `hdc_id` bigint(20) NOT NULL,
        `sal_id` bigint(20) NOT NULL,
        `prf_id` bigint(20),
        `prv_id` bigint(20),
        `cen_id` bigint(20) NOT NULL,
        `version` INTEGER,
        PRIMARY KEY (`aul_id`),
        KEY `FK_AUL_HDC_ID` (`hdc_id`),
        KEY `FK_AUL_SAL_ID` (`sal_id`),
        KEY `FK_AUL_PRF_ID` (`prf_id`),
        KEY `FK_AUL_PRV_ID` (`prv_id`),
        KEY `FK_AUL_CEN_ID` (`cen_id`),
        CONSTRAINT `FK_AUL_HDC_ID` FOREIGN KEY (`hdc_id`) REFERENCES `horario_disponivel_cenario` (`hdc_id`),
        CONSTRAINT `FK_AUL_SAL_ID` FOREIGN KEY (`sal_id`) REFERENCES `salas` (`sal_id`),
        CONSTRAINT `FK_AUL_PRF_ID` FOREIGN KEY (`prf_id`) REFERENCES `professores` (`prf_id`),
        CONSTRAINT `FK_AUL_PRV_ID` FOREIGN KEY (`prv_id`) REFERENCES `professores_virtuais` (`prf_id`),
        CONSTRAINT `FK_AUL_CEN_ID` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;

      CREATE TABLE  `trieda`.`aulas_turmas` (
        `aul_id` bigint(20) NOT NULL,
        `tur_id` bigint(20) NOT NULL,
        KEY `FK_AUL_TUR_AUL_ID` (`aul_id`),
        KEY `FK_AUL_TUR_TUR_ID` (`tur_id`),
        CONSTRAINT `FK_AUL_TUR_AUL_ID` FOREIGN KEY (`aul_id`) REFERENCES `aulas` (`aul_id`),
        CONSTRAINT `FK_AUL_TUR_TUR_ID` FOREIGN KEY (`tur_id`) REFERENCES `turmas` (`tur_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;

	  SET msg_status = 'Conversao realizada';
	   
      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (30);
      
	  SET msg_status = 'Versao do BD incrementada';
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

CALL convertDBFrom_Version029_To_Version030();