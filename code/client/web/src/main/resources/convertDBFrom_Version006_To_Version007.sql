DELIMITER $$

DROP PROCEDURE IF EXISTS `convertDBFrom_Version006_To_Version007` $$
CREATE DEFINER=`trieda`@`%` PROCEDURE `convertDBFrom_Version006_To_Version007`()
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

    IF actual_db_version = 6 THEN
      SET msg_status = 'Base de dados compativel com script de conversao, inicio processo conversao';
      
	/*TRIEDA-1543 - Automatizar gravação de informação de uso de sábados e uso de domingos via importação de excel */
    ALTER TABLE `trieda`.`disciplinas` ADD `dis_usa_sabado` BIT(1) DEFAULT NULL;
    ALTER TABLE `trieda`.`disciplinas` ADD `dis_usa_domingo` BIT(1) DEFAULT NULL;
	UPDATE `trieda`.`disciplinas` d SET dis_usa_sabado = 
		(SELECT CASE WHEN EXISTS (SELECT disciplinas
			FROM horario_disponivel_cenario INNER JOIN horario_disponivel_cenario_disciplinas h ON hdc_id = horarios WHERE dia_semana = 5 and d.dis_id = h.disciplinas)
		THEN b'1'
		ELSE b'0'
		END);
	UPDATE `trieda`.`disciplinas` d SET dis_usa_domingo = 
		(SELECT CASE WHEN EXISTS (SELECT disciplinas
			FROM horario_disponivel_cenario INNER JOIN horario_disponivel_cenario_disciplinas h ON hdc_id = horarios WHERE dia_semana = 6 and d.dis_id = h.disciplinas)
		THEN b'1'
		ELSE b'0'
		END);

		
      INSERT INTO `trieda`.`db_version` (`db_version`) VALUES (7);
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

CALL convertDBFrom_Version006_To_Version007();