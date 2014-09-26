DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version036_To_Version037` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version036_To_Version037`()
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

    IF actual_db_version = 36 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';

      /* TRIEDA: Adicionando Informacoes na requisicao de otimizacao */
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_instante_inicio_requisicao` DATETIME;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_instante_inicio_otimizacao` DATETIME;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_instante_termino` DATETIME;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_status_cod` INTEGER;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_status_texto` VARCHAR(255);
      ALTER TABLE `trieda`.`requisicao_otimizacao` ADD COLUMN `username_cancelamento` varchar(50),
        ADD CONSTRAINT `FK_ROP_USER_CANC` FOREIGN KEY `FK_ROP_USER_CANC` (`username_cancelamento`) REFERENCES `users` (`username`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` DROP FOREIGN KEY FK28A7CC4079DB6948;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` CHANGE `username` `username_criacao` varchar(50),
        ADD CONSTRAINT `FK_ROP_USER_CRIA` FOREIGN KEY `FK_ROP_USER_CRIA` (`username_criacao`) REFERENCES `users` (`username`)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_total_campi` INTEGER;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_campi_selecionados` VARCHAR(225);
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_total_turnos` INTEGER;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_turnos_selecionados` VARCHAR(255);
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_total_alunos` INTEGER;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_total_alunos_demandas_p1` INTEGER;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_total_alunos_demandas_p2` INTEGER;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_total_ambientes` INTEGER;
      ALTER TABLE  `trieda`.`requisicao_otimizacao` ADD COLUMN `rop_total_professores` INTEGER;


      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (37);
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

CALL convertDBFrom_Version036_To_Version037();