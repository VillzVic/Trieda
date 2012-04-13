DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version000_To_Version001` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version000_To_Version001`()
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

    IF actual_db_version = 0 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';
      /* TRIEDA-1280: Alterar o BD para armazenar o novo parametro de planejamento "Otimizar por" */
      ALTER TABLE `trieda`.`parametros` ADD COLUMN `par_otimizarpor` VARCHAR(20) DEFAULT NULL AFTER `par_niveldifdisci`;
      UPDATE `trieda`.`parametros` SET `par_otimizarpor` = 'BLOCOCURRICULAR';

      /* A coluna que armazena o nome do professor estava com tamanho muito pequeno (no caso, VARCHAR(20)) */
      ALTER TABLE `trieda`.`professores` MODIFY COLUMN `prf_nome` VARCHAR(255) NOT NULL;

      /* TRIEDA-1325: Atualizar o BD de modo que o relacionamento entre parametros e campi, que atualmente eh de 1:1, seja de 1:N */
      DROP TABLE IF EXISTS `trieda`.`parametros_campi`;
      CREATE TABLE  `trieda`.`parametros_campi` (
        `par_id` bigint(20) NOT NULL,
        `cam_id` bigint(20) NOT NULL,
        PRIMARY KEY (`par_id`,`cam_id`),
        KEY `FKE2C87FE3D74052F6` (`par_id`),
        KEY `FKE2C87FE35121F2A6` (`cam_id`),
        CONSTRAINT `FKE2C87FE35121F2A6` FOREIGN KEY (`cam_id`) REFERENCES `campi` (`cam_id`),
        CONSTRAINT `FKE2C87FE3D74052F6` FOREIGN KEY (`par_id`) REFERENCES `parametros` (`par_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      INSERT INTO `trieda`.`parametros_campi` (`par_id`,`cam_id`) SELECT DISTINCT `par_id`, `cam_id` FROM `trieda`.`parametros`;
      ALTER TABLE `trieda`.`parametros` DROP FOREIGN KEY `FK1B57F25A5121F2A6`;
      ALTER TABLE `trieda`.`parametros` DROP COLUMN `cam_id`;

      /* TRIEDA-1306: Armazenar no BD o controle das requisicoes de otimizacao */
      DROP TABLE IF EXISTS `requisicao_otimizacao`;
      CREATE TABLE `requisicao_otimizacao` (
        `rop_id` bigint(20) NOT NULL AUTO_INCREMENT,
        `rop_round` bigint(20) NOT NULL,
        `version` int(11) DEFAULT NULL,
        `cen_id` bigint(20) NOT NULL,
        `username` varchar(50) NOT NULL,
        `par_id` bigint(20) NOT NULL,
        PRIMARY KEY (`rop_id`),
        UNIQUE KEY `username` (`username`,`rop_round`,`cen_id`),
        KEY `FK28A7CC4079DB6948` (`username`),
        KEY `FK28A7CC4067EDB695` (`cen_id`),
        KEY `FK28A7CC40D74052F6` (`par_id`),
        CONSTRAINT `FK28A7CC40D74052F6` FOREIGN KEY (`par_id`) REFERENCES `parametros` (`par_id`),
        CONSTRAINT `FK28A7CC4067EDB695` FOREIGN KEY (`cen_id`) REFERENCES `cenarios` (`cen_id`),
        CONSTRAINT `FK28A7CC4079DB6948` FOREIGN KEY (`username`) REFERENCES `users` (`username`)
      ) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
      ALTER TABLE `trieda`.`parametros` ADD COLUMN `rop_id` bigint(20) DEFAULT NULL;
      ALTER TABLE `trieda`.`parametros` ADD CONSTRAINT `FK1B57F25A9D41A9F6` FOREIGN KEY (`rop_id`) REFERENCES `requisicao_otimizacao` (`rop_id`);

      /* TRIEDA-1312: Adaptar BD para armazenar informacao de Atendimentos de Demandas de Alunos enviada pelo solver no XML de output */
      DROP TABLE IF EXISTS `alunos_demanda_atendimentos_operacional`;
      CREATE TABLE `alunos_demanda_atendimentos_operacional` (
        `alunos_demanda` bigint(20) NOT NULL,
        `atendimentos_operacional` bigint(20) NOT NULL,
        PRIMARY KEY (`alunos_demanda`,`atendimentos_operacional`),
        KEY `FK5A2D64C7F2EF1866` (`alunos_demanda`),
        KEY `FK5A2D64C77ACBBB36` (`atendimentos_operacional`),
        CONSTRAINT `FK5A2D64C77ACBBB36` FOREIGN KEY (`atendimentos_operacional`) REFERENCES `atendimento_operacional` (`atp_id`),
        CONSTRAINT `FK5A2D64C7F2EF1866` FOREIGN KEY (`alunos_demanda`) REFERENCES `alunos_demanda` (`ald_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;
      DROP TABLE IF EXISTS `alunos_demanda_atendimentos_tatico`;
      CREATE TABLE `alunos_demanda_atendimentos_tatico` (
        `alunos_demanda` bigint(20) NOT NULL,
        `atendimentos_tatico` bigint(20) NOT NULL,
        PRIMARY KEY (`alunos_demanda`,`atendimentos_tatico`),
        KEY `FKDDCB9B8AF2EF1866` (`alunos_demanda`),
        KEY `FKDDCB9B8A51EFDC6C` (`atendimentos_tatico`),
        CONSTRAINT `FKDDCB9B8A51EFDC6C` FOREIGN KEY (`atendimentos_tatico`) REFERENCES `atendimento_tatico` (`att_id`),
        CONSTRAINT `FKDDCB9B8AF2EF1866` FOREIGN KEY (`alunos_demanda`) REFERENCES `alunos_demanda` (`ald_id`)
      ) ENGINE=InnoDB DEFAULT CHARSET=latin1;

      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (1);
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

CALL convertDBFrom_Version000_To_Version001();