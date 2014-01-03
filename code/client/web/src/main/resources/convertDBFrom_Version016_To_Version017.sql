DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version016_To_Version017` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version016_To_Version017`()
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

    IF actual_db_version = 16 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';
      
      /* TRIEDA-1684: Adicionar suporte a cenário nas telas do Trieda. */
      ALTER TABLE `trieda`.`areas_titulacao` ADD COLUMN `cen_id` BIGINT(20),
	  	ADD CONSTRAINT `FK_CEN_ID_ATT` FOREIGN KEY `FK_CEN_ID_ATT` (`cen_id`) REFERENCES `cenarios` (`cen_id`);
	  UPDATE `trieda`.`areas_titulacao` d SET cen_id = (SELECT cen_id FROM `trieda`.`cenarios` WHERE `cen_masterdata` is TRUE LIMIT 1);
	  ALTER TABLE `trieda`.`professores_virtuais` ADD COLUMN `cen_id` BIGINT(20),
	  	ADD CONSTRAINT `FK_CEN_ID_PRV` FOREIGN KEY `FK_CEN_ID_PRV` (`cen_id`) REFERENCES `cenarios` (`cen_id`);
	  UPDATE `trieda`.`professores_virtuais` d SET cen_id = (SELECT cen_id FROM `trieda`.`cenarios` WHERE `cen_masterdata` is TRUE LIMIT 1);
	  ALTER TABLE `trieda`.`semana_letiva` ADD COLUMN `cen_id` BIGINT(20),
	  	ADD CONSTRAINT `FK_CEN_ID_SLE` FOREIGN KEY `FK_CEN_ID_SLE` (`cen_id`) REFERENCES `cenarios` (`cen_id`);
	  UPDATE `trieda`.`semana_letiva` d SET cen_id = (SELECT cen_id FROM `trieda`.`cenarios` WHERE `cen_masterdata` is TRUE LIMIT 1);
	  ALTER TABLE `trieda`.`tipos_contrato` ADD COLUMN `cen_id` BIGINT(20),
	  	ADD CONSTRAINT `FK_CEN_ID_TCO` FOREIGN KEY `FK_CEN_ID_TCO` (`cen_id`) REFERENCES `cenarios` (`cen_id`);
	  UPDATE `trieda`.`tipos_contrato` d SET cen_id = (SELECT cen_id FROM `trieda`.`cenarios` WHERE `cen_masterdata` is TRUE LIMIT 1);
	  ALTER TABLE `trieda`.`tipos_curso` ADD COLUMN `cen_id` BIGINT(20),
	  	ADD CONSTRAINT `FK_CEN_ID_TCU` FOREIGN KEY `FK_CEN_ID_TCU` (`cen_id`) REFERENCES `cenarios` (`cen_id`);
	  UPDATE `trieda`.`tipos_curso` d SET cen_id = (SELECT cen_id FROM `trieda`.`cenarios` WHERE `cen_masterdata` is TRUE LIMIT 1);
	  ALTER TABLE `trieda`.`tipos_disciplina` ADD COLUMN `cen_id` BIGINT(20),
	  	ADD CONSTRAINT `FK_CEN_ID_TDI` FOREIGN KEY `FK_CEN_ID_TDI` (`cen_id`) REFERENCES `cenarios` (`cen_id`);
	  UPDATE `trieda`.`tipos_disciplina` d SET cen_id = (SELECT cen_id FROM `trieda`.`cenarios` WHERE `cen_masterdata` is TRUE LIMIT 1);
	  ALTER TABLE `trieda`.`tipos_sala` ADD COLUMN `cen_id` BIGINT(20),
	  	ADD CONSTRAINT `FK_CEN_ID_TSA` FOREIGN KEY `FK_CEN_ID_TSA` (`cen_id`) REFERENCES `cenarios` (`cen_id`);
	  UPDATE `trieda`.`tipos_sala` d SET cen_id = (SELECT cen_id FROM `trieda`.`cenarios` WHERE `cen_masterdata` is TRUE LIMIT 1);
	  ALTER TABLE `trieda`.`titulacoes` ADD COLUMN `cen_id` BIGINT(20),
	  	ADD CONSTRAINT `FK_CEN_ID_TIT` FOREIGN KEY `FK_CEN_ID_TIT` (`cen_id`) REFERENCES `cenarios` (`cen_id`);
	  UPDATE `trieda`.`titulacoes` d SET cen_id = (SELECT cen_id FROM `trieda`.`cenarios` WHERE `cen_masterdata` is TRUE LIMIT 1);
	  DROP INDEX sle_codigo ON semana_letiva;
	  CREATE INDEX sle_codigo ON semana_letiva (sle_codigo, cen_id);



      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (17);
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

CALL convertDBFrom_Version016_To_Version017();